/*
Lixie.cpp - Library for driving Lixie displays!
Created by Connor Nishijima, October 26th 2016.
Released under the GPLv3 license.
*/

#include "Lixie.h"
#include "config.h"

#define NUM_LEDS NUM_DIGITS * 24
CRGB leds[NUM_LEDS];

byte led_states[NUM_LEDS/8];
byte addresses[10] = {3, 4, 2, 0, 8, 6, 5, 7, 9, 1};
CRGB colors[NUM_DIGITS];
CRGB colors_off[NUM_DIGITS];

Lixie::Lixie(){}

void setBit(uint16_t pos, byte val){
	bitWrite(led_states[(pos/8)], pos % 8, val);
}

byte getBit(uint16_t pos){
	return bitRead(led_states[(pos/8)], pos % 8);
}

void Lixie::begin() {
	FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.show();
	for(byte i = 0; i < NUM_DIGITS; i++){
		colors[i] = CRGB(255,255,255);
		colors_off[i] = CRGB(0,0,0);
	}
	clear();
}

void Lixie::clear(bool show_change = true) {
	for (uint16_t i = 0; i < NUM_LEDS; i++) {
		setBit(i,0);
	}
	if(show_change == true){
		show();
	}
}

void Lixie::show(){
	for(uint16_t i = 0; i < NUM_LEDS; i++){
		if(getBit(i) == 1){
			byte r = colors[i/20].r;
			byte g = colors[i/20].g;
			byte b = colors[i/20].b;
			leds[i] = CRGB(r,g,b);
		}
		else{
			byte r = colors_off[i/20].r;
			byte g = colors_off[i/20].g;
			byte b = colors_off[i/20].b;
			leds[i] = CRGB(r,g,b);
		}
	}
	FastLED.show();
}

// set all on color ------------------------------------
void Lixie::color_on(byte r, byte g, byte b){
	for(byte i = 0; i < NUM_DIGITS; i++){
		colors[i].r = r;
		colors[i].g = g;
		colors[i].b = b;
	}
}

void Lixie::color_on(CRGB c){
	for(byte i = 0; i < NUM_DIGITS; i++){
		colors[i] = c;
	}
}

// set index on color ------------------------------------
void Lixie::color_on(byte r, byte g, byte b, byte index){
	colors[index].r = r;
	colors[index].g = g;
	colors[index].b = b;
}

void Lixie::color_on(CRGB c, byte index){
	colors[index] = c;
}

// set all off color -------------------------------------
void Lixie::color_off(byte r, byte g, byte b){
	for(byte i = 0; i < NUM_DIGITS; i++){
		colors_off[i].r = r;
		colors_off[i].g = g;
		colors_off[i].b = b;
	}
}

void Lixie::color_off(CRGB c){
	for(byte i = 0; i < NUM_DIGITS; i++){
		colors_off[i] = c;
	}
}

// set index color off -----------------------------------
void Lixie::color_off(byte r, byte g, byte b, byte index){
	colors_off[index].r = r;
	colors_off[index].g = g;
	colors_off[index].b = b;
}

void Lixie::color_off(CRGB c, byte index){
	colors_off[index] = c;
}

byte get_size(uint32_t input){
	byte places = 0;
	while(input > 0){
		places++;
		input = input / 10;
	}
	return places;
}

byte char_to_number(char input){
	return byte(input-48); // convert ascii index to real number
}

bool char_is_number(char input){
	if(input == '0'){
		return true;
	}
	else if(input == '1'){
		return true;
	}
	else if(input == '2'){
		return true;
	}
	else if(input == '3'){
		return true;
	}
	else if(input == '4'){
		return true;
	}
	else if(input == '5'){
		return true;
	}
	else if(input == '6'){
		return true;
	}
	else if(input == '7'){
		return true;
	}
	else if(input == '8'){
		return true;
	}
	else if(input == '9'){
		return true;
	}
	else{
		return false;
	}
}

void Lixie::write(char* input){
	char temp[20] = "";
	byte index = 0;
	for(byte i = 0; i < 20; i++){
		if(char_is_number(input[i])){
			temp[index] = input[i];
			index++;
		}
	}
	uint32_t output = atol(temp);
	write(output);
}

void Lixie::write(uint32_t input){
	char t[20] = "";
	sprintf(t,"%lu",input);
	clear(false);
	if(input != 0){
		for(byte i = 0; i < get_size(input); i++){
			push_digit(char_to_number(t[i]));
		}
	}
	else{
		push_digit(0);
	}
	show();
}

void Lixie::write(int input){
	write(uint32_t(input));
}

void Lixie::write_digit(byte input, byte index){
	uint16_t start = (index*20);

	for(uint16_t i = start; i < start+20; i++){
		setBit(i,0);
	}

	uint16_t L1 = start+addresses[input];
	uint16_t L2 = start+addresses[input] + 10;

	setBit(L1,1);
	setBit(L2,1);

	show();
}

void Lixie::push_digit(byte number) {
	if (NUM_DIGITS > 1) {
		for (uint16_t i = NUM_LEDS - 1; i >= 20; i--) {
			setBit(i,getBit(i - 20));
		}
		for (uint16_t i = 0; i < 20; i++) {
			setBit(i,0);
		}
	}
	else {
		clear(false);
	}

	uint16_t L1 = addresses[number];
	uint16_t L2 = addresses[number] + 10;

	setBit(L1,1);
	setBit(L2,1);
}

void Lixie::print_binary() {
	for (uint16_t i = 0; i < NUM_LEDS; i++) {
		Serial.print(getBit(i));
		if ((i + 1) % 20 == 0 && i != 0) {
			Serial.print('\t');
		}
	}
	Serial.println();
}

const uint8_t Lixie::get_numdigits(){
	return NUM_DIGITS;
}

bool Lixie::maxed_out(float input){
	if(input >= pow(10,NUM_DIGITS)){ // If input > number that can be displayed
		return false;
	}
	else{
		return true;
	}
}
