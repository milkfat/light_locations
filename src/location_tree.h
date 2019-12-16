
#define LIGHTS_HOSTNAME "tree"

#define NUM_LEDS 500
#define DATA_PIN 4


struct TREE_COORD {
    uint8_t x = 0;
    uint16_t y = 0;
    uint8_t z = 0;
};

#define MATRIX_HEIGHT 41
#define MATRIX_WIDTH 15

#define MATRIX_MIDDLE (MATRIX_WIDTH/2)

TREE_COORD tree_coords[NUM_LEDS+1]; //xyz coordinates for each pixel in 3d space
int16_t screen_coords[MATRIX_HEIGHT][MATRIX_WIDTH] = {NUM_LEDS}; //xy coordinates, so we can treat the tree as a simple 2d matrix

#include "config.h"
#include "common.h"
#include "bushes.h"
#include "candy_cane.h"

const int16_t tree_center[] = {-9,5,19,33,46,59,72,85,98,108,121,134,147,159,172,184,197,207,220,232,245,257,269,281,294,304,317,329,342,354,366,379,391,400,412,424,437,449,461,473,485,496,508,508};



void location_setup() {

    //CALCULATE 2D COORDINATES
    int temp_coords[NUM_LEDS+1][2];
    int min_x = 0;

    for (int i = 0; i < MATRIX_HEIGHT; i++) {
        for (int j = 0; j < MATRIX_WIDTH; j++) {
            screen_coords[i][j] = NUM_LEDS;
        }
    }

    //go through and fine coordinates for each LED, the middle of the string representing x == 0
    for ( int i = 0; i < MATRIX_HEIGHT; i++) {
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
