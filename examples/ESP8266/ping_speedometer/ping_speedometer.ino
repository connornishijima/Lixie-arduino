/* -------------------------------------------------
   Ping Speedometer
   using ESP8266 and Lixie Displays!

   by Connor Nishijima - 12/28/2016
   -------------------------------------------------

   To use your Lixie Displays / ESP8266 as a ping
   speedometer for measuring your internet latency,
   you'll need a few things:

   - WIFI_SSID
   - WIFI_PASSWORD
   - PING_SITE
   - PING_COUNT
   - PING_INTERVAL
   - PING_GOOD
   - PING_BAD

  PING_SITE is the webpage you'd like to ping
  PING_COUNT is the number of times to ping the site
  PING_INTERVAL is how long we wait between pings
  PING_GOOD is a good ping speed for you (default 35ms)
  PING_BAD is a bad ping speed for you (default 100ms)

  This sketch relies on the ESP8266Ping library by
  Daniele Colanardi. You can get it here:

  https://github.com/dancol90/ESP8266Ping

  This is a live meter of your network latency in mS.
  If set up correctly, it will show green for low
  network latency, and red for high network latency.
  However, it has a few caveats:

  - The WiFi of the ESP8266 will always be slower of
    that in your Playstation/PC/Etc. It's more of a
    relative measurement of latency, take it with a
    grain of salt.

  - By default, it's measuring latency from Google,
    which is a very ideal server. If you're using this
    to measure speed for GTA Online (or another
    game/service) you may want to ping the servers
    of that company instead.

   -------------------------------------------------
*/

#include "Lixie.h" // Include Lixie Library
#define DATA_PIN   5
#define NUM_LIXIES 4
Lixie lix(DATA_PIN, NUM_LIXIES);

#include <ESP8266Ping.h>        // ESP8266Ping Lib
#include <ESP8266WiFi.h>        // ESP8266 WIFI Lib
#include <ESP8266WiFiMulti.h>   // WifiMulti Lib for connection handling
ESP8266WiFiMulti WiFiMulti;

char* WIFI_SSID = "";
char* WIFI_PASS = "";

char* PING_SITE = "www.google.com";
uint8_t PING_COUNT = 2;
uint16_t PING_INTERVAL = 250;
uint16_t PING_GOOD = 35;
uint16_t PING_BAD = 100;

void setup() {
  Serial.begin(115200);
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASS); // Your WIFI credentials
  lix.begin(); // Initialize LEDs

  // This sets all lights to yellow while we're connecting to WIFI
  while ((WiFiMulti.run() != WL_CONNECTED)) {
    lix.color(255, 255, 0);
    lix.write(8888);
    delay(100);
  }

  // Green on connection success
  lix.color(0, 255, 0);
  lix.write(8888);
  delay(500);

  // Reset colors to default
  lix.color(255, 255, 255);
  lix.clear();
}

void loop() {
  checkPing();
  delay(PING_INTERVAL);
}

// FUNCTIONS ----------------------------------------

void checkPing(){
  bool ret = Ping.ping(PING_SITE,PING_COUNT);
  if(ret == true){
    int avg_time_ms = Ping.averageTime();

    uint16_t avg_shift = constrain(avg_time_ms-PING_GOOD,0,65535);
    uint16_t bad_shift = constrain(PING_BAD - PING_GOOD,0,65535);

    float rating = constrain(float(avg_shift) / float(bad_shift), 0, 1);
    
    lix.color(255*rating, 255*(1-rating), 0);
    
    lix.write(avg_time_ms);
    Serial.println(avg_time_ms);
  }
  else{
    // ERROR
    lix.color(255, 0, 0);
    lix.write("0000");
  }
}
