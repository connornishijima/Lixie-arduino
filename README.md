![Lixie Library](https://i.imgur.com/MH40PLQ.jpg)

# Library for driving Lixie edge-lit displays!

This library allows for easy writing to Lixie Panels! It takes care of all the addressing and updating for you!

----------
# Contents
- [Installation](#installation)
- [Getting Started](#getting-started)
- [Basic Functions](#basic-functions)
- [Advanced Functions](#advanced-functions)
- [Debug Functions](#debug-functions)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License and credits](#license-and-credits)

----------
# Installation

***The Lixie library relies on [the FastLED library from Daniel Garcia](https://github.com/FastLED/FastLED), so make sure you have that installed as well!***

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

## \*Getting Started has moved to it's own guide [HERE](https://github.com/connornishijima/Lixie-arduino/blob/master/getting-started.md)!

----------
# Some Basic Functions

**lix.begin**();

Inititalizes the underlying FastLED library and sets up variables like the digit colors.

**lix.clear**();

Clears all displays, all lights off.

**lix.write**(int **input** OR char* **input**);

    int: Clears the displays, and pushes in a multi-digit integer as in: 2016

    char*: Clears the displays, and pushes in a char array as in: "2016". This ignores any non-numeric chars in the string, allowing you to send "12:52:47 PM" and have the displays show "12 52 47" for a clock.

**lix.color**(byte **r**, byte **g**, byte **b**);

Sets the "on" color of the digits using RGB. This is the color of an active number in the display. (Default: 255,255,255)

**lix.color_off**(byte **r**, byte **g**, byte **b**);

Sets the "off" color of the digits using RGB. This is the color of all inactive numbers in the display. (Default: 0,0,0)

**lix.show**();

Force the Lixies to update with current values and colors. Called automatically after every write call.

----------
# Some Advanced Functions

**lix.nixie_mode**(bool **enabled**);

The best damn Not-A-Nixie-Tube out there. Includes a well balanced amber digit, with neon aura emulation for a very convincing look.

**lix.brightness**(byte **bright**);

Sets the brightness of the displays, from 0 - 255. (Default: 255)

**lix.white_balance**(CRGB **c_adj**);

Sets a color calibration for the LEDs. Supports all FastLED color temperatures, and custom temperatures in the form CRGB(r, g, b). (Default: Tungsten100W / R: 255 G: 214 B: 170)

**lix.max_power**(int **volts**, int **milliamps**);

Sets a software power limit for all Lixies. Displaying white at full brightness, a Lixie will draw approximately 1200mA. (Default: 5V, 1000mA)

**lix.get_numdigits**();

Returns the number of Lixie displays currently in use, as an integer.

**lix.maxed_out**(int **input**);

Returns true if the input is too large to fit on the displays, false otherwise.

**lix.get_leds**();

Returns a pointer to the CRGB array that holds the color values sent to the displays. The library's *show()* function overwrites the values in this array, so call *FastLED.show()* to see any changes.

----------
# Debug Functions

The library also includes a few debugging functions if you're having issues with your displays. These functions require that the Serial library be initialized with "Serial.begin(speed)" before they will work.

**lix.print_binary**();

Prints the values of the led_states array, in binary, to the serial console.

**lix.print_current**();

Prints the current values on the display, in integers, to the serial console.

**lix.get_number**();

Returns the current number the display is showing in uint32_t form.

----------
# Troubleshooting

At the moment the library uses dynamic allocation for the LED arrays, which is not reported as allocated memory by the compiler. If your microcontroller is doing strange things, the first thing to check is that you have at least 70 bytes of dynamic memory available per Lixie digit.

If you've discovered a bug in the library, please create an issue on the Github repository so it can be fixed!

----------
# Contributing
Any advice or pull requests are welcome. :)

----------
# License and Credits
**Developed by Connor Nishijima (2016)**

**Pull Request Hall of Fame:**
- [David Madison](https://github.com/dmadison)

**Released under the [GPLv3 license](http://www.gnu.org/licenses/gpl-3.0.en.html).**
