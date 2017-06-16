# Getting Started with Lixie

I know you're excited, but we have a few things to cover to make sure you're ready to run Lixie displays!

## Protective Cover

Once you've unboxed your new Lixies, they'll have a paper cover on the top. This protects the glossy black acrylic cap from scratches, don't remove this quite yet.

## Lixies don't do anything on their own.

The Lixie display is a bit more cost effective than competitors like the NixiePipe due to it being a "naive" display. It has no onboard controller you can program, you'll need an external microcontroller like the Arduino or ESP8266.

## Microcontroller

Any AVR/Arduino or ESP8266-derivative microcontroller can be used to control Lixie displays. There is a Lixie library available for the Arduino IDE:

- Click: Menu  >  Sketch  >  Include Library  >  Manage Libraries
- Search for "Lixie"
- Select version 1.3.0 (or latest) and click Install.

If you're working with Platform.io or any alternative IDE, you'll have to download the library manually:

- Visit https://github.com/connornishijima/Lixie-arduino/
- "Clone or Download"  >  "Download ZIP"
- Extract the .zip to (Sketchbook folder)/libraries/Lixie

When finished, an example folder structure for Windows should be:

    C:\Users\[USERNAME]\Documents\Arduino\libraries\Lixie\src\Lixie.cpp

## Wiring

Before using Lixies, I highly suggest reading through [Adafruit's "NeoPixel Uberguide"](https://learn.adafruit.com/adafruit-neopixel-uberguide/overview). It goes over many details of how the WS2812/B/S smart-RGB LED works. It's one of the most well-written guides out there. There are 20 of these LEDs in each Lixie digit, so you have to consider the following for your power requirements:

### Lixies are powered and controlled with 5V.

The 20 WS2812B LEDs onboard are nominally powered with 5 volts DC, and expect at least 3.5V (Power voltage\*0.7) on the DIN line to control the pixels. If you have a 3V3-logic microcontroller, you may or may not have to use [a level shifter](https://www.adafruit.com/product/1787). Personally, I have successfully run Lixies off of a 3.3V Adafruit Huzzah (ESP-12 controller) without needing a level shifter - even with the LEDs powered by the 5 volt "V+" line.

### Are you running them in combined colors, or full white?

Showing a number at full white means only 2 of the 20 LEDs are at full power. With a power consumption of 60mA per fully lit LED, that's 120mA at most per Lixie. If you were only displaying in red, green, or blue the consumption would be 40mA, a combination like cyan, magenta or yellow would take 80mA.

At this rate, a 6-digit clock at full brightness in white would consume 120mA\*6 = 720mA or 0.72A. This could be powered over USB with a standard AC phone charger.
