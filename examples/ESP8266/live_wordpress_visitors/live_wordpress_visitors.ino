/* -------------------------------------------------
 * Live Visitor Counter for Wordpress Installations
 * using ESP8266 and Lixie Displays!
 * 
 * by Connor Nishijima - 12/22/2016
 * -------------------------------------------------
 * 
 * To use your Lixie Displays / ESP8266 as a live
 * Wordpress active visitor counter, you need to
 * install and activate the WP Live Statistics
 * plugin by Paratheme. This plugin will keep a 
 * live count of users browsing your site, which
 * the ESP8266 can request and write to your Lixies!
 * 
 * To start, here's what you need to provide below:
 * 
 * - WIFI_SSID
 * - WIFI_PASSWORD
 * - WORDPRESS_URL
 * 
 * Your WordPress URL is the root of your website -
 * For example: "http://connor-n.com"
 * 
 * WP Live Statistics Plugin:
 * https://wordpress.org/plugins/wp-live-statistics/
 * 
 * -------------------------------------------------
 */

#include "Lixie.h" // Include Lixie Library
#define DATA_PIN   5
#define NUM_LIXIES 4
Lixie lix(DATA_PIN, NUM_LIXIES);

#include <ESP8266WiFi.h>        // ESP8266 WIFI Lib
#include <ESP8266WiFiMulti.h>   // WifiMulti Lib for connection handling
#include <ESP8266HTTPClient.h>  // HTTPClient for web requests
ESP8266WiFiMulti WiFiMulti;

char* WIFI_SSID = "";
char* WIFI_PASS = "";
char* WORDPRESS_URL = "http://connor-n.com/lixie"; // This should be the root page of your Wordpress site!

// This is the visitor count used to compare changes over time for growth or loss
uint16_t visitor_count = 0;

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
  lix.color_off(0, 255, 0);
  lix.write(9999);
  delay(500);

  // Reset colors to default
  lix.color(255, 255, 255);
  lix.color_off(0,0,0);
  lix.clear();
}

void loop() {
  checkVisitors(WORDPRESS_URL);
  delay(1000);
}

// FUNCTIONS ----------------------------------------

// Set custom color, fade back to default
void flash_color(byte r, byte g, byte b){
  lix.color(r,g,b);
  lix.show();
  delay(500);
  for(float progress = 0; progress < 1; progress+=0.01){
    lix.color(
      255*progress + r*(1-progress),
      255*progress + g*(1-progress),
      255*progress + b*(1-progress)
    );
    lix.show();
    delay(1);
  }
}

// Check Wordpress live visitor count
void checkVisitors(String wordpress) {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin(wordpress+"/wp-admin/admin-ajax.php?action=wpls_ajax_online_total");
    int httpCode = http.GET();
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        int payload = http.getString().toInt();
        if(payload > visitor_count){
          lix.write(payload);
          flash_color(0,255,0);
        }
        else if(payload < visitor_count){
          lix.write(payload);
          flash_color(255,0,0);
        }
        visitor_count = payload;
        Serial.println(visitor_count);
      }
    }
    http.end();
  }
}
