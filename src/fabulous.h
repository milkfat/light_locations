#ifndef LIGHTS_FABULOUS_H
#define LIGHTS_FABULOUS_H

#include "light_sketches.h"

class FABULOUS: public LIGHT_SKETCH {
  public:
    FABULOUS() {setup();}
    ~FABULOUS() {}

    void reset() {}
    void next_effect() {}

    public:
    CRGB rgb;
    unsigned long time0 = millis(); //frame clock (lock to ~60fps)
    unsigned long time1 = 0; //show clock (time until next show)
    unsigned long time2 = millis(); //speed clock (time until the speed of the effect changes)
    unsigned long time_spark = millis(); //time until next spark (if sparks are enabled)
    int bdistness = 255;
    uint8_t all_done = 0;
    uint8_t halt = 1;
    uint8_t h1 = 0;
    uint8_t h2 = 0;
    uint8_t next_color = 0;
    uint8_t frames = 20;
    uint8_t blank_pass = 0;
    uint8_t randomize = 0;
    char changedir = 1;
    uint8_t usebackground = 0;
    uint8_t fade_string = 0;
    float fade_value = 0.0;
    uint8_t mask_flash = 0;
    uint8_t sparks = 0;
    int longest_delay = 0;
    uint8_t set_new_color = 0;
    uint8_t counter = 0;
    uint8_t shift_all = 0;

    class SECTION {

    public:

        uint8_t start;
        uint8_t dist;
        int stp;
        int stps;
        uint8_t effect;
        uint8_t sequence;
        uint8_t sequence_step;
        uint8_t reverse;
        uint8_t h1;
        uint8_t s1;
        uint8_t v1;
        uint8_t h2;
        uint8_t s2;
        uint8_t v2;
        uint8_t invert;
        uint8_t off;
        uint8_t done = 1;
        uint8_t invert_direction;
        uint8_t shift;
        uint8_t invert_position;
        uint8_t color;
        uint8_t change_color;
        uint8_t ease;
        uint8_t reset_cnt;
        uint8_t divide;
        uint8_t divide_mirror;
    
    };

    SECTION section[9];

    class SPARK {

    public:

        uint8_t pos = 0;
        char spd = 0;
        uint8_t cnt = 0;
        uint8_t life = 0;
        uint8_t v = 0;
        char dir = 1;
    };

    SPARK spark[10];


    //(section orders with length at end) 
    uint8_t orders[3][18] =
    {
        {0,1,2,3,4,5,6,7,8,0,0,0,0,0,0,0,0,0},
        {2,1,0,0,1,2,3,4,3,1,1,1,0,0,0,0,0,1},
        {0,1,2,0,1,2,0,1,2,0,0,0,0,0,0,0,0,0}
    };

    uint8_t order = 0;

    uint8_t sequence[38][10] = 
    //0//effect
    //1//reverse, play effect backwards
    //2//invert foreground/background
    //3//off/draw with black
    //4//specify lights 1 = every light, 2 = every other light, 3 = every 3rd light, etc.
    //5//light offset 0 = start with first light, 1 = start with second light, etc
    //6//divide section into this number of subsections
    //7//mirror subsections
    //8//change color
    //9//ease in and out (0 = no easing, 1 = ease in, 2 = ease out, 3 = ease in and out)
    {
        {3,0,0,0,1,0,1,0,0,0},
        {3,0,1,0,1,0,1,0,0,0},
        {0,0,0,1,1,0,1,0,0,0},
        {0,0,0,1,1,0,1,0,0,0},
        {3,1,1,0,1,0,1,0,0,0},
        {3,1,0,0,1,0,1,0,0,0},
        //6
        {3,0,0,0,1,0,1,0,0,0},
        {3,0,1,0,1,0,1,0,0,0},
        //8
        {1,0,0,0,1,0,1,0,0,0},
        {1,0,0,1,1,0,1,0,0,0},
        //10
        {2,0,0,0,1,0,1,0,0,0},
        {2,0,1,0,1,0,1,0,0,0},
        {2,1,1,0,1,0,1,0,0,0},
        {2,1,0,0,1,0,1,0,0,0},
        //14
        {3,0,0,0,1,0,1,0,0,0},
        {1,0,0,0,1,0,1,0,0,0},
        //16
        {4,0,0,0,1,0,1,0,0,0},
        {4,1,0,0,1,0,1,0,0,0},
        //18
        {4,0,0,0,4,0,1,0,1,0},
        {4,0,0,0,4,1,1,0,0,0},
        {4,0,0,0,4,2,1,0,0,0},
        {4,0,0,0,4,3,1,0,0,0},
        {4,1,0,0,4,0,1,0,0,0},
        {4,1,0,0,4,1,1,0,0,0},
        {4,1,0,0,4,2,1,0,0,0},
        {4,1,0,0,4,3,1,0,0,0},
        //26
        {5,0,0,0,1,0,1,0,0,3},
        {5,1,0,0,1,0,1,0,0,3},
        //28
        {5,0,0,0,1,0,1,0,0,0},
        {5,1,0,0,1,0,1,0,0,0},
        //30
        {5,0,0,0,1,0,1,0,1,0},
        {5,0,0,0,1,0,1,0,0,0},
        //32
        {2,0,0,1,1,0,1,0,0,0},
        //33 bounce effect
        {2,0,0,0,1,0,1,0,0,1}, //drop in
        {7,1,0,0,1,0,1,0,0,2}, //bounce 1/2
        {7,0,0,0,1,0,1,0,0,1},
        {8,1,0,0,1,0,1,0,0,2}, //bounce 1/4
        {8,0,0,0,1,0,1,0,0,1},
    };

    const uint8_t shows[35][24] PROGMEM =
    //0//sequence start position
    //1//sequence length
    //2-10//initial sequence step for each section

    //11//frame delay per section (255 = random delays)
    //12//finish drawing all sections before starting again (loop); 2 = smooth loop (no delay between)
    //13//invert section order every other time (bounce back-and-forth, use with loop)
    //14//frames per sequence step (lower = faster)

    //15//use background color (0 = no background, 1 = use background color, 2 = black background, 3 = use background color, but black background on 1st pass)
    //16//new color on loop
    //17//last pass draws with black (blanks string)
    //18//effect duration in seconds (0 = default)

    //19//fade string 0 - 100 (slowest to fastest: 0 = no fade, 100 = instant fade)
    //20//add flashes between sections (mask areas)
    //21//order
    //22//sparks

    //23//shift on loop
    {
        {32,1, 0,0,0,0,0,0,0,0,0, 80,1,0,80, 2,0,0,0, 0,0,0,1, 0},
        
        {0,6, 0,3,0,3,0,3,0,3,0, 0,0,0,20, 2,0,0,0, 0,0,0,0, 0},

        //2 fade in/out
        {16,2, 0,0,0,0,0,0,0,0,0, 20,2,1,20, 0,0,0,10, 0,0,0,0, 0},
        {16,2, 0,0,0,0,0,0,0,0,0, 20,1,1,20, 0,0,0,5, 0,0,1,0, 0},
        {16,2, 0,0,0,0,0,0,0,0,0, 20,1,1,20, 0,1,0,5, 0,0,1,0, 0},
        {16,2, 0,0,0,0,0,0,0,0,0, 20,2,0,20, 0,1,0,10, 0,0,2,0, 0},

        //6 2-color smooth movement
        {10,2, 0,1,0,1,0,1,0,1,0, 0,0,0,20, 1,0,0,0, 0,0,0,0, 0},
        
        //7 zippy 
        {26,1, 0,0,0,0,0,0,0,0,0, 0,1,1,20, 2,1,0,5, 0,0,0,0, 0},
        {26,1, 0,0,0,0,0,0,0,0,0, 0,1,1,20, 0,1,1,5, 20,1,0,0, 0},
        {26,1, 0,0,0,0,0,0,0,0,0, 20,1,1,20, 0,1,0,5, 20,1,0,0, 0},
        {28,1, 0,0,0,0,0,0,0,0,0, 8,1,1,8, 0,1,0,5, 50,1,0,0, 0},
        {26,2, 0,0,0,0,0,0,0,0,0, 5,0,0,20, 2,1,0,5, 0,1,0,0, 0},
        {26,2, 0,0,0,0,0,0,0,0,0, 20,0,0,20, 2,1,1,5, 0,1,0,0, 0},

        //13 classic chasing effect
        {18,8, 0,4,0,4,0,4,0,4,0, 0,0,0,10, 0,0,0,5, 0,0,0,0, 0},
        {18,8, 0,4,0,4,0,4,0,4,0, 0,0,0,5, 0,0,0,5, 0,0,0,0, 0},

        //15 random fade
        {16,2, 0,0,0,0,0,0,0,0,0, 255,0,0,20, 0,0,0,10, 0,0,0,0, 0},

        //16center out
        {14,2, 0,0,0,0,0,0,0,0,0, 20,2,0,20, 0,1,0,0, 0,0,0,0, 0},
        {14,2, 0,0,0,0,0,0,0,0,0, 10,2,0,10, 0,1,1,0, 0,0,0,0, 0},

        //18neato back and forth
        {6,2, 0,0,0,0,0,0,0,0,0, 0,0,0,20, 2,0,0,0, 0,0,0,0, 0},
        {6,2, 0,0,0,0,0,0,0,0,0, 10,2,1,20, 2,0,0,0, 0,0,0,0, 0},
        {6,2, 0,0,0,0,0,0,0,0,0, 5,2,1,10, 2,1,0,0, 0,0,0,0, 0},
        {6,2, 0,0,0,0,0,0,0,0,0, 2,2,1,10, 2,0,0,0, 0,0,0,0, 0},
        {8,2, 0,0,0,0,0,0,0,0,0, 10,1,1,20, 2,0,0,0, 0,0,0,0, 0},
        {8,2, 0,0,0,0,0,0,0,0,0, 5,2,0,5, 2,1,0,0, 0,0,0,0, 0},
        {8,2, 0,0,0,0,0,0,0,0,0, 2,2,0,2, 2,1,0,3, 0,0,0,0, 0},
        {8,2, 0,0,0,0,0,0,0,0,0, 1,2,0,1, 2,1,0,2, 0,0,0,0, 0},
        //26
        {10,2, 0,0,0,0,0,0,0,0,0, 0,1,1,20, 2,0,0,0, 0,0,0,0, 0},
        
        {10,4, 0,0,0,0,0,0,0,0,0, 0,1,0,20, 2,0,0,0, 0,0,1,0, 0},  
        {10,2, 0,0,0,0,0,0,0,0,0, 10,1,1,20, 2,0,0,0, 0,0,0,0, 0},
        {10,2, 0,0,0,0,0,0,0,0,0, 20,2,0,20, 2,0,0,0, 0,0,0,0, 0},
        {10,2, 0,0,0,0,0,0,0,0,0, 8,2,0,8, 2,0,0,0, 0,0,0,0, 0},
        {10,2, 0,0,0,0,0,0,0,0,0, 8,1,0,8, 2,0,0,0, 0,0,1,0, 1},
        {10,2, 0,0,0,0,0,0,0,0,0, 8,2,0,8, 2,0,0,0, 0,0,1,0, 1},
        {10,4, 0,2,0,2,0,2,0,2,0, 0,0,0,20, 2,0,0,0, 0,0,0,0, 0},
        //34 bouncing effect
        {33,5, 0,0,0,0,0,0,0,0,0, 40,1,0,20, 3,1,0,10, 0,0,1,0, 0}
    };
    #define NUM_SHOWS 35

    uint8_t colors[4] = {0,40,96,160};

    uint8_t current_show = 33;

    //start light, number of lights, level
    uint8_t masks[10][3] = 
    {
        {199,3,0},
        {19,3,0},
        {29,3,0},
        {52,4,0},
        {79,5,0},
        {106,4,0},
        {127,3,0},
        {150,3,0},
        {176,3,0},
        {199,3,0}
    };


    void setup() { 

    //define sections
    uint8_t sects[9][16] = 
        {
        {0,20,0,20,1,0,0,0,160,255,255,0,255,0,0,0},
        {21,10,0,20,1,0,3,0,160,255,255,40,255,0,0,0},
        {31,21,0,20,1,0,0,0,160,255,255,96,255,0,0,0},
        {56,23,0,20,1,0,3,0,160,255,255,40,255,0,0,0},
        {84,22,0,20,1,0,0,0,160,255,255,96,255,0,0,0},
        {109,20,0,20,1,0,3,0,160,255,255,192,255,0,0,0},
        {129,22,0,20,1,0,0,0,160,255,255,192,255,0,0,0},
        {152,25,0,20,1,0,3,0,160,255,255,192,255,0,0,0},
        {178,22,0,20,1,0,0,0,160,255,255,192,255,0,0,0}
        };
    
    //initialize sections
    for (int i = 0; i < 9; i++) {
        section[i].start = sects[i][0];
        section[i].dist = sects[i][1];
        section[i].stp = sects[i][2] % sects[i][3]; //start step
        section[i].stps = sects[i][3]; //number of steps
        section[i].effect = sects[i][4];
        section[i].sequence = sects[i][5];
        section[i].sequence_step = sects[i][6];
        section[i].reverse = sects[i][7];
        section[i].h1 = sects[i][8],
        section[i].s1 = sects[i][9],
        section[i].v1 = sects[i][10],
        section[i].h2 = sects[i][11];
        section[i].s2 = sects[i][12];
        section[i].v2 = sects[i][13];
        section[i].invert = sects[i][14];
        section[i].shift = 0;
        section[i].invert_position = 0;
        section[i].off = sects[i][15];
        section[i].change_color = 0;
        section[i].ease = 0;
        section[i].invert_direction = 0;
        section[i].reset_cnt = 0;
    }
    };

    void loop() {

        

    if (millis() > time0) {
        
        //CONSOLE STUFF
        if (Serial.available() > 0) {
        uint8_t c = Serial.read(); // read the next char received
        if (c == 100 || c == 97) { //d = next show
            time1 = 0;
            halt = 1;
            for (uint8_t i = 0;i < 9; i++) {
            section[i].done = 1;
            }
            if(c == 97) {
            if(current_show == 0) { //a = previous show
                current_show = NUM_SHOWS-2;
            } else {
                current_show -= 2;
            }
            }
        }
        }

        //SHOWS
        all_done = 1;
        for (uint8_t i = 0;i < 9; i++) {
        if(section[i].done == 0) {
            all_done = 0;
            if(section[i].stp >= 0) {
            
            
            uint8_t sec = i;
            //adjust for section order
            sec = (sec + section[i].shift) % 9;
            if (section[i].invert_position == 1) {
                sec = 8 - i;
            }
            if (pgm_read_byte(&shows[current_show][15]) > 0 && blank_pass != 1) {
                
                //fill section with background color
                hsv2rgb_rainbow(CHSV(section[i].h2,section[i].s2,section[i].v2), rgb);
                if (section[i].off == 1) {
                hsv2rgb_rainbow(CHSV(0,0,0), rgb);
                }  else if (section[i].invert == 1) {
                hsv2rgb_rainbow(CHSV(section[i].h1,section[i].s1,section[i].v1), rgb);
                }
            
                for (uint8_t j = section[sec].start;j < section[sec].start + section[sec].dist ; j++) {
                leds[adjust(j)].r = rgb.r;
                leds[adjust(j)].g = rgb.g;
                leds[adjust(j)].b = rgb.b;
                }
            }
            
            //DRAW EFFECT
            uint8_t x;
            uint8_t y;
            uint8_t stp = section[i].stp;
            uint8_t stps = section[i].stps;
            
            //motion easing
            if (section[i].ease == 1) {
                //ease in
                stp = stp*stp/stps;
            } else if (section[i].ease == 2) {
                //ease out
                stp = stps - stp;
                stp = stp*stp/stps;
                stp = stps - stp;
            } else if ((section[i].ease == 3) && (stp < stps/2)) {
                //ease in and out part 1
                stp = stp*stp/(stps/2);
            } else if ((section[i].ease == 3) && (stp > stps/2)) {
                //ease in and out part 2
                stp = stps - stp;
                stp = stp*stp/(stps/2);
                stp = stps - stp;
            }

            //reverse effect
            if (section[i].reverse == 1) {
                stp = stps - stp;
            }
            
            int start = section[sec].start;
            int dist = section[sec].dist;
            dist /= section[i].divide;
            
            //effect 0 = off/background color
            if (section[i].effect == 0) {
                x = start;
                y = start;
            //effect 1 = on/foreground color
            } else if (section[i].effect == 1) {
                x = start;
                y = start + dist;
            //effect 2 = grow from low side
            } else if (section[i].effect == 2) {
                x = start;
                y = start + (dist*stp)/stps;
            //effect 3 = grow out from middle
            } else if (section[i].effect == 3) {
                if (stp == stps) { //try to account for rounding error
                x = start;
                y = start + dist;
                } else {
                int half = dist/2;
                x = start + half - (half*stp)/stps;
                y = start + half + (half*stp)/stps;
                }
            //effect 4 = fade in
            } else if (section[i].effect == 4) {
                x = start;
                y = start + (dist);
                section[i].v1 = (255*stp)/stps;
            //effect 5 = light moves from low side
            } else if (section[i].effect == 5) {
                char siz = 3;
                x = start + ((dist-siz)*stp)/stps;
                y = start + ((dist-siz)*stp)/stps + siz;
            //effect 6 = grow to high side from 1/4
            //effect 7 = grow to high side from 1/2
            //effect 8 = grow to high side from 3/4
            } else if (section[i].effect == 6 || section[i].effect == 7 || section[i].effect == 8) {
                char start_dist = dist/4;
                if (section[i].effect == 7) {
                start_dist *= 2;
                } else if (section[i].effect == 8) {
                start_dist *= 3;
                }
                x = start;
                y = start + start_dist + ((dist - start_dist)*stp) / stps;
            }
            
            uint8_t s0 = min(x,y);
            uint8_t e0 = max(x,y);

            
            for ( uint8_t k = 0; k < section[i].divide; k++ ) {
                //divide sections
                uint8_t s = s0 + (dist*k);
                uint8_t e = e0 + (dist*k);
                
                //invert LEDs in section
                if ( (section[i].invert_position == 1 && section[i].invert_direction == 0) || (section[i].invert_position == 0 && section[i].invert_direction == 1) ) {
                uint8_t s1 = s;
                s = e;
                e = s1;
                s -= section[sec].start;
                s = section[sec].dist - s;
                s += section[sec].start;
                e -= section[sec].start;
                e = section[sec].dist - e;
                e += section[sec].start;
                }
                
                //foreground color
                hsv2rgb_rainbow(CHSV(section[i].h1,section[i].s1,section[i].v1), rgb);
                if (section[i].off == 1 || blank_pass == 1) {
                hsv2rgb_rainbow(CHSV(0,0,0), rgb);
                }  else if (section[i].invert == 1) {
                hsv2rgb_rainbow(CHSV(section[i].h2,section[i].s2,section[i].v2), rgb);
                }
                uint8_t led_div = sequence[ section[i].sequence_step + pgm_read_byte(&shows[current_show][0]) ][4];
                uint8_t led_offset = sequence[ section[i].sequence_step + pgm_read_byte(&shows[current_show][0]) ][5];
                for (uint8_t j = s;j < e; j++) {
                if ((j+led_offset) % led_div == 0) {
                    //if we're "fading out" then approximate the fading effect to preserve light color
                    if (section[i].effect == 4 && section[i].reverse == 1 && stp != 0) {
                    float val = .8;
                    if (section[i].stps > 20) {
                        val = .9;
                    } else if (section[i].stps < 10) {
                        val = .6;
                    }
                    leds[adjust(j)].r *= val;
                    leds[adjust(j)].g *= val;
                    leds[adjust(j)].b *= val;
                    //light normally
                    } else {
                    leds[adjust(j)].r = rgb.r;
                    leds[adjust(j)].g = rgb.g;
                    leds[adjust(j)].b = rgb.b;
                    }
                }
                }
    
                //SPARKS
                if ( sparks == 1 && (millis() > time_spark) ) {
                for (uint8_t j = 0; j < 10; j++) {
                    if (spark[j].v == 0) {
                    spark[j].pos = e;
                    spark[j].spd = random(1,5); //(lower = faster)
                    spark[j].cnt = 0;
                    spark[j].life = random(5,30);
                    spark[j].v = 255;
                    time_spark = millis() + random(200);
                    if (random(2) == 1) {
                        spark[j].dir *= -1;
                    }
                    break;
                    }
                
                    
                }
                }

                
            }
            
            section[i].stp += 1;


        
            //end of sequence, figure out what to happens next
            if(section[i].stp > section[i].stps) {

                //flash masks
                if(mask_flash == 1) {
                uint8_t mask = sec+1;
                if((section[sec].reverse == 1 && section[i].invert_position == 0) || (section[sec].reverse == 0 && section[i].invert_position == 1)) {
                    mask = sec;
                }
                masks[mask][2] = 255;
                
                }

                //move to next sequence
                section[i].sequence_step = (section[i].sequence_step + 1) % pgm_read_byte(&shows[current_show][1]);
                section[i].stp = 0;
                section[i].stps = frames;

                //change color when looping with halt == 2
                //this might be fragile...
                if(section[i].sequence_step == 0 && halt == 2) {
                section[i].shift += shift_all;
                counter += 1;
                
                //add appropriate delay so the effect goes around continuously
                section[i].stp = longest_delay - pgm_read_byte(&shows[current_show][11]) + pgm_read_byte(&shows[current_show][1]) * pgm_read_byte(&shows[current_show][14]);
                //invert sections for looping back-and-forth
                if (pgm_read_byte(&shows[current_show][13]) == 1) {
                    section[i].invert_position = (section[i].invert_position + 1) % 2;   
                    section[i].stp += pgm_read_byte(&shows[current_show][11]);
                } else {
                    //section[i].stp += pgm_read_byte(&shows[current_show][11]);
                }
                //if we change colors then do it here
                if (set_new_color == 1) {
                    while (next_color == section[i].h1) {
                    next_color = colors[random(4)];
                    }
                    section[i].h2 = h1;
                    section[i].h1 = next_color;
                }
                }
                

                
                //random delay and color
                if ( randomize == 1 ) {
                section[i].stp = -random(6*frames);
                if (section[i].sequence_step == 0 ) {
                    next_color = colors[random(4)];
                    while (next_color == section[0].h1) {
                    next_color = colors[random(4)];
                    }
                    h2 = h1;
                    h1 = next_color;
                    section[i].h1 = h1;
                    section[i].h2 = h2;
                }
                }
                
                if (section[i].sequence_step == 0 && halt == 1) {
                section[i].done = 1;
                }
                uint8_t sequence_position = section[i].sequence_step + pgm_read_byte(&shows[current_show][0]);
                section[i].effect = sequence[sequence_position][0];
                section[i].reverse = sequence[sequence_position][1];
                section[i].invert = sequence[sequence_position][2];
                section[i].off = sequence[sequence_position][3];
                section[i].change_color = sequence[sequence_position][8];
                section[i].ease = sequence[sequence_position][9];


                if (section[i].change_color == 1) {
                    next_color = colors[random(4)];
                    while (next_color == section[0].h1) {
                    next_color = colors[random(4)];
                    }
                    h2 = h1;
                    h1 = next_color;
                    section[i].h1 = h1;
                    section[i].h2 = h2;
                    section[i].v2 = section[i].v1;
                }
            }
    
            
        
            } else if (section[i].done == 0) {
            section[i].stp += 1;
            } 
        }
        }

        //change speed
    //    if(millis() > time2) {
    //      time2 += 300;
    //      if (frames > 19) {
    //        changedir = -1;
    //      } else if (frames < 5) {
    //        changedir = 1;
    //      }
    //      frames += changedir;
    //    }
    
        //switch shows
        if(millis() > time1 && halt != 1) {
        halt = 1;
        }

        if (all_done == 1) {

        //invert sections for shows that bounce back and forth
        if(pgm_read_byte(&shows[current_show][13]) == 1) {
            for (int i = 0; i < 9; i++) {
            section[i].invert_position = (section[i].invert_position + 1) % 2;
            }
            
        }
        set_new_color = 0;
        if (pgm_read_byte(&shows[current_show][16]) == 1) {
            set_new_color = 1;
        }

        //change show
        if(millis() > time1) {
            if(blank_pass < 2) {
            current_show = (current_show+1) % NUM_SHOWS;
            halt = pgm_read_byte(&shows[current_show][12]);
            frames = pgm_read_byte(&shows[current_show][14]);
            uint8_t duration = 5;
            if (pgm_read_byte(&shows[current_show][18]) > 0) {
                duration = pgm_read_byte(&shows[current_show][18]);
            }
            time1 = millis() + duration * 1000;
            set_new_color = 1;
            fade_string = pgm_read_byte(&shows[current_show][19]);
            fade_value = 1.0 - ( pgm_read_byte(&shows[current_show][19]) ) / 100.0;
            blank_pass = pgm_read_byte(&shows[current_show][17])*2;
            mask_flash = pgm_read_byte(&shows[current_show][20]);
            for (int i = 0; i < 9; i++) {
                section[i].v1 = 255;
                section[i].invert_position = 0;
                section[i].shift = 0;
                section[i].reset_cnt = 0;
            }
            order = pgm_read_byte(&shows[current_show][21]);
            sparks = pgm_read_byte(&shows[current_show][22]);
            shift_all = pgm_read_byte(&shows[current_show][23]);
            } else {
            blank_pass = 1;
            mask_flash = 0;
            }
        }

        
        if (set_new_color == 1) {
            next_color = colors[random(4)];
            while (next_color == h1) {
            next_color = colors[random(4)];
            }
            h2 = h1;
            h1 = next_color;
        }
        
        //reset show
        longest_delay = 0;
        for (int i = 0; i < 9; i++) {
            section[i].done = 0;
            if (halt == 2 && pgm_read_byte(&shows[current_show][13]) == 1 && i == 8) {
            section[i].done = 1;
            }
            uint8_t section_step = orders[order][i];
            section[i].sequence_step = pgm_read_byte(&shows[current_show][orders[order][section_step]+2]);
            //section[i].sequence_step = shows[current_show][i+2];
            section[i].invert_direction = orders[order][i+9];
            section[i].stp = -pgm_read_byte(&shows[current_show][11])*section_step;
            if(section[i].stp < longest_delay) {
            longest_delay = section[i].stp;
            }
            section[i].shift += shift_all;
            section[i].stps = frames;
            section[i].h2 = h2;
            section[i].h1 = h1;
            section[i].v2 = 0;
            randomize = 0;
            if ( pgm_read_byte(&shows[current_show][11]) == 255 ) {
            randomize = 1;
            section[i].stp = -random(120);
            section[i].h1 = colors[i%4];
            section[i].h2 = colors[(i+1)%4];
            }
            if(pgm_read_byte(&shows[current_show][15]) == 1 || ( pgm_read_byte(&shows[current_show][15]) == 3 && section[i].reset_cnt != 0 ) ) {
            section[i].v2 = 255;
            }

            uint8_t sequence_position = section[i].sequence_step + pgm_read_byte(&shows[current_show][0]);
            section[i].divide = sequence[sequence_position][6];
            section[i].divide_mirror = sequence[sequence_position][7];
            section[i].effect = sequence[sequence_position][0];
            section[i].reverse = sequence[sequence_position][1];
            section[i].invert = sequence[sequence_position][2];
            section[i].off = sequence[sequence_position][3];
            section[i].change_color = sequence[sequence_position][8];
            section[i].ease = sequence[sequence_position][9];
            section[i].reset_cnt += 1;
        }
        
        
        }
        
        //draw/fade mask areas
        for (uint8_t i = 0; i < 10; i++) {
        if(masks[i][2] > 0) {
            for (uint8_t j = masks[i][0]; j < masks[i][0]+masks[i][1]; j++) {
            uint8_t k = j % NUM_LEDS;
            leds[adjust(k)].r = masks[i][2];
            leds[adjust(k)].g = masks[i][2];
            leds[adjust(k)].b = masks[i][2];
            }
            masks[i][2] *= .7;
            if(masks[i][2] == 0 ) {
            for (uint8_t j = masks[i][0]; j < masks[i][0]+masks[i][1]; j++) {
                uint8_t k = j % NUM_LEDS;
                leds[adjust(k)].r = masks[i][2];
                leds[adjust(k)].g = masks[i][2];
                leds[adjust(k)].b = masks[i][2];
            }
            }
        }
        }


        //draw sparks

        for (uint8_t i = 0; i < 10; i++) {
        if (spark[i].v > 0) {
            //subtract spark's life
            if(spark[i].life > 0) {
            spark[i].life--;
            //dim the spark
            } else {
            spark[i].v *= .5;
            }
            //update the iteration count
            spark[i].cnt++;
            //if iteration count is == speed, then move the spark one position
            if(spark[i].cnt == spark[i].spd) {
            spark[i].pos += spark[i].dir;
            spark[i].cnt = 0;
            spark[i].spd += random(0,1);
            }
            if(spark[i].pos > 0 && spark[i].pos < NUM_LEDS) {
            leds[adjust(spark[i].pos)].r = spark[i].v;
            leds[adjust(spark[i].pos)].g = spark[i].v;
            leds[adjust(spark[i].pos)].b = spark[i].v;
            } else {
            spark[i].v = 0;
            }
        }
        //pos spd cnt life v
        }


        




        //DRAW STRING
        FastLED.show();

        if (sparks == 1) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[adjust(i)].r = 0;
            leds[adjust(i)].g = 0;
            leds[adjust(i)].b = 0;
        }
        }

        
        //fade sections
        if(fade_string > 0) {
        for (int h = 0; h < 9; h++) {
            for (int i = section[h].start; i < (section[h].start + section[h].dist); i++)
                {
                leds[adjust(i)].r *= fade_value;
                leds[adjust(i)].g *= fade_value;
                leds[adjust(i)].b *= fade_value;
                }
        }
        }

        
    time0 += 16;
    
    }
    
    } 
};


LIGHT_SKETCHES::REGISTER<FABULOUS> fabulous("fabulous");

#endif