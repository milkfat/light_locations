
#define LIGHTS_HOSTNAME "window"

#define NUM_LEDS0 150  //number of LEDS string 1
#define NUM_LEDS1 50  //number of LEDS string 2
#define NUM_LEDS (NUM_LEDS0 + NUM_LEDS1) //total number of LEDS
#define DATA_PIN0 5    //data pin for string 1
#define DATA_PIN1 4    //data pin for string 2


uint8_t adjust(int x);

#include "config.h"
#include "common.h"
#include "fireflies.h"
#include "halloween.h"
#include "leaf.h"
#include "fabulous.h"

void location_setup() { 
    LEDS.addLeds<WS2812B, DATA_PIN0, RGB>(leds, NUM_LEDS0);
    LEDS.addLeds<WS2812B, DATA_PIN1, RGB>(leds, NUM_LEDS0, NUM_LEDS1);
    fireflies.setup();
    fabulous.setup();
    leaf.setup();
}

void location_loop() {
    
    switch (current_light_sketch) {
        case 0:
            fireflies.loop();
            break;
        case 1:
            halloween.loop();
            break;
        case 2:
            leaf.loop();
            break;
        case 3:
            fabulous.loop();
            break;
        default:
            current_light_sketch=0;
            break;
    }
    
} 


uint8_t adjust(int x) {
  //loop position around if it goes off either end of the string
  uint8_t y = (x+NUM_LEDS) % NUM_LEDS; 
  if (y < 150) {
    return y;
  } else {
    //lights 150-199 are in reverse order
    return 349-y;
  }
}