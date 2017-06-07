/*
 * Lixie Multi-Counter
 * by David Madison
 * 
 * Parts needed:
 * - 2x Lixie displays
 * 
 * Counts up on one display and down on the other.
 * 
 * Created: January 8, 2017
 */
 
#include "Lixie.h" // Include Lixie Library

const int dataPin1 = 4; // Data pin for Lixie group #1
const int dataPin2 = 5; // Data pin for Lixie group #2
const int numLixies = 1; // # of Lixies in each group (1-9)

int countUp;
int countDown;
int maxCount;
byte hue = 0;

Lixie lix1(dataPin1, numLixies);
Lixie lix2(dataPin2, numLixies);

void setup() {
	// Initialize Lixies
	lix1.begin();
	lix2.begin();

	// Calculate max value that can be displayed
	maxCount = 9;
	for(int i = 1; i < numLixies; i++){
		maxCount = maxCount * 10;
		maxCount = maxCount + 9;
	}

	countUp = 0;
	countDown = maxCount;
}

void loop() {
	// Set colors with hue, saturation, value (HSV)
	lix1.color(CHSV(hue, 255, 255));
	lix2.color(CHSV(hue + 64, 255, 255)); // offset color hue by 1/4

	lix1.write(countUp);
	countUp++;
	delay(100);

	lix2.write(countDown);
	countDown--;
	delay(100);

	if(countUp > maxCount){
		countUp = 0;
	}

	if(countDown < 0){
		countDown = maxCount;
	}
  
	hue++;
}

