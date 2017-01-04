#include "Lixie.h" // Include Lixie Library
Lixie lix;         // Set class nickname for faster coding

int MAX_NUMBER = 500; // Number of times to write to the displays

void setup() {
  lix.begin(); // Initialize LEDs
  lix.color(255, 255, 0); // Yellow while testing

  int count = 0;
  unsigned long tStart = micros(); // Start time
  while (count < MAX_NUMBER) {
    lix.write(count);
    count++;
  }
  unsigned long tEnd = micros(); // End time
  
  int FPS = 1000000 / ((tEnd - tStart) / MAX_NUMBER); // Get FPS by how many microseconds all writes took
  lix.color(0, 255, 0); // Green when done
  lix.write(FPS);          // Display FPS
}

void loop() {
}
