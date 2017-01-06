/*
Lixie.cpp - Library for driving Lixie displays!
Created by Connor Nishijima, October 26th 2016.
Released under the GPLv3 license.
*/

#include "Lixie.h"

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

static constexpr byte Lixie::addresses[];
float color_bal[3] = {1.00, 0.90, 0.65};

Lixie::Lixie(uint8_t pin, uint8_t nDigits):NumDigits(nDigits), NumLEDs(nDigits * 20){
	leds = new CRGB[NumLEDs];
	led_states = new byte[NumDigits * 3]; // 24 bits for 20 LED states
	colors = new CRGB[NumDigits];
	colors_off = new CRGB[NumDigits];
  
	build_controller(pin);
}

void Lixie::setBit(uint16_t pos, byte val){
	bitWrite(led_states[(pos/8)], pos % 8, val);
}

byte Lixie::getBit(uint16_t pos) const{
	return bitRead(led_states[(pos/8)], pos % 8);
}

void Lixie::begin() {
	FastLED.show();
	max_power(5,1000); // 5V, 1000mA
	for(byte i = 0; i < NumDigits; i++){
		colors[i] = CRGB(255,255,255);
		colors_off[i] = CRGB(0,0,0);
	}
	clear();
}

void Lixie::clear(bool show_change) {
	for (uint16_t i = 0; i < NumLEDs; i++) {
		setBit(i,0);
	}
	if(show_change == true){
		show();
	}
}

void Lixie::show(){
	for(uint16_t i = 0; i < NumLEDs; i++){
		if(getBit(i) == 1)
			leds[i] = colors[i/20];
		else
			leds[i] = colors_off[i/20];
	}
	FastLED.show();
}

// set all on color ------------------------------------
void Lixie::color(byte r, byte g, byte b){
	for(byte i = 0; i < NumDigits; i++){
		colors[i].r = r*color_bal[0];
		colors[i].g = g*color_bal[1];
		colors[i].b = b*color_bal[2];
	}
}

void Lixie::color(CRGB c){
	for(byte i = 0; i < NumDigits; i++){
		colors[i].r = c.r*color_bal[0];
		colors[i].g = c.g*color_bal[1];
		colors[i].b = c.b*color_bal[2];
	}
}

// set index on color ------------------------------------
void Lixie::color(byte r, byte g, byte b, byte index){
	colors[index].r = r*color_bal[0];
	colors[index].g = g*color_bal[1];
	colors[index].b = b*color_bal[2];
}

void Lixie::color(CRGB c, byte index){
	colors[index].r = c.r*color_bal[0];
	colors[index].g = c.g*color_bal[1];
	colors[index].b = c.b*color_bal[2];
}

// set all off color -------------------------------------
void Lixie::color_off(byte r, byte g, byte b){
	for(byte i = 0; i < NumDigits; i++){
		colors_off[i].r = r*color_bal[0];
		colors_off[i].g = g*color_bal[1];
		colors_off[i].b = b*color_bal[2];
	}
}

void Lixie::color_off(CRGB c){
	for(byte i = 0; i < NumDigits; i++){
		colors_off[i].r = c.r*color_bal[0];
		colors_off[i].g = c.g*color_bal[1];
		colors_off[i].b = c.b*color_bal[2];
	}
}

// set index color off -----------------------------------
void Lixie::color_off(byte r, byte g, byte b, byte index){
	colors_off[index].r = r*color_bal[0];
	colors_off[index].g = g*color_bal[1];
	colors_off[index].b = b*color_bal[2];
}

void Lixie::color_off(CRGB c, byte index){
	colors_off[index].r = c.r*color_bal[0];
	colors_off[index].g = c.g*color_bal[1];
	colors_off[index].b = c.b*color_bal[2];
}

byte Lixie::get_size(uint32_t input) const{
	byte places = 0;
	while(input > 0){
		places++;
		input = input / 10;
	}
	return places;
}

byte Lixie::char_to_number(char input) const{
	return byte(input-48); // convert ascii index to real number
}

bool Lixie::char_is_number(char input) const{
	if(input <= 57 && input >= 48) // if between ASCII '9' and '0'
		return true;
	else
		return false;
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


void Lixie::write(long input){
	write(uint32_t(input));
}

void Lixie::write(int8_t input){
	write(uint32_t(input));
}

void Lixie::write(byte input){
	write(uint32_t(input));
}

void Lixie::write(float input){
	write(uint32_t(input));
}

void Lixie::write(double input){
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
	if (NumDigits > 1) {
		for (uint16_t i = NumLEDs - 1; i >= 20; i--) {
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
	for (uint16_t i = 0; i < NumLEDs; i++) {
		Serial.print(getBit(i));
		if ((i + 1) % 20 == 0 && i != 0) {
			Serial.print('\t');
		}
	}
	Serial.println();
}

uint8_t Lixie::get_numdigits() const{
	return NumDigits;
}

bool Lixie::maxed_out(float input){
	if(input >= pow(10,NumDigits)){ // If input > number that can be displayed
		return false;
	}
	else{
		return true;
	}
}

void Lixie::build_controller(uint8_t DataPin){
	switch (DataPin){
		case 0:
			FastLED.addLeds<LED_TYPE, 0, COLOR_ORDER>(leds, NumLEDs);
			break;
		case 1:
			FastLED.addLeds<LED_TYPE, 1, COLOR_ORDER>(leds, NumLEDs);
			break;
		case 2:
			FastLED.addLeds<LED_TYPE, 2, COLOR_ORDER>(leds, NumLEDs);
			break;
		case 3:
			FastLED.addLeds<LED_TYPE, 3, COLOR_ORDER>(leds, NumLEDs);
			break;
		case 4:
			FastLED.addLeds<LED_TYPE, 4, COLOR_ORDER>(leds, NumLEDs);
			break;
		case 5:
			FastLED.addLeds<LED_TYPE, 5, COLOR_ORDER>(leds, NumLEDs);
			break;
		case 6:
			FastLED.addLeds<LED_TYPE, 6, COLOR_ORDER>(leds, NumLEDs);
			break;
		case 7:
			FastLED.addLeds<LED_TYPE, 7, COLOR_ORDER>(leds, NumLEDs);
			break;
		case 8:
			FastLED.addLeds<LED_TYPE, 8, COLOR_ORDER>(leds, NumLEDs);
			break;
		case 9:
			FastLED.addLeds<LED_TYPE, 9, COLOR_ORDER>(leds, NumLEDs);
			break;
		case 10:
			FastLED.addLeds<LED_TYPE, 10, COLOR_ORDER>(leds, NumLEDs);
			break;
		case 11:
			FastLED.addLeds<LED_TYPE, 11, COLOR_ORDER>(leds, NumLEDs);
			break;
		case 12:
			FastLED.addLeds<LED_TYPE, 12, COLOR_ORDER>(leds, NumLEDs);
			break;
		case 13:
			FastLED.addLeds<LED_TYPE, 13, COLOR_ORDER>(leds, NumLEDs);
			break;
	}
}

void Lixie::max_power(byte volts, uint16_t milliamps){
	FastLED.setMaxPowerInVoltsAndMilliamps(volts,milliamps); 
}

void Lixie::color_balance(float r_adj,float g_adj,float b_adj){
	color_bal[0] = r_adj;
	color_bal[1] = g_adj;
	color_bal[2] = b_adj;
}
