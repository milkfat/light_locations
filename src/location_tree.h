
#define LIGHTS_HOSTNAME "tree"

#define NUM_LEDS 500
#define DATA_PIN 4


struct TREE_COORD {
    uint8_t x = 0;
    uint16_t y = 0;
    uint8_t z = 0;
};

TREE_COORD tree_coords[NUM_LEDS+1];

#include "config.h"
#include "common.h"
#include "bushes.h"
#include "candy_cane.h"

int16_t tree_center[] = {-9,5,19,33,46,59,72,85,98,108,121,134,147,159,172,184,197,207,220,232,245,257,269,281,294,304,317,329,342,354,366,379,391,400,412,424,437,449,461,473,485,496,508};



void location_setup() {

    int number_of_circles = (sizeof(tree_center))/sizeof(tree_center[0]);
    //calculate the coordinates of each LED
    int tree_center_pos = 1;
    int circle_height = 0;

    for (int i = 0; i < NUM_LEDS; i++) {

        if (tree_center[tree_center_pos] <= i) {
            tree_center_pos++;
            circle_height++;
        }

        // x coordinates
        int circle_size = tree_center[tree_center_pos] - tree_center[tree_center_pos-1];
        circle_size+=(tree_center[tree_center_pos]/100-tree_center[tree_center_pos-1]/100)*2; //account for the gap between strings of 100
        int relative_position = i - tree_center[tree_center_pos-1];
        relative_position+=(i/100 - tree_center[tree_center_pos-1]/100)*2; //account for the gap between strings of 100

        tree_coords[i].x = (relative_position*256) / circle_size;

        // y coordinates
        tree_coords[i].y = (circle_height*65536)/number_of_circles + tree_coords[i].x;

        // z coordinates
        tree_coords[i].z = ease8InOutCubic(abs(128-tree_coords[i].x)*2);


    }
    LEDS.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    bushes.setup();
    candy_cane.setup();
}

void location_loop() {

    switch (current_light_sketch) {
        case 0:
            bushes.loop();
            break;
        case 1:
            candy_cane.loop();
            break;
        default:
            current_light_sketch=0;
            break;
    }
     
} 
