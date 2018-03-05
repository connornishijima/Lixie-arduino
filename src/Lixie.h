/*
Lixie.h - Library for driving Lixie displays!
Created by Connor Nishijima, October 26th 2016.
Released under the GPLv3 license.
*/

#ifndef lixie_h
#define lixie_h

#include "Arduino.h"

#ifdef ESP8266
	#define FASTLED_ESP8266_RAW_PIN_ORDER
	#define FASTLED_ALLOW_INTERRUPTS 0
	#define FASTLED_INTERRUPT_RETRY_COUNT 0
#endif

#include "FastLED.h"

#ifndef LED_TYPE
  #define LED_TYPE WS2812B
#endif

#ifndef COLOR_ORDER
  #define COLOR_ORDER GRB
#endif

class Lixie{
	public:
		Lixie(const uint8_t pin, uint8_t nDigits);
		void begin();
   
		void clear(bool show_change = true);
		void clear_digit(byte index, bool show_change = true);
		void show();
		
		void write(uint32_t input);
		void write(char* input);
		
		void write_flip(uint32_t input, uint16_t flip_time = 100, uint8_t flip_speed = 10);
		void write_fade(uint32_t input, uint16_t fade_time = 250);
		
		void sweep(CRGB col, byte speed = 15);
		void progress(float percent, CRGB col1, CRGB col2);
		void fill_fade_in(CRGB col, byte fade_speed = 20);
		void fill_fade_out(CRGB col, byte fade_speed = 20);
    
		void write_digit(byte input, byte index, bool show_change = true);
		void push_digit(byte number);
    
		void color(byte r, byte g, byte b);
		void color(CRGB c);
		void color(byte r, byte g, byte b, byte index);
		void color(CRGB c, byte index);

		void color_off(byte r, byte g, byte b);
		void color_off(CRGB c);
		void color_off(byte r, byte g, byte b, byte index);
		void color_off(CRGB c, byte index);
		
		void color_fade(CRGB col, uint16_t duration);
		void color_fade(CRGB col, uint16_t duration, byte index);
		
		void color_array_fade(CRGB *cols, uint16_t duration);
		void color_array_fade(CHSV *cols, uint16_t duration);
		
		void color_wipe(CRGB col1, CRGB col2);

		void nixie_mode(bool enabled, bool has_aura = true);
		void nixie_aura_intensity(byte val);
		
		void brightness(byte bright);
		void white_balance(CRGB c_adj);
		
		void rainbow(uint8_t r_hue, uint8_t r_sep);

		void max_power(byte volts, uint16_t milliamps);
	
		uint8_t get_numdigits() const;
		bool maxed_out(uint32_t input) const;

		CRGB* get_leds() const;

		// Debug Functions
		void print_binary() const;
		void print_current() const;
		uint32_t get_number();

	private:
		static constexpr byte Addresses[10] = {3, 4, 2, 0, 8, 6, 5, 7, 9, 1};
		const static uint8_t LEDsPerDigit = 20;
		const uint8_t NumDigits;
		const uint16_t NumLEDs;
		CRGB *leds;
		CRGB *colors;
		CRGB *colors_off;
		byte *led_states;
		byte *digit_brightness;
		CLEDController *controller;
		byte sweep_dir = 1;
		void setBit(uint16_t pos, byte val);
		byte getBit(uint16_t pos) const;
		byte get_size(uint32_t input) const;
		byte char_to_number(char input) const;
		bool char_is_number(char input) const;
		void build_controller(uint8_t DataPin);
		uint32_t current_number = 0;
		byte current_number_size = 0;
		byte current_number_arr[20];
		void store_current(uint32_t input);
		
		bool nixie = false;
		bool nixie_aura = false;
		byte nixie_aura_level = 0;
		byte nixie_aura_level_default = 8;
		CRGB nixie_col_ion = CRGB(255,70,7);
		CRGB nixie_col_aura = CRGB(0,100,255);
};

#endif
