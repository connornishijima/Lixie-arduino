/*
  Lixie.cpp - Library for driving Lixie displays!
  Created by Connor Nishijima, October 26th 2016.
  Released under the GPLv3 license.
*/

#include "Lixie.h"

// Check if FastLED version is sufficient
#if FASTLED_VERSION < 3000000
  #error "Lixie requires FastLED 3.0.0 or later: https://github.com/FastLED/FastLED"
#endif

constexpr byte Lixie::Addresses[];
float bright = 1.0;

Lixie::Lixie(const uint8_t pin, uint8_t nDigits):NumDigits(nDigits), NumLEDs(nDigits * LEDsPerDigit){
	leds = new CRGB[NumLEDs];
	led_states = new byte[NumDigits * 3]; // 24 bits for 20 LED states
	colors = new CRGB[NumDigits];
	colors_off = new CRGB[NumDigits];
	digit_brightness = new byte[NumDigits];
	
	build_controller(pin);
}

CRGB enforce_brightness(CRGB input){
	/*
	 *	FastLED.setBrightness() does not work with CLEDControllers, so brightness
	 *  unfortunately has to be manually enforced throughout the library. This takes
	 *  a CRGB in, reduces the brightness by "bright" variable (0.0-1.0) and returns it.
	 */
	
	input.r = input.r*bright;
	input.g = input.g*bright;
	input.b = input.b*bright;
	
	return input;
}

void Lixie::setBit(uint16_t pos, byte val){
	bitWrite(led_states[(pos/8)], pos % 8, val);
}

byte Lixie::getBit(uint16_t pos) const{
	return bitRead(led_states[(pos/8)], pos % 8);
}

void Lixie::begin() {
	max_power(5,1000); // 5V, 1000mA DEFAULT
	for(byte i = 0; i < NumDigits; i++){
		colors[i] = CRGB(255,255,255);
		colors_off[i] = CRGB(0,0,0);
		digit_brightness[i] = 255;
	}
	white_balance(Tungsten100W);
	store_current(0);
	clear();
}

void Lixie::clear(bool show_change) {
	memset(led_states, 0, NumDigits * 3);
	if(show_change == true){
		show();
	}
}

void Lixie::show(){
	for(uint16_t i = 0; i < NumLEDs; i++){
#ifdef ESP8266
		yield();
#endif
		CRGB col;
		CRGB col_off;
		if(nixie == true){
			col = nixie_col_ion;
			col_off = nixie_col_aura;
			
			col_off.r *= (nixie_aura_level/255.0);
			col_off.g *= (nixie_aura_level/255.0);
			col_off.b *= (nixie_aura_level/255.0);
		}
		else{
			col = colors[i/LEDsPerDigit];
			col_off = colors_off[i/LEDsPerDigit];
		}

		if(getBit(i) == 1){			
			leds[i].r = col.r * digit_brightness[i/LEDsPerDigit]/255.0;
			leds[i].g = col.g * digit_brightness[i/LEDsPerDigit]/255.0;
			leds[i].b = col.b * digit_brightness[i/LEDsPerDigit]/255.0;
		}
		else{
			if(nixie == false){
				leds[i].r = col_off.r * digit_brightness[i/LEDsPerDigit]/255.0;
				leds[i].g = col_off.g * digit_brightness[i/LEDsPerDigit]/255.0;
				leds[i].b = col_off.b * digit_brightness[i/LEDsPerDigit]/255.0;
			}
			else if(nixie == true){
				byte index = i/LEDsPerDigit;
				if(index < current_number_size && nixie_aura == true){
					leds[i].r = col_off.r * digit_brightness[i/LEDsPerDigit]/255.0;
					leds[i].g = col_off.g * digit_brightness[i/LEDsPerDigit]/255.0;
					leds[i].b = col_off.b * digit_brightness[i/LEDsPerDigit]/255.0;
				}
				else{
					leds[i].r = 0;
					leds[i].g = 0;
					leds[i].b = 0;
				}
			}
		}
		
		//Enforce brightness
		leds[i] = enforce_brightness(leds[i]);
#ifdef ESP8266
		yield();
#endif
	}
	
	controller->showLeds();
}

// set all on color ------------------------------------
void Lixie::color(byte r, byte g, byte b){
	for(byte i = 0; i < NumDigits; i++){
		colors[i].r = r;
		colors[i].g = g;
		colors[i].b = b;
	}
}

void Lixie::color(CRGB c){
	for(byte i = 0; i < NumDigits; i++){
		colors[i] = c;
	}
}

// set index on color ------------------------------------
void Lixie::color(byte r, byte g, byte b, byte index){
	if(index < NumDigits){
		colors[index].r = r;
		colors[index].g = g;
		colors[index].b = b;
	}
}

void Lixie::color(CRGB c, byte index){
	if(index < NumDigits){
		colors[index] = c;
	}
}

// set all off color -------------------------------------
void Lixie::color_off(byte r, byte g, byte b){
	for(byte i = 0; i < NumDigits; i++){
		colors_off[i].r = r;
		colors_off[i].g = g;
		colors_off[i].b = b;
	}
}

void Lixie::color_off(CRGB c){
	for(byte i = 0; i < NumDigits; i++){
		colors_off[i] = c;
	}
}

// set index color off -----------------------------------
void Lixie::color_off(byte r, byte g, byte b, byte index){
	if(index < NumDigits){
		colors_off[index].r = r;
		colors_off[index].g = g;
		colors_off[index].b = b;
	}
}

void Lixie::color_off(CRGB c, byte index){
	if(index < NumDigits){
		colors_off[index] = c;
	}
}

void Lixie::fill_fade_in(CRGB col, byte fade_speed){
	col = enforce_brightness(col);
	for(float fade = 0; fade < 1; fade += 0.05){
		for(uint16_t i = 0; i < NumLEDs; i++){
			leds[i].r = col.r*fade*bright;
			leds[i].g = col.g*fade*bright;
			leds[i].b = col.b*fade*bright;
		}
		
		FastLED.show();
		delay(fade_speed);
	}
}

void Lixie::fill_fade_out(CRGB col, byte fade_speed){
	col = enforce_brightness(col);
	for(float fade = 1; fade > 0; fade -= 0.05){
		for(uint16_t i = 0; i < NumLEDs; i++){
			leds[i].r = col.r*fade*bright;
			leds[i].g = col.g*fade*bright;
			leds[i].b = col.b*fade*bright;
		}
		
		FastLED.show();
		delay(10);
	}
}

void Lixie::color_fade(CRGB col, uint16_t duration){
	
	CRGB colors_temp[NumDigits];
	for(byte i = 0; i < NumDigits; i++){
		colors_temp[i] = colors[i];
	}

	byte push = 50;
	
	if(duration < 100){
		duration = 100;
	}
	else if(duration < 250){
		push = 100;
	}

	uint16_t del = push*(duration/1000.0);
	for (float progress = 0; progress < 1; progress += (push/1000.0)){
		long tStart = millis();
		for(byte i = 0; i < NumDigits; i++){
			color(CRGB(
				colors_temp[i].r * (1 - progress) + col.r * progress,
				colors_temp[i].g * (1 - progress) + col.g * progress,
				colors_temp[i].b * (1 - progress) + col.b * progress
			),i);
		}
		show();
		long tEnd = millis();
		delay(del - (tEnd - tStart));
	}
	color(col);
	show();
}

void Lixie::color_fade(CRGB col, uint16_t duration, byte index){
	CRGB color_temp = colors[index];
	byte push = 50;
	
	if(duration < 100){
		duration = 100;
	}
	else if(duration < 250){
		push = 100;
	}
	
	uint16_t del = push*(duration/1000.0);
	for (float progress = 0; progress < 1; progress += (push/1000.0)){
		long tStart = millis();
			color(CRGB(
				color_temp.r * (1 - progress) + col.r * progress,
				color_temp.g * (1 - progress) + col.g * progress,
				color_temp.b * (1 - progress) + col.b * progress
			),index);
		
		show();
		long tEnd = millis();
		delay(del - (tEnd - tStart));
	}
	color(col,index);
	show();
}

void Lixie::color_array_fade(CRGB *cols, uint16_t duration){
	CRGB colors_temp[NumDigits];
	for(byte i = 0; i < NumDigits; i++){
		colors_temp[i] = colors[i];
	}

	byte push = 50;
	
	if(duration < 100){
		duration = 100;
	}
	else if(duration < 250){
		push = 100;
	}

	uint16_t del = push*(duration/1000.0);
	for (float progress = 0; progress < 1; progress += (push/1000.0)){
		long tStart = millis();
		for(byte i = 0; i < NumDigits; i++){
			color(CRGB(
				colors_temp[i].r * (1 - progress) + cols[i].r * progress,
				colors_temp[i].g * (1 - progress) + cols[i].g * progress,
				colors_temp[i].b * (1 - progress) + cols[i].b * progress
			),i);
		}
		show();
		long tEnd = millis();
		delay(del - (tEnd - tStart));
	}
	show();
}

void Lixie::color_array_fade(CHSV *cols, uint16_t duration){
	CRGB tempArray[NumDigits];
	for(byte i = 0; i < NumDigits; i++){
		tempArray[i] = CRGB(cols[i]);
	}
	color_array_fade(tempArray,duration);
}

void Lixie::color_wipe(CRGB col1, CRGB col2){
  float push = 1 /float(NumDigits);
  for(byte i = 0; i < NumDigits; i++){
    color(
      CRGB(
        (col1.r*(push*i)) + (col2.r*(push*(NumDigits-i))),
        (col1.g*(push*i)) + (col2.g*(push*(NumDigits-i))),
        (col1.b*(push*i)) + (col2.b*(push*(NumDigits-i)))
      ),
      i
    );
  }
}

void Lixie::nixie_mode(bool enabled, bool has_aura){
    nixie = enabled;
	nixie_aura_level = nixie_aura_level_default;
	nixie_aura = has_aura;
}

void Lixie::nixie_aura_intensity(byte val){
	nixie_aura_level = val;
}

byte Lixie::get_size(uint32_t input) const{
	byte places = 1;
	while(input > 9){
		places++;
		input /= 10;
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
	store_current(input);
	uint32_t nPlace = 1;

	clear(false);

	// Powers of 10 while avoiding floating point math
	for(uint8_t i = 1; i < get_size(input); i++){
		nPlace *= 10;
	}

	for(nPlace; nPlace > 0; nPlace /= 10){
		push_digit(input / nPlace);
		if(nPlace > 1) input = (input % nPlace);
	}

	show();
}

void print_array(byte *arr, byte sz){
	for(byte i = 0; i < sz; i++){
		Serial.print(arr[i]);
		Serial.print("\t");
	}
	Serial.println();
}

byte rng() {
  static unsigned int y = 0;
  y += micros(); // seeded with changing number
  y ^= y << 2; y ^= y >> 7; y ^= y << 7;
  return (y);
}

void Lixie::write_flip(uint32_t input, uint16_t flip_time, uint8_t flip_speed){
    byte new_number_arr[20];
	char buf [20];
	
	for(byte i = 0; i < 20; i++){
		new_number_arr[i] = 255;
		buf[i] = '0';
	}
	
	byte places = get_size(input);

	sprintf (buf, "%lu", input); // int to char array

	for (byte i = 0; i < places; i++) {
		byte d = char_to_number(buf[i]);
		if(d <= 9){
			new_number_arr[i] = d; // char to int
		}
	}
	
	byte changed[20];
	
	for(byte i = 0; i < 20; i++){
		changed[i] = 0;
		if(new_number_arr[i] != current_number_arr[i]){
			changed[i] = 1;
		}
	}
	
	store_current(input);
	
	byte changed_shift[NumDigits];
	byte new_shift[NumDigits];
	
	byte shift = NumDigits-places;
	for(byte i = 0; i < NumDigits; i++){
		if(i >= shift){
			changed_shift[i] = changed[i-shift];
			new_shift[i] = new_number_arr[i-shift];
		}
		else{
			changed_shift[i] = 0;
			new_shift[i] = 0;
		}
	}

	byte temp_digit = random(0,10);
	uint32_t flip_end = millis()+flip_time;
	while(millis() < flip_end){
		for(byte i = 0; i < NumDigits; i++){
			byte lix_index = NumDigits-1-i;
			if(changed_shift[i] == 1){
				write_digit(temp_digit,lix_index,false);
			}
			if(i < shift){
				clear_digit(lix_index,false);
			}
			temp_digit++;
			if(temp_digit >= 10){
				temp_digit = 0;
			}
		}
		show();
		delay(flip_speed);
	}
	for(byte i = 0; i < NumDigits; i++){
		byte lix_index = NumDigits-1-i;
		if(changed_shift[i] == 1){
			write_digit(new_shift[i],lix_index,false);
		}
	}
	show();
}

void Lixie::write_fade(uint32_t input, uint16_t fade_time){
    byte new_number_arr[20];
	char buf [20];
	
	for(byte i = 0; i < 20; i++){
		new_number_arr[i] = 255;
		buf[i] = '0';
	}
	
	byte places = get_size(input);

	sprintf (buf, "%lu", input); // int to char array

	for (byte i = 0; i < places; i++) {
		byte d = char_to_number(buf[i]);
		if(d <= 9){
			new_number_arr[i] = d; // char to int
		}
	}
	
	byte changed[20];
	
	for(byte i = 0; i < 20; i++){
		changed[i] = 0;
		if(new_number_arr[i] != current_number_arr[i]){
			changed[i] = 1;
		}
	}
	
	store_current(input);
	
	byte changed_shift[NumDigits];
	byte new_shift[NumDigits];
	
	byte shift = NumDigits-places;
	for(byte i = 0; i < NumDigits; i++){
		if(i >= shift){
			changed_shift[i] = changed[i-shift];
			new_shift[i] = new_number_arr[i-shift];
		}
		else{
			changed_shift[i] = 1;
			new_shift[i] = 0;
		}
	}
	
	byte div = 20;
	
	byte step = fade_time/(255/div)/4;
	
	show();
	
	for(int f = 255; f > 0; f-=div){
		for(byte i = 0; i < NumDigits; i++){
			byte lix_index = NumDigits-1-i;
			if(changed_shift[i] == 1){
				//if(i >= shift){
					digit_brightness[lix_index] = f;
				//}
			}
		}
		show();
		delay(step);
	}
	write(input);
	for(int f = 0; f < 255; f+=div){		
		for(byte i = 0; i < NumDigits; i++){
			byte lix_index = NumDigits-1-i;
			if(changed_shift[i] == 1){
				//if(i >= shift){
					digit_brightness[lix_index] = f;
				//}
			}
		}
		show();
		delay(step);
	}
	write(input);
	for(byte i = 0; i < NumDigits; i++){
		digit_brightness[i] = 255;
	}
	show();
}

void Lixie::write_digit(byte input, byte index, bool show_change){
	if(index >= NumDigits) return;

	while(input > 9){
		input-=9;
	}
	
	uint16_t start = (index*LEDsPerDigit);

	for(uint16_t i = start; i < start+LEDsPerDigit; i++){
		setBit(i,0);
	}

	uint16_t L1 = start+Addresses[input];
	uint16_t L2 = start+Addresses[input] + 10;

	setBit(L1,1);
	setBit(L2,1);

	if(show_change == true){
		show();
	}
}

void Lixie::clear_digit(byte index, bool show_change){
	if(index >= NumDigits) return;
  
	uint16_t start = (index*LEDsPerDigit);

	for(uint16_t i = start; i < start+LEDsPerDigit; i++){
		setBit(i,0);
	}
	
	if(show_change == true){
		show();
	}
}

void Lixie::push_digit(byte number) {
	if(number > 9) return;

	// If multiple digits, move all LED states forward one
	if (NumDigits > 1) {
		for (uint16_t i = NumLEDs - 1; i >= LEDsPerDigit; i--) {
			setBit(i,getBit(i - LEDsPerDigit));
		}
	}
 
	// Clear the LED states for the first digit
	for (uint16_t i = 0; i < LEDsPerDigit; i++) {
		setBit(i,0);
	}

	uint16_t L1 = Addresses[number];
	uint16_t L2 = Addresses[number] + 10;

	setBit(L1,1);
	setBit(L2,1);
}

void Lixie::store_current(uint32_t cur){
	char buf [20];
	
	for(byte i = 0; i < 20; i++){
		current_number_arr[i] = 255;
		buf[i] = '0';
	}
	
	byte places = get_size(cur);

	sprintf (buf, "%lu", cur); // int to char array

	for (byte i = 0; i < places; i++) {
		byte d = char_to_number(buf[i]);
		if(d <= 9){
			current_number_arr[i] = d; // char to int
		}
	}
	current_number = cur;
	current_number_size = places;
}

void Lixie::sweep(CRGB col, byte speed){
	col = enforce_brightness(col);
	
	for(byte s = 0; s < 2; s++){ // BACK AND FORTH
		if(sweep_dir == 0){
			sweep_dir = !sweep_dir;
			for(byte d = 0; d < NumDigits; d++){
				for(byte i = 0; i < 7; i++){
					for(uint16_t l = 0; l<NumLEDs; l++){
						leds[l] = CRGB(col.r*0.2,col.g*0.2,col.b*0.2);
					}
					leds[(d*20)+i+6] = col;
					leds[(d*20)+i+7] = col;
					leds[(d*20)+i+8] = col;
					FastLED.show();
					delay(speed);
				}
			}
		}
		else if(sweep_dir == 1){
			sweep_dir = !sweep_dir;
			for(int d = NumDigits-1; d >= 0; d--){
				for(int i = 6; i >= 0; i--){
					for(uint16_t l = 0; l<NumLEDs; l++){
						leds[l] = CRGB(col.r*0.2,col.g*0.2,col.b*0.2);
					}
					leds[(d*20)+i+6] = col;
					leds[(d*20)+i+7] = col;
					leds[(d*20)+i+8] = col;
					FastLED.show();
					delay(speed);
				}
			}
		}
	}
}

void Lixie::progress(float percent, CRGB col1, CRGB col2){	
	uint16_t index = NumLEDs-(NumLEDs*(percent/100.0));
	for(uint16_t i = 0; i < NumLEDs; i++){
		if(i <= index){
			leds[i] = col2;
		}
		else{
			leds[i] = col1;
		}
		leds[i] = enforce_brightness(leds[i]);
#ifdef ESP8266
		yield();
#endif
	}
	FastLED.show();
#ifdef ESP8266
	yield();
#endif
}

void Lixie::print_binary() const{
	for (uint16_t i = 0; i < NumLEDs; i++) {
		Serial.print(getBit(i));
		if ((i + 1) % LEDsPerDigit == 0 && i != 0) {
			Serial.print('\t');
		}
	}
	Serial.println();
}

void Lixie::print_current() const{
	// Reversed map of the standard addresses
	static const uint8_t readdress[10] = {3, 9, 2, 0, 1, 6, 5, 7, 4, 8,};

	for(int8_t i = NumDigits - 1; i >= 0; i--){
		for(uint8_t j = 0; j < 10; j++){
			if(getBit(i*LEDsPerDigit + j))
				Serial.print(readdress[j]);
		}
	}
	Serial.println();
}

uint8_t Lixie::get_numdigits() const{
	return NumDigits;
}

uint32_t Lixie::get_number(){
	return current_number;
}

bool Lixie::maxed_out(uint32_t input) const{
	if(get_size(input) > NumDigits) // If input > number that can be displayed
		return true;
	else
		return false;
}

CRGB* Lixie::get_leds() const{
	return leds;
}

void Lixie::build_controller(const uint8_t pin){
	#ifdef __AVR__
		if (pin == 0)
			controller = &FastLED.addLeds<LED_TYPE, 0, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 1)
			controller = &FastLED.addLeds<LED_TYPE, 1, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 2)
			controller = &FastLED.addLeds<LED_TYPE, 2, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 3)
			controller = &FastLED.addLeds<LED_TYPE, 3, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 4)
			controller = &FastLED.addLeds<LED_TYPE, 4, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 5)
			controller = &FastLED.addLeds<LED_TYPE, 5, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 6)
			controller = &FastLED.addLeds<LED_TYPE, 6, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 7)
			controller = &FastLED.addLeds<LED_TYPE, 7, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 8)
			controller = &FastLED.addLeds<LED_TYPE, 8, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 9)
			controller = &FastLED.addLeds<LED_TYPE, 9, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 10)
			controller = &FastLED.addLeds<LED_TYPE, 10, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 11)
			controller = &FastLED.addLeds<LED_TYPE, 11, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 12)
			controller = &FastLED.addLeds<LED_TYPE, 12, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 13)
			controller = &FastLED.addLeds<LED_TYPE, 13, COLOR_ORDER>(leds, NumLEDs);
	#elif ESP8266
		if (pin == 0)
			controller = &FastLED.addLeds<LED_TYPE, 0, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 2)
			controller = &FastLED.addLeds<LED_TYPE, 2, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 4)
			controller = &FastLED.addLeds<LED_TYPE, 4, COLOR_ORDER>(leds, NumLEDs);
		else if (pin == 5)
			controller = &FastLED.addLeds<LED_TYPE, 5, COLOR_ORDER>(leds, NumLEDs);
	#endif
}

void Lixie::max_power(byte volts, uint16_t milliamps){
	FastLED.setMaxPowerInVoltsAndMilliamps(volts,milliamps); 
}

void Lixie::white_balance(CRGB c_adj){
	controller->setTemperature(c_adj);
}

void Lixie::brightness(byte newBright){
	bright = newBright/255.0;
}

void Lixie::rainbow(uint8_t r_hue, uint8_t r_sep){
  for(uint8_t i = 0; i < NumDigits; i++){
    color(CHSV(r_hue,255,255),i);
    r_hue+=r_sep;
  }
}
