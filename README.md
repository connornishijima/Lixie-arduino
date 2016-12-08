#Library for driving Lixie edge-lit displays!
*Disclaimer: This library is still in early development and may change drastically in the coming weeks!*

This library allows for easy writing to Lixie digit displays! It takes care of all the addressing and updating for you!

#Getting Started:

    #include "Lixie.h" // Include Lixie Library
    Lixie lix;         // Set class nickname for faster coding
    
    void setup() {
      lix.begin();                     // Initialize LEDs
      lix.color_on_hsv(127,255,255);   // Set color with HSV (CYAN)
      //lix.color_on_rgb(0,255,255);   // Set color with RGB (CYAN)
    }
    
    void loop() {
      lix.write_int( millis() );       // Write millis() to the displays
      delay(1);
    }
