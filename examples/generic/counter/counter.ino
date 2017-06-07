#include "Lixie.h" // Include Lixie Library

#define DATA_PIN   5
#define NUM_LIXIES 4
Lixie lix(DATA_PIN, NUM_LIXIES);

uint32_t count = 0;

void setup() {
  lix.begin(); // Initialize LEDs
}

void loop() {
  lix.write(count); // Display count
  delay(100);
  count++;
}
