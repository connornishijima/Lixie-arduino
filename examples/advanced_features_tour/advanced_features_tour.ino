/*
   ____ ___  ____ _  _     ____ ____ ____ _ ____ _        _  _ ____ _  _ _ ___ ____ ____   /
   |  | |__] |___ |\ |     [__  |___ |__/ | |__| |        |\/| |  | |\ | |  |  |  | |__/  / 
   |__| |    |___ | \|     ___] |___ |  \ | |  | |___     |  | |__| | \| |  |  |__| |  \ .  

  (115200 Baud)
*/                                                                                                                

void title(){
  Serial.println(F("----------------------------------"));
  Serial.println(F(" Lixie Advanced Features Tour     "));
  Serial.println(F(" by Connor Nishijima              "));
  Serial.println(F(" July 16th, 2017                  "));
  Serial.println();
  Serial.println(F(" Released under the GPLv3 License "));
  Serial.println(F("----------------------------------"));
}

#include "Lixie.h" // Include Lixie Library

#define DATA_PIN   11 // Pin to drive Lixies
#define NUM_LIXIES 6  // How many Lixies you have
Lixie lix(DATA_PIN, NUM_LIXIES);

void setup() {
  Serial.begin(115200);
  lix.begin(); // Initialize LEDs
  lix.brightness(255); // 0-255
  lix.white_balance(Tungsten100W); // Default
    // Can be: Tungsten40W, Tungsten100W,  
    //         Halogen,     CarbonArc,
    //         HighNoonSun, DirectSunlight,
    //         OvercastSky, ClearBlueSky  
    // 2,600K - 20,000K
}

void loop() {
  // Intro
  fill_zero();
  title();
  delay(3000);
  dialog(F("\nThis sketch demonstrates some advanced Lixie features!"),3000);
  dialog(F("Follow along here to see what's going on!\n"),3000);
  
  // Full color functions...
  dialog(F("This is the lix.fill_fade_in/out() function in various colors..."),0);
  rainbow_fade();
  
  dialog(F("lix.sweep() can be used to show a waiting function such as connecting to WiFi..."),0);
  rainbow_sweep();
  
  dialog(F("Your Lixies can also show a rudimentary progress bar...\n"),0);
  prog_bar();
  
  dialog(F("Let's count down from 515 to 495 with the standard lix.write()..."),1000);
  normal_count();
  
  dialog(F("Now we'll do the same countdown with two fancier functions:"),1000);
  dialog(F(" - lix.write_fade()"),0);
  fade_count();
  
  dialog(F(" - lix.write_flip()\n"),0);
  flip_count();

  dialog(F("And here's millis() shown with lix.rainbow():"),1000);
  rainbow_count();

  dialog(F("You can also control the 'off' color of your panels:"),1000);
  hue_cycle();

  /*
    Nixie Mode activate!
    This mode emulates Nixie Tubes using two specific details:
      - Ionizing Color (amber)
      - Aura Color (very faint blue)

    Any custom colors used while Nixie Mode is active
    will NOT be shown until it is turned off!
  */

  dialog(F("\nAnd for the Nixie Tube fans..."),3000);
  
  lix.nixie_mode(true); // Activate Nixie Mode
  lix.nixie_aura_intensity(8); // Default aural intensity is 8 (0-255)
  // Subtlety is key with the aura, this default 3% brightness should do it.

  lix.write(123456);  
  dialog(F("lix.nixie_mode() has a preset color and neon aural emulation to get a very nice Nixie look!"),3000);
  hue_cycle(); // Reuse the random rainbow numbers to show color ignorance
  delay(1000);
  
  dialog(F("It also works with all the fancy write functions from before."),1000);
  fade_count(); // Resuse the fading countdown

  dialog(F("You can also disable the faint blue neon aura if you'd like with lix.nixie_aura_intensity(0)..."),1000);
  lix.nixie_aura_intensity(0); // Turn off the aura  
  hue_cycle(); // One more cycle

  dialog(F("Or make it more visible! - lix.nixie_aura_intensity(14)"),1000);
  lix.nixie_aura_intensity(14); // Turn off the aura  
  hue_cycle(); // One more cycle

  lix.nixie_aura_intensity(8);
  lix.write(123456);  

  dialog(F("\nAnd that's the end of our advanced Lixie features tour!"),0);
  dialog(F("Feel free to share any cool creations of yours and I can include them in the examples!\n"),0);
  dialog(F("Be sure to check the code to see how exactly these functions are used!\n"),10000);
  dialog(F("\n\n\n\n\n"),0);
  lix.nixie_mode(false); // Nixie Mode off
}

// Uses the lix.fill_fade_in/out() function across different colors!
void rainbow_fade(){
  for(int i = 0; i < 255; i+=25){
    lix.fill_fade_in(CHSV(i,255,255));
    lix.fill_fade_out(CHSV(i,255,255));
  }
}

// Uses the lix.sweep() function across different colors!
// lix.sweep() takes a color and frame speed (ms) as input,
// and wipes a "hot spot" across the displays.
void rainbow_sweep(){
  for(int i = 0; i < 255; i+=25){
    lix.sweep(CHSV(i,255,255),5);
  }
  lix.fill_fade_out(CHSV(0,255,255));
}

// Progress bars are possible with lix.progress(), and have
// three parameters: progress, bar color, background color.
void prog_bar(){
  lix.fill_fade_in(CRGB(0,255,255));
  for(float i = 0; i < 100; i+=0.1){
    lix.progress(i,CRGB(255,0,255),CRGB(0,255,255));
  }
  delay(1000);
  lix.fill_fade_out(CRGB(255,0,255));
}

// This demonstrates basic number display with lix.write()
void normal_count(){
  for(int i = 515; i >= 495; i--){
    lix.write(i);
    delay(400);
  }
  delay(1000);
}

// This counts down with lix.write_fade() which in this case
// uses 250ms to fade between the old number to the new one.
// It will only fade out digits which have changed.
void fade_count(){
  int fade_time = 250;
  
  for(int i = 515; i >= 495; i--){
    lix.write_fade(i, fade_time);
  }
  delay(1000);
}

// This countdown uses lix.write_flip(), which flashes a random
// sequence of numbers on digits which have changed, similar in
// effect to old Solari flap displays used in train stations.
void flip_count(){
  byte flip_time_ms = 200;
  byte flip_speed_ms = 15;
  
  for(int i = 515; i >= 495; i--){
    lix.write_flip(i, flip_time_ms, flip_speed_ms);
    delay(400);
  }
  delay(1000);
}

// Uses the lix.rainbow() function to show millis() value.
// hue is the color of the first digit, and the separation is
// hue+(sep*display_index) which allows for showing the
// entire color wheel, or just a portion at a time.
void rainbow_count(){
  byte hue = 0;
  byte color_separation = 20;
  
  for(uint16_t i = 500; i--;){
    lix.rainbow(hue,color_separation);
    lix.write(1000000+millis());
    hue++;
  }
  delay(1000);
}

// This demonstrates the lix.color_off() function. This is
// the color shown on all panes which aren't currently active.
// For example, if a Lixie is showing a "9", panes 0-8 will be
// the color set by lix.color_off();
void hue_cycle(){
  byte hue = 0;
  for(uint16_t i = 120; i--;){
    lix.color(CHSV(hue,255,255));
    lix.color_off(CHSV(hue+127,255,127));
    random_numbers();
    delay(40);
    hue++;
  }
  lix.color(CRGB(255,255,255));
  lix.color_off(CRGB(0,0,0));
}

// Writes random numbers to the displays.
void random_numbers(){
  uint32_t min_num = pow(10,(NUM_LIXIES)); // gets number big enough for NUM_LIXIES
  uint32_t max_num = min_num*2;
  lix.write(random(min_num,max_num));
}

// Writes zeroes to all displays.
void fill_zero(){
  lix.write(pow(10,(NUM_LIXIES))); // gets number big enough for NUM_LIXIES
}

void dialog(String input, uint16_t wait){
  Serial.println(input);
  delay(wait);
}
