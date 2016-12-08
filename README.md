![Lixie Library](http://i.imgur.com/nFgz0Zt.jpg)

#Library for driving Lixie edge-lit displays!
*Disclaimer: This library is still in early development and may change drastically in the coming weeks!*

This library allows for easy writing to Lixie digit displays! It takes care of all the addressing and updating for you!

#Getting Started:

    #include "Lixie.h" // Include Lixie Library
    Lixie lix;         // Set class nickname for faster coding
    
    void setup() {
      lix.begin();                     // Initialize LEDs
      lix.color_on_hsv(127,255,255);   // Set color with HSV (CYAN)
    }
    
    void loop() {
      lix.write_int( millis() );       // Write millis() to the displays
      delay(1);
    }

----------
# Functions

**lix.begin**();

Inititalizes the underlying FastLED library and sets up variables like the digit colors.

**lix.clear**();

Clears all displays, all lights off.

**lix.write_int**(unsigned int **input**);

Clears the displays, and pushes in a multi-digit integer as in: 2016

**lix.write_string**(char\* **input**);

Clears the displays, and pushes in a char array as in: "2016". This ignores any non-numeric chars in the string, allowing you to send "12:52:47 PM" and have the displays show "12 52 47" for a clock.

**lix.color_on_rgb**(byte **r**, byte **g**, byte **b**);

Sets the "on" color of the digits using RGB. This is the color of an active number in the display. (Default: 255,255,255)

**lix.color_off_rgb**(byte **r**, byte **g**, byte **b**);

Sets the "off" color of the digits using RGB. This is the color of all inactive numbers in the display. (Default: 0,0,0)

**lix.color_on_hsv**(byte **h**, byte **s**, byte **v**);

Sets the "on" color of the digits using HSV. This is the color of an active number in the display. (Default: 0,0,255)

**lix.color_off_hsv**(byte **h**, byte **s**, byte **v**);

Sets the "off" color of the digits using HSV. This is the color of all inactive numbers in the display. (Default: 0,0,0)
