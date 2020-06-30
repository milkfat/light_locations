
#define LIGHTS_HOSTNAME "tree"
#include "vector3.h"
#define NUM_LEDS0 500
#define NUM_LEDS1 300
#define DATA_PIN 4
#define DATA_PIN1 5
#define NUM_LEDS (NUM_LEDS0 + NUM_LEDS1)

typedef VECTOR3 TREE_COORD;
// struct TREE_COORD {
//     uint8_t x = 0;
//     uint16_t y = 0;
//     uint8_t z = 0;
// };

#define LED_ROWS (41+26)
#define MATRIX_HEIGHT 41
#define MATRIX_WIDTH 30

#define MATRIX_MIDDLE 7

TREE_COORD tree_coords[NUM_LEDS+1]; //xyz coordinates for each pixel in 3d space
uint8_t tree_radians[NUM_LEDS];
int16_t screen_coords[MATRIX_HEIGHT][MATRIX_WIDTH] = {NUM_LEDS}; //xy coordinates, so we can treat the tree as a simple 2d matrix


void update_matrix();

#ifndef XY_DEFINED
#define XY_DEFINED
static inline __attribute__ ((always_inline)) uint32_t XY(const int& x, const int& y) {
    int val = screen_coords[y][x];
    if (x >= 0 && x < MATRIX_WIDTH && y >= 0 && y < MATRIX_HEIGHT) {
        val = screen_coords[y][x];
    } else {
        val = NUM_LEDS-1;
    }
    if (val >= 0 && val < NUM_LEDS) {
        return val;
    } else {
        return NUM_LEDS;
    }
}
#endif
#define DISABLE_DEFAULT_SKETCHES
#include "light_sketches.h"

#include "config.h"
#include "common.h"
//#include "bushes.h"
#include "tree_sim.h"
#include "tree_test.h"
//#include "candy_cane.h"

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

const int16_t tree_center[100] = {-9,5,19,33,46,59,72,85,98,108,121,134,147,159,172,184,197,207,220,232,245,257,269,281,294,304,317,329,342,354,366,379,391,400,412,424,437,449,461,473,485,496
,503,516,529,542,555,567,580,592,602,614,626,638,650,661,673,685,697,706,718,729,741,752,763,775,786,798, 810,822,834,846,858,870,882,894,906,918,930,942,954,966,978,990,1002,1014};

void update_matrix() {
    for (int i = 0; i < NUM_LEDS; i++) {
        gamma8_decode(leds[i]);
    }
    FastLED.show();
}


    int temp_coords[NUM_LEDS+1][2];
void location_setup() {

    //CALCULATE 2D COORDINATES
    int min_x = 0;

    for (int i = 0; i < MATRIX_HEIGHT; i++) {
        for (int j = 0; j < MATRIX_WIDTH; j++) {
            screen_coords[i][j] = NUM_LEDS;
        }
    }

    //go through and find coordinates for each LED, the middle of the string representing x == 0
    for ( int i = 0; i < LED_ROWS; i++) {
        int min_led = (tree_center[i]+tree_center[i+1])/2;
        int max_led = (tree_center[i+1]+tree_center[i+2])/2;
        int mid_led = tree_center[i+1];

        int x_val = 0;

        for (int j = mid_led; j >= min_led; j--) {
            if (x_val < min_x) {
                min_x = x_val;
            }
            if (j/100 != (j+1)/100) {
                x_val-=2;
            }
            if (j >= 0 && j < NUM_LEDS-1) {
                temp_coords[j][0] = x_val;
                temp_coords[j][1] = i;
            }
            x_val--;
        }

        x_val = 0;

        for (int j = mid_led; j < max_led; j++) {
            if (x_val < min_x) {
                min_x = x_val;
            }
            if ((j-1)/100 != j/100) {
                x_val+=2;
            }
            if (j >= 0 && j < NUM_LEDS-1) {
                temp_coords[j][0] = x_val;
                temp_coords[j][1] = i;
            }
            x_val++;
        }

    }

    //shift everything over so that our minimum x value becomes 0
    for (int i = 0; i < NUM_LEDS; i++) {
        temp_coords[i][0]-=min_x;
    }

    //shift the second trunk into place
    for (int i = NUM_LEDS0; i < NUM_LEDS; i++) {
        temp_coords[i][0]+=15;
        temp_coords[i][1]-=30;
    }

    //update our screen_coords array
    for (int i = 0; i < NUM_LEDS; i++) {
        int x = temp_coords[i][0];
        int y = temp_coords[i][1];
        if (x >= 0 && x < MATRIX_WIDTH && y >= 0 && y < MATRIX_HEIGHT) {
            screen_coords[y][x] = i;
        }
    }



    //CALCULATE 3D COORDINATES

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

        tree_coords[i].x = (relative_position*256) / circle_size; //radial coordinates? how dumb of me...
        tree_radians[i] = tree_coords[i].x;
        // y coordinates
        tree_coords[i].y = (circle_height*65536)/number_of_circles + tree_coords[i].x;

        // z coordinates
        tree_coords[i].z = ease8InOutCubic(abs(128-tree_coords[i].x)*2);

        VECTOR3 vec = VECTOR3(0, -20*256, -4*256);
        rotate_y(vec, (relative_position*240) / circle_size);
        vec.y += (circle_height*256) + (relative_position*256) / circle_size;
        if ( i >= NUM_LEDS0) {
            vec.y -= 30*256;
            vec.x += 10*256;
            vec.z += 10*256;
        }
        tree_coords[i] = vec;



    }
    delay(1000);
    #ifdef DEBUG
    Serial.println("initializing light sketches");
    #endif
    light_sketches.loop();
    #ifdef DEBUG
    Serial.println("light sketches initialized");
    #endif
    LEDS.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS0);
    LEDS.addLeds<WS2811, DATA_PIN1, RGB>(leds, NUM_LEDS0, NUM_LEDS1);
    delay(1000);
}

void location_loop() {
    light_sketches.loop();
    // switch (current_light_sketch) {
    //     case 0:
    //         bushes.loop();
    //         break;
    //     case 1:
    //         candy_cane.loop();
    //         break;
    //     default:
    //         current_light_sketch=0;
    //         break;
    // }
     
} 
