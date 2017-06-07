/* 
 * Lixie Scoreboard
 * by David Madison
 * 
 * Parts needed:
 * - 2x push buttons
 * - 2x Lixie displays
 * 
 * This sketch keeps track of two player's scores between two Lixie
 * displays. You'll need to attach two buttons to two Arduino inputs,
 * and two Lixies to separate output pins. Pressing either button will
 * update the respective display. When either display hits the winning
 * number, it's game over!
 * 
 * Created: January 8, 2017
 */

#include "Lixie.h" // Include Lixie Library

// Set i/o pins and number of Lixies
const int dataPin1 = 4;
const int dataPin2 = 5;
const int inputPin1 = 2;
const int inputPin2 = 3;

const int numLixies = 1;

// Set Lixie colors
const CRGB Player1Color = CRGB::Blue;
const CRGB Player2Color = CRGB::Red;

const CRGB UpdateColor = CRGB::Yellow;
const CRGB WinColor = CRGB::White;
const CRGB WinColorOff = CRGB::Green;

// Create Lixie instances
Lixie player1Lix(dataPin1, numLixies);
Lixie player2Lix(dataPin2, numLixies);

// Keep track of player scores
int player1Score;
int player2Score;

// Set the number requires to win!
const int winningNumber = 10;

void setup() {
	// Sets the input pins as inputs
	pinMode(inputPin1, INPUT);
	pinMode(inputPin2, INPUT);

	// Sets internal pull-up resistors
	digitalWrite(inputPin1, HIGH);
	digitalWrite(inputPin2, HIGH);

	// Initialize Lixie displays
	player1Lix.begin();
	player2Lix.begin();

	// Start the game!
	gameStart();
}

void loop(){
	// Amount of time, in milliseconds, to ignore input after button press
	const int debounce = 300;
  
	// If button 1 is pressed
	if(digitalRead(inputPin1) == HIGH){
		player1Score++; // Increase player 1's score

		// If player 1 wins...
		if(player1Score >= winningNumber){
			// Celebration colors!
			player1Lix.color(WinColor);
			player1Lix.color_off(WinColorOff);

			// Light show, five times!
			for(int i = 0; i < 5; i++){
				for(int j = 0; j < winningNumber; j++){
					player1Lix.write(j);
					delay(100);
				}
			}
			gameStart(); // Let's go again!
		}
		// If player 1 doesn't win...
		else{
			// Update the score
			player1Lix.write(player1Score);
			delay(debounce);
		}
	}
	// If button 2 is pressed
	else if(digitalRead(inputPin2) == HIGH){
		player2Score++; // Increase player 2's score

		// If player 2 wins...
		if(player2Score >= winningNumber){
			// Celebration colors!
			player2Lix.color(WinColor);
			player2Lix.color_off(WinColorOff);

			// Light show, five times!
			for(int i = 0; i < 5; i++){
				for(int j = 0; j < winningNumber; j++){
					player2Lix.write(j);
					delay(100);
				}
			}
			gameStart(); // Let's go again!
		}
		// If player 2 doesn't win...
		else{
			// Update the score
			player2Lix.write(player2Score);
			delay(debounce);
		}
	}
}

void gameStart(){
	// Small light show for the start of the game!

	// Clear both displays
	player1Lix.clear();
	player2Lix.clear();

	// Reset the colors to player's favorites
	resetColors();

	// Scroll through all scores up to the winning number
	for(int i = 0; i<winningNumber; i++){
		player1Lix.write(i);
		player2Lix.write(i);
		delay(150);
	}

	// Set winning number color
	player1Lix.color(UpdateColor);
	player2Lix.color(UpdateColor);

	// Write the winning number
	player1Lix.write(winningNumber);
	player2Lix.write(winningNumber);
	delay(1000);

	// Flash the winnning number
	for(int i = 0; i<3; i++){
		player1Lix.write(winningNumber);
		player2Lix.write(winningNumber);
		delay(100);
		player1Lix.clear();
		player2Lix.clear();
		delay(100);
	}
	delay(500);
  
	// Reset colors and scores!
	resetColors();
	resetScores();
}

void resetColors(){
	// Reset 'on' colors to players's favorites
	player1Lix.color(Player1Color);
	player2Lix.color(Player2Color);

	// Reset 'off' colors to nothing
	player1Lix.color_off(CRGB::Black);
	player2Lix.color_off(CRGB::Black);
}

void resetScores(){
	// Set both scores to 0
	player1Score = 0;
	player2Score = 0;

	// Send scores to the Lixies
	player1Lix.write(player1Score);
	player2Lix.write(player2Score);
}

