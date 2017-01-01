/*
Lixie.h - Library for driving Lixie displays!
Created by Connor Nishijima, October 26th 2016.
Released under the GPLv3 license.
*/

#ifndef lixie_h
#define lixie_h

#include "Arduino.h"
#include "FastLED.h"

#define write_string(input) write_string_f(input, sizeof(input)-1);

class Lixie{
	public:
		Lixie();
		void begin();
		void clear(bool show_change = true);

		void write_int(float input);
		void push_digit(byte number);
		void write_digit(byte input, byte index);

		void write_string_f(char* input, byte len);
		void write_char(char input, byte index);

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
};

#endif
