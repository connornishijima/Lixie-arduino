#include "Lixie.h" // Include Lixie Library

#define DATA_PIN   13
#define NUM_LIXIES 4
Lixie lix(DATA_PIN, NUM_LIXIES);

int MAX_NUMBER = 500; // Number of times to write to the displays

void setup() {
  lix.begin(); // Initialize LEDs
  lix.color_on(255, 255, 0); // Yellow while testing

  int count = 0;
  unsigned long tStart = micros(); // Start time
  while (count < MAX_NUMBER) {
    lix.write(count);
    count++;
  }
  unsigned long tEnd = micros(); // End time
  
  int FPS = 1000000 / ((tEnd - tStart) / MAX_NUMBER); // Get FPS by how many microseconds all writes took
  lix.color_on(0, 255, 0); // Green when done
  lix.write(FPS);          // Display FPS
}

void loop() {
}
