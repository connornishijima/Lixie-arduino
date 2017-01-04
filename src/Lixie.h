/*
Lixie.h - Library for driving Lixie displays!
Created by Connor Nishijima, October 26th 2016.
Released under the GPLv3 license.
*/

#ifndef lixie_h
#define lixie_h

#include "Arduino.h"
#include "FastLED.h"

class Lixie{
	public:
		Lixie(uint8_t pin, uint8_t nDigits);
		void begin();
		void clear(bool show_change = true);

		void write(uint32_t input);
		void write(char* input);
	
		void write(int input);
		void write(long input);
		void write(int8_t input);
		void write(byte input);
		void write(float input);
		void write(double input);
	
		void push_digit(byte number);
		void write_digit(byte input, byte index);

		void show();
		void print_binary();
		uint8_t get_numdigits() const;
		bool maxed_out(float input);

		void color_on(byte r, byte g, byte b);
		void color_on(CRGB c);

		void color_on(byte r, byte g, byte b, byte index);
		void color_on(CRGB c, byte index);

		void color_off(byte r, byte g, byte b);
		void color_off(CRGB c);

		void color_off(byte r, byte g, byte b, byte index);
		void color_off(CRGB c, byte index);
	private:
		const uint8_t NumDigits;
		const uint16_t NumLEDs;
		const uint8_t DataPin;
		void setBit(uint16_t pos, byte val);
		byte getBit(uint16_t pos) const;
		byte get_size(uint32_t input) const;
		byte char_to_number(char input) const;
		bool char_is_number(char input) const;
};

#endif
