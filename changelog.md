# LIXIE for ARDUINO CHANGE LOG:
(Most recent at top, please!)

Added Nixie Mode functionality! (7/17/17 - connornishijima)
-----------------------------------------------------------
People often buy Lixies because Nixies won't work for them, either functionally or financially. I've spent quite a bit of time dialing in just the right numbers to closely emulate the beautiful Nixie Tube glow. This is done with two details:

- Ionized color (amber)
- Aura color (pale blue)

These two colors in the right mix make for the awesome new *lix.nixie_mode(bool **enabled**)* function! Just enable or disable it to switch on and off the new Nixie theme. The aura emulation intensity can be turned up, down or off with *lix.nixie_aura_intensity(byte **val**);*. A usage example can be found in Examples -> advanced_features_tour.

Added "Advanced Features Tour" Example Sketch (7/17/17 - connornishijima)
-----------------------------------------------------------
This new demo has many of the special functions I've added but yet to share publically - including the new Nixie Mode!

Fixed lix.brightness() function (7/17/17 - connornishijima)
-----------------------------------------------------------
I found that FastLED.setBrightness() doesn't actually work with the CLEDController implementation that allows for multiple Lixie chains. Brightness settings are now manually enforced throughout the library!

Fixed pin assignment issue for ESP8266 (6/7/17 - connornishijima)
-----------------------------------------------------------
After this change, only pins 0,2,4 and 5 are usable for Lixie on ESP8266. Because the Arduino IDE has no way of differentiating between different version of the ESP8266 breakout boards or SMT modules, I have to go with this solution that covers all popular bases like the bare ESP-12, Wemos, HUZZAH, NodeMCU, and more. Before, the default pin of sketches was 13. This worked for Arduino, and for the Adafruit Huzzah I was using. But a WEMOS D1 Mini or NodeMCU doesn't have a pin assigned to "D13", leading users to a compile-time error that wasn't very descript at all. It should work now!

New default Lixie pin on any archetecture is Pin 5.
    
Added sweep() animation (1/14/17 - connornishijima)
-----------------------------------------------------------
This runs one sweep of a cylon-scanner style animation, call it again afterwards to keep it running.

    lix.sweep(CRGB col);

Added write_flip() function (1/14/17 - connornishijima)
-----------------------------------------------------------

This simulates a [split flap display](https://www.youtube.com/watch?v=jXM3odR4GoM) animation when writing a new number to the displays. It only accepts numbers, and only works for numbers displayable with your number of digits.

    lix.write_flip(uint32_t input, uint16_t flip_time = 100);

Added color_array_fade() function (1/14/17 - connornishijima)
-----------------------------------------------------------

This allows you to define a CRGB array[NumDigits] of colors for every digit to fade to at once.

    // Random color party!

    void loop() {
    	CRGB arr[4];
    	arr[0] = CHSV(random(0, 256), 255, 255);
    	arr[1] = CHSV(random(0, 256), 255, 255);
    	arr[2] = CHSV(random(0, 256), 255, 255);
    	arr[3] = CHSV(random(0, 256), 255, 255);

    	lix.write(count);
    	lix.color_array_fade(arr, 250);

    	count++;
    }

Added color_fade() functionality (1/13/17 - connornishijima)
-----------------------------------------------------------

Calling the following:

    lix.color_fade(CRGB(0,255,0), 1000);
    
will fade all digits (no matter their current color) to pure green over a period of 1 second! (Blocking method, watch out!) This has an accuracy of +/-5 milliseconds or so. You can also specify an index to only fade one digit:

    lix.color_fade(CRGB(0,255,0), 1000, 0);

Added check for FastLED minimum version (3.0.0) (1/8/17 - connornishijima)
-----------------------------------------------------------

Lixie.cpp:

    // Error if we can't find FastLED
    #if FASTLED_VERSION < 3000000
      #error "Lixie requires FastLED 3.0 or later: https://github.com/FastLED/FastLED"
    #endif

Fixed random colors issue (1/8/17 - connornishijima)
-----------------------------------------------------------

This change fixes random colors on boot, caused by showing FastLED before setting up colors:

New:

    void Lixie::begin() {
    	max_power(5,1000); // 5V, 1000mA
    	for(byte i = 0; i < NumDigits; i++){
    		colors[i] = CRGB(255,255,255);
    		colors_off[i] = CRGB(0,0,0);
    	}
    	color_balance(Tungsten100W);
    	clear();
    }
    
Old:

    void Lixie::begin() {
        controller->showLeds(bright);
    	max_power(5,1000); // 5V, 1000mA
    	for(byte i = 0; i < NumDigits; i++){
    		colors[i] = CRGB(255,255,255);
    		colors_off[i] = CRGB(0,0,0);
    	}
    	color_balance(Tungsten100W);
    	clear(false);
    }   

Added multi-Lixie examples (1/8/17 - dmadison)
-----------------------------------------------------------

I added two new examples that show off multi-group support: *counter_multi* and *scoreboard*.

Counter is the typical counter example, modified to work with two groups of Lixies. One group counts down from its max-displayable value, while the other counts up.

Scoreboard is a fun little program that requires two push buttons and at least two Lixies. Press one of the two buttons to see your score go up. When it reaches the 'winning number', it plays a short animation and then resets.

Added 'get_leds' function (1/8/17 - dmadison)
-----------------------------------------------------------

Added a simple function that returns a pointer to the LED array. This allows the user to edit the data sent to the LEDs directly if they wish.

Misc. Updates (1/8/17 - dmadison)
-----------------------------------------------------------

I tweaked the ESP8266-specific code Connor just added so it's mutually exclusive with the AVR (Arduino) architecture, which should prevent the program from trying to create duplicate FastLED controllers. There was also an 'else' missing in the AVR if statement ladder which was fixed.

At the same time I removed all of the brackets from the build_controller case ladder. No reason for them to be there, and it makes the code neater. The 'addresses' array was also capitalzied to signify it is constant.

I moved the LED defines (LED_TYPE and COLOR_ORDER) to the header and added guards to prevent redefines.

I also added a 'LEDsPerDigit' constant set to 20, to replace all of the scattered '20' values around the library. Should make the code more readable.

Lastly, I updated the README to include a 'Troubleshooting' section and a line about needing to leave a little dynamic memory for the Lixie arrays.

'Clear' function now uses memset (1/8/17 - dmadison)
-----------------------------------------------------------

I rewrote the 'clear' function to use 'memset' rather than 'setBit'. That means rather than going bit by bit finding the correct place in the led_state array and then performing a *single* bitWrite, it simply rewrites the entire chunk of memory at once. On my rig it's another 10% speed increase!

It may make sense to eventually call 'memset' for the times the library clears single digits, but in the current implementation the digits share bytes in the array so it's not possible.

Fixed ESP8266 compilation issue (1/7/17 - connornishijima)
-----------------------------------------------------------

After a string of very creative contributions by David Madison (dmadison below), the library would no longer compile on ESP8266 architectures. This was solved by only allowing the pins 0, 2, 4, 12, and 13 to control Lixie displays if compiled for an ESP8266 architecture. Now his awesome additions to this library work on both AVR and ESP8266!

Updated documentation (1/6/17 - dmadison)
-----------------------------------------------------------

Along with all of the other changes in this pull request, I updated the keywords.txt, README.md, and changelog.md (Hi!).

The keywords list was reorganized to match the header file, and I also added a few missing function keywords to the list. The README was updated with all of the new functions that have been added in the past few weeks and the 'uint32_t' datatypes were renamed as 'int' for comprehension purposes. I also changed the wording from "Additional Functions" to "*Advanced* Functions", and added another section for "Debug Functions".

Reorganized header function order (1/6/17 - dmadison)
-----------------------------------------------------------

This is an aesthetic change, but I moved around some of the functions in the header file to make a more logical order. Like functions are grouped, with more important functions higher in the list.

Changed 'maxed_out' function (1/6/17 - dmadison)
-----------------------------------------------------------

I was struggling with the inaccuracy of Arduino's floats when I was rewriting the 'write' function, so I thought it was best to get rid of them here as well. The function now takes an integer rather than a float and does the calculation using the 'get_size' function. I also flipped the output, so if the value *does* 'max out' the displays the function will return *true*, rather than false.


Added 'print_current' debug function (1/6/17 - dmadison)
-----------------------------------------------------------

While I was messing with rewriting the 'write' functions, I thought it would be useful to have a function that returned the current values of the displays, based on the led_states array. This function prints values as integers to the serial monitor in the same order they are inputted (i.e. opposite of the data order sent to the Lixies), and requires the Serial class be initialized.


Rewrote integer version of 'write' function (1/6/17 - dmadison)
-----------------------------------------------------------

Previously the integer version of the 'write' function used a temporary character array and sprintf. I rewrote the function to work with pure integer math, which improves performance by ~10% on my setup (ignoring push_digit). It also uses less memory.


Removed overloaded versions of the 'write' function (1/6/17 - dmadison)
-----------------------------------------------------------

Connor had added a few extra functions that overloaded the 'write' function to handle outside cases such as floats and smaller integers. This was a good idea, but in practice the overloaded methods just explicitly invoked a type conversion without doing any additional data transformation. The compiler actually catches these cases automatically, so they did nothing but add a bit of complexity to the library and confuse the compiler (e.g. "call of overloaded 'write(uint16_t&)' is ambiguous"). I removed the extra versions - smaller integers will still fit in the larger uint32_t datatype and floats are automatically type converted by the compiler.


Added input checks to digit write functions (1/6/17 - dmadison)
-----------------------------------------------------------

The lower-level 'write_digit' and 'push_digit' functions now have input sanitization. This should prevent accidentally reading and writing outside of allocated memory.


Cleaned up 'push_digit' function (1/6/17 - dmadison)
-----------------------------------------------------------

I added comments to make the function a bit less obscure, and I removed the redundant 'clear' call because the led_states for the first digit needs to be cleared regardless.


Fixed get_size function bug (1/6/17 - dmadison)
-----------------------------------------------------------

Previously the function would report that any input of '0' had a size of 0. Because the '0' digit still requires one display, the function will return 1 by default and is bounded to 9 rather than 0.


Added brightness control (1/5/17 - dmadison)
-----------------------------------------------------------

With the instance-specific FastLED controllers set up, now seemed like the perfect time to implement brightness control. Brightness control is isolated per-instance, so you can adjust the brightness of each Lixie group independently. You can adjust brightness by calling:

    brightness(255);

Where the parameter is an integer between 0 - 255.

Changed color balance functions to use FastLED (1/5/17 - dmadison)
-----------------------------------------------------------

I rewrote the color balance functionality so that it uses FastLED's color temperature implementation and CRGB objects. This has a number of benefits:

- It's faster: the calibration math is done during the FastLED show() call, and I believe it's also done between bit-bang clock cycles. This mean's adjusting calibration on the fly is quicker.
- It's nondestructive: colors in the color-setting arrays are not changed, which makes gradual color changes easier.
- It's cleaner: I like pretty code.

Created instance-specific FastLED controller (1/5/17 - dmadison)
-----------------------------------------------------------

Each instance now has its own FastLED controller, so things like brightness control, color balance, and show() calls can be isolated to specific displays rather than driven globally.


Added array bounds check to color setting by index (1/5/17 - dmadison)
-----------------------------------------------------------

The color setting functions that allow the user to set a color for a specific digit now check to see if the digit exists before writing to the array. This should prevent memory addressing problems.


Removed 'config.h' and added support for multiple instances (1/4/17)
-----------------------------------------------------------

I removed the config file and changed the class to configure the data pin and number of Lixies in the constructor. This should make initial setup easier, at the cost of dynamic data allocation at runtime.

This update also adds support for multiple Lixie groups! You can now drive multiple Lixie sets independently from the same microcontroller.


Moved color balance math from show() to color()/color_off()
-----------------------------------------------------------

Since we're dealing with floating points, I moved the color balance adjustment math to the color() & color_off() functions, instead of being in show(). This way, the floating point math is potentially only done once, when the color is set.

Shortened "color_on()" to "color()" (1/4/17)
--------------------------------------

To make things a bit quicker to write, I changed the color_on() function to simply: color(). The two color setting functions are now:

    color(r,g,b);
    color_off(r,g,b);

Added color_balance(r_adj, g_adj, b_adj) function: (1/4/17)
-----------------------------------------------------------

By default, CRGB(255,255,255) on a WS2812B produces a very blue-shifted, cold white. I don't think you can call it white if it's light blue! I've added these default color balance values:

    output_r = input_r * 1.00;
    output_g = input_g * 0.94;
    output_b = input_b * 0.59;
    
This equates to a very nice - not hot, not cold - balanced white. But let's say you wanted to change this default to a warm white. Just call **lix.color_balance(1.0, 0.7, 0.3)**.

These values need to be a floating point number between 0 and 1.

Added max_power(volts, milliamps) function: (1/4/17)
---------------------------------------------------

I discovered that FastLED has an option to limit global strip brightness based on the max wattage you'd like to consume. This is amazing! By default, the Lixie library will limit the current consumption to 5W, or 1A @ 5V. Under normal use, each digit will only consume a maximum ~120mA by having two LEDs lighting a digit with full white. You could run 8 digits without hitting the 1A cap.
		
  But some people might want to make the displays flash a color using all LEDs, as in the end of a countdown clock or IFTTT notifications. To protect your power supply (probably USB) from being overloaded, this default 5W cap is placed when lix.begin() is called. To change the maximum consumption allowed, enter your power supply specs into this new function:

    lix.max_power(volts, milliamps);

Be sure to leave some modest headroom for other things like the microcontroller doing all the work!

Started changelog, should have done this sooner! (1/4/17)
---------------------------------------------------------
