
#define LIGHTS_HOSTNAME "window"

#define NUM_LEDS0 150  //number of LEDS string 1
#define NUM_LEDS1 50  //number of LEDS string 2
#define NUM_LEDS (NUM_LEDS0 + NUM_LEDS1) //total number of LEDS
#define DATA_PIN0 5    //data pin for string 1
#define DATA_PIN1 4    //data pin for string 2

#define MATRIX_HEIGHT 200
#define MATRIX_WIDTH 1

#define DISABLE_DEFAULT_SKETCHES
#define LIGHTS_ADJUST
uint8_t adjust(int x);

#include "vector3.h"

VECTOR3 tree_coords[NUM_LEDS+1]; //xyz coordinates for each pixel in 3d space

void update_matrix();

#include "config.h"
#include "common.h"
#include "fireflies.h"
#include "halloween.h"
#include "leaf.h"
#include "fabulous.h"
#include "balls.h"
#include "confetti.h"
#include "snow.h"
#include "raindrops.h"
#include "fireworks.h"
#include "floating.h"
#include "rainbow_stretch.h"

#include "metaballs.h"
#include "growcircle.h"
#include "neon.h"
#include "simplex.h"
#include "snowflakes.h"
#include "phosphene.h"

void location_setup() { 
  //lines of LEDS with the x,y coordinates for each end
    int sects[6][6] =
    {
      {  0, 31, -30, -20, -30,  10},
      { 31, 23, -30,  10, -10,  20},
      { 54, 54, -10,  20,  30,   0},
      {108, 21,  30,   0,  30, -20},
      {129, 71,  30, -20, -30, -20},
    };

//figure out the distance of each pixel from the starting point
    for (byte i = 0; i < 6; i++) {
      for (int j = sects[i][0]; j < sects[i][0]+sects[i][1]; j++) {
        int x2 = map( j, sects[i][0], sects[i][0]+sects[i][1], sects[i][2], sects[i][4]);
        int y2 = map( j, sects[i][0], sects[i][0]+sects[i][1], sects[i][3], sects[i][5]);
        
        tree_coords[j].x = x2*256;
        tree_coords[j].y = y2*256;
        tree_coords[j].z = 0;
      }
    }

    Serial.println("light sketch initial loop started");
    delay(1000);
    light_sketches.loop();
    Serial.println("light sketch initial loop complete");
    Serial.println();

    Serial.println("assigning LEDs started");
    LEDS.addLeds<WS2812B, DATA_PIN0, RGB>(leds, NUM_LEDS0);
    LEDS.addLeds<WS2812B, DATA_PIN1, RGB>(leds, NUM_LEDS0, NUM_LEDS1);
    Serial.println("assigning LEDs complete");
    Serial.println();

}

void location_loop() {
    
    light_sketches.loop();
    
} 

#define LIGHTS_ADJUST
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

void update_matrix() {
    for (int i = 0; i < NUM_LEDS; i++) {
        gamma8_decode(leds[i]);
    }
    FastLED.show();
}