#include "Lixie.h" // Include Lixie Library

#define DATA_PIN   5
#define NUM_LIXIES 4
Lixie lix(DATA_PIN, NUM_LIXIES);

void setup() {
  lix.begin(); // Initialize LEDs
}

void loop() {
  byte reading = analogRead(A0) / 4; // Map 10-bit to 8-bit
  lix.color(255-reading,reading,0);
  lix.write((reading/255.0)*500); // Display reading as 0 - 5(.)00
  delay(100);
}
