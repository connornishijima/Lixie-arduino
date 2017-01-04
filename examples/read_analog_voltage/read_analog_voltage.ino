#include "Lixie.h" // Include Lixie Library
Lixie lix;         // Set class nickname for faster coding

void setup() {
  lix.begin(); // Initialize LEDs
}

void loop() {
  byte reading = analogRead(A0) / 4; // Map 10-bit to 8-bit
  lix.color(255-reading,reading,0);
  lix.write((reading/255.0)*500); // Display reading as 0 - 5(.)00
  delay(100);
}
