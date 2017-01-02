![Lixie Library](http://i.imgur.com/nFgz0Zt.jpg)

#Library for driving Lixie edge-lit displays!
*Disclaimer: This library is still in early development and may change drastically in the coming weeks! It also depends on the "FastLED" library from Daniel Garcia, so make sure you have that installed as well!*

This library allows for easy writing to Lixie digit displays! It takes care of all the addressing and updating for you!

----------
# Contents
- [Installation](#installation)
- [Getting Started](#getting-started)
- [Functions](#functions)
- [Contributing](#contributing)
- [License and credits](#license-and-credits)

----------
# Installation

***The Lixie library relies on the FastLED library from Daniel Garcia, so make sure you have that installed as well!***

**With Arduino Library Manager:**

1. Open *Sketch > Include Library > Manage Libraries* in the Arduino IDE.
2. Search for "Lixie", and select the latest version.
3. Click the Install button and Arduino will prepare the library and examples for you!

**Manual Install:**

1. Click "Clone or Download" above to get an "Lixie-arduino-master.zip" file.
2. Extract it's contents to the libraries folder in your sketchbook.
3. Rename the folder from "Lixie-arduino-master" to "Lixie".

------------
# Getting Started:

    #include "Lixie.h" // Include Lixie Library
    Lixie lix;         // Set class nickname for faster coding
    
    void setup() {
      lix.begin();                     // Initialize LEDs
      lix.color_on(0,255,255);   // Set color with RGB (CYAN)
    }
    
    void loop() {
      lix.write( millis() );       // Write millis() to the displays
      delay(1);
    }

----------
# Functions

**lix.begin**();

Inititalizes the underlying FastLED library and sets up variables like the digit colors.

**lix.clear**();

Clears all displays, all lights off.

**lix.write**(uint32_t **input** OR char* **input**);

    uint32_t: Clears the displays, and pushes in a multi-digit integer as in: 2016

    char*: Clears the displays, and pushes in a char array as in: "2016". This ignores any non-numeric chars in the string, allowing you to send "12:52:47 PM" and have the displays show "12 52 47" for a clock.

**lix.color_on**(byte **r**, byte **g**, byte **b**);

Sets the "on" color of the digits using RGB. This is the color of an active number in the display. (Default: 255,255,255)

**lix.color_off**(byte **r**, byte **g**, byte **b**);

Sets the "off" color of the digits using RGB. This is the color of all inactive numbers in the display. (Default: 0,0,0)

----------
# Contributing
Any advice or pull requests are welcome. :)

----------
# License and Credits
**Developed by Connor Nishijima (2016)**

**Released under the [GPLv3 license](http://www.gnu.org/licenses/gpl-3.0.en.html).**
