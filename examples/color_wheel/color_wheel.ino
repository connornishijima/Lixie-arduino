#include "Lixie.h" // Include Lixie Library
Lixie lix;         // Set class nickname for faster coding

uint16_t count = 0; // Use this number to count up
byte hue = 0;       // This is the HSV hue of the digits, and shifts a bit every 10 numbers

void setup() {
  lix.begin();                   // Initialize LEDs
  lix.color_on_hsv(hue,255,255); // Set initial HSV color (RED)
}

void loop() {
  lix.write_int(count);             // Write the count to the displays
  count++;                          // Increment count

  if(count % 10 == 0){              // If we've incremented ten times
    hue+=32;                        // Push hue 1/8th through the color wheel
    lix.color_on_hsv(hue,255,255);  // Set new color
  }
  
  delay(100);
}
