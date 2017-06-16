# Getting Started with Lixie

I know you're excited, but we have a few things to cover to make sure you're ready to run Lixie displays!

## Protective Cover

Once you've unboxed your new Lixies, they'll have a paper cover on the top. This protects the glossy black acrylic cap from scratches, don't remove this quite yet.

## Microcontroller

Any AVR or ESP8266-derivative microcontroller can be used to control Lixie displays. There is a Lixie library available for the Arduino IDE:

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

### Are you running them in combined colors, or full white?

