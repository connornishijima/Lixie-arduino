/*
 * Lixie FPS Test
 * by Connor Nishijima
 * 
 * This sketch will count from 0 to MAX_NUMBER,
 * writing each number to your Lixie Displays.
 * 
 * The time it takes to do this in microseconds
 * will be divided by MAX_NUMBER, giving the
 * microseconds each write took. Dividing
 * 1,000,000 by this value gives the FPS of your
 * microcontroller.
 * 
 * The higher MAX_NUMBER is, the more accurate
 * your results will be.
 */

#include "Lixie.h" // Include Lixie Library
Lixie lix;         // Set class nickname for faster coding

uint32_t MAX_NUMBER = 500; // Number of times to write to the displays

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Lixie FPS test...");
  
  lix.begin(); // Initialize LEDs
  lix.color(255, 255, 0); // Yellow while testing

  uint32_t count = 0;
  unsigned long tStart = micros(); // Start time
  while (count < MAX_NUMBER) {
    lix.write(count);
    count++;
  }
  unsigned long tEnd = micros(); // End time
  
  int FPS = 1000000 / ((tEnd - tStart) / MAX_NUMBER); // Get FPS by how many microseconds all writes took
  lix.color(0, 255, 0); // Green when done
  lix.write(FPS);          // Display FPS

  Serial.print("Done!\nFPS is: ");
  Serial.println(FPS);
}

void loop() {
}
