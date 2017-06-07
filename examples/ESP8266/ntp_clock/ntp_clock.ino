/* -------------------------------------------------
   NTP Clock
   using ESP8266 and Lixie Displays!

   by Connor Nishijima - 12/28/2016
   -------------------------------------------------

   To use your Lixie Displays / ESP8266 as an NTP
   clock, you'll need a few things:

   - WIFI_SSID
   - WIFI_PASSWORD
   - SIX_DIGIT
   - TIME_COLOR_RGB
   - TIME_OFFSET

  SIX_DIGIT is a true/false for 6 or 4 digit clocks
  TIME_COLOR_RGB is an 8-bit RGB value to color the displays
  TIME_OFFSET is your local UTC offset for time zones

   -------------------------------------------------
*/

#include "Lixie.h" // Include Lixie Library
#define DATA_PIN   5
#define NUM_LIXIES 4
Lixie lix(DATA_PIN, NUM_LIXIES);

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h>   // WifiMulti Lib for connection handling
ESP8266WiFiMulti WiFiMulti;

//---------------------------------------
const char* WIFI_SSID = "";  //  your network SSID (name)
const char* WIFI_PASS = "";  //  your network password

const bool HOUR_12 = true;    // 12-hour format
const bool SIX_DIGIT = false; // True if 6-digit clock with seconds
const byte TIME_COLOR_RGB[3] = {0,255,255}; // CYAN
const int TIME_OFFSET = -7;     // US Mountain Time
//---------------------------------------

// NTP Servers:
static const char ntpServerName[] = "us.pool.ntp.org";
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

time_t getNtpTime();
void digitalClockDisplay();
void sendNTPpacket(IPAddress &address);

void setup()
{
  lix.begin(); // Initialize LEDs
  Serial.begin(115200);
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASS); // Your WIFI credentials

  // This sets all lights to yellow while we're connecting to WIFI
  while ((WiFiMulti.run() != WL_CONNECTED)) {
    lix.color(255, 255, 0);
    lix.write(8888);
    delay(100);
  }

  // Green on connection success
  lix.color(0, 255, 0);
  lix.write(9999);
  delay(500);

  // Reset colors to default
  lix.color(255, 255, 255);
  lix.clear();

  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
}

time_t prevDisplay = 0; // when the digital clock was displayed

void loop()
{
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      digitalClockDisplay();
    }
  }
}

void digitalClockDisplay()
{
  // Using 1,000,000 as our base number creates zero-padded times.
  uint32_t sum = 1000000;

  // Put the hour on two digits,
  sum += format_hour(hour())*10000;
  // The minute on two more,
  sum += minute()*100;
  // and the seconds on two more.
  sum += second();

  // Take out the seconds if we just have a 4-digit clock
  if(SIX_DIGIT == false){
    sum /= 100;
  }

  lix.color(TIME_COLOR_RGB[0],TIME_COLOR_RGB[1],TIME_COLOR_RGB[2]);
  lix.write(sum); // write our formatted long
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + TIME_OFFSET * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

byte format_hour(byte hr){
  if(hr > 12){
    hr-=12;
  }
  if(hr == 0){
    hr = 12;
  }
  return hr;
}
