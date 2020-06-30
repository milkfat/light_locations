#ifndef LIGHTS_TREE_TEST_H
#define LIGHTS_TREE_TEST__H

#include "light_sketches.h"
#include <vector3.h>
    

class TREE_TEST: public LIGHT_SKETCH {

    uint8_t pos = 0;
    VECTOR3 loc = VECTOR3(0,0,0);

  public:

    TREE_TEST() {
        setup();
    }

    
    void setup() {
        control_variables.add(loc.x, "X", -50*256, 50*256);
        control_variables.add(loc.y, "Y", -50*256, 50*256);
        control_variables.add(loc.z, "Z", -50*256, 50*256);
    }

    void next_effect() {}

    void reset() {}

    void loop() {
        LED_show();
        LED_black();
        pos++;
        int pos2 = (pos*50*256)/256-25*256;

        for (int i = 0; i < NUM_LEDS; i++) {

            if (  abs(tree_coords[i].x - pos2) < 256 ) {
                leds[i].r = 32;
            } else {
                leds[i].r = 0;
            }


            if (  abs(tree_coords[i].y - pos2) < 256 ) {
                leds[i].g = 32;
            } else {
                leds[i].g = 0;
            }


            if (  abs(tree_coords[i].z - pos2) < 256 ) {
                leds[i].b = 32;
            } else {
                leds[i].b = 0;
            }

            int32_t dist = sqrt(sq(loc.x-tree_coords[i].x)+sq(loc.y-tree_coords[i].y)+sq(loc.z-tree_coords[i].z));

            leds[i].r += _max(10*256-dist,0)/20;
            leds[i].g += _max(10*256-dist,0)/20;
            leds[i].b += _max(10*256-dist,0)/20;


        }

    } 
};

LIGHT_SKETCHES::REGISTER<TREE_TEST> tree_test("Tree Test");

#endif