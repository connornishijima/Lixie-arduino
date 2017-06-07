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

#define DATA_PIN   5
#define NUM_LIXIES 4
Lixie lix(DATA_PIN, NUM_LIXIES);

uint32_t MAX_NUMBER = 500; // Number of times to write to the displays

void setup() {
  Serial.begin(115200);  
  lix.begin(); // Initialize LEDs
}

void loop() {
  checkFPS();
  delay(3000);
}

void checkFPS(){
  Serial.println("Starting Lixie FPS test...");
  lix.color(255, 255, 0); // Yellow while testing

  unsigned long tStart = micros(); // Start time
  for(uint32_t count = 0; count < MAX_NUMBER; count++) {
    lix.write(count);
  }
  unsigned long tEnd = micros(); // End time
  
  int FPS = 1000000 / ((tEnd - tStart) / MAX_NUMBER); // Get FPS by how many microseconds all writes took
  lix.color(0, 255, 0); // Green when done
  
  Serial.print("Done!\nFPS is: ");
  Serial.println(FPS);
  
  // Blink FPS result
  for(byte i = 0; i < 4; i++){
    lix.clear();
    delay(250);
    lix.write(FPS);
    delay(250);
  }
}
