#include "Lixie.h" // Include Lixie Library
Lixie lix;         // Set class nickname for faster coding

uint32_t count = 0;

void setup() {
  lix.begin(); // Initialize LEDs
}

void loop() {
  lix.write(count); // Display count
  delay(100);
  count++;
}
