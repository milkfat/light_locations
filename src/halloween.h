#ifndef LIGHTS_HALLOWEEN_H
#define LIGHTS_HALLOWEEN_H

#define NUM_HLIGHTS 3
#define MAX_NUM_HLIGHTS 255

#include "light_sketches.h"

class HALLOWEEN: public LIGHT_SKETCH {
    uint8_t num_hlights = NUM_HLIGHTS;
    
  public:
    HALLOWEEN() {setup();}
    ~HALLOWEEN() {}

    void reset() {}
    void next_effect() {}

    
    unsigned char myArray[NUM_LEDS][3]; //create array to store HSV color

    struct HLIGHT {
        unsigned long time = millis();
        int pos = 0;
        int dir = 1;
        uint8_t hue = 0;
    };


    HLIGHT hlights[MAX_NUM_HLIGHTS];

    unsigned long time3 = millis();
    int pos3 = 0;
    
    public:
    
    void setup() { 
      for (int i = 0; i < MAX_NUM_HLIGHTS; i++) {
          hlights[i].pos = random(NUM_LEDS);
          hlights[i].dir = 1;
          if (random(2)) {
              hlights[i].dir = -1;
          }
          hlights[i].hue = random(256);
      }
      control_variables.add(num_hlights, "Number of lights", 0, 255);
    }

    void loop() {
        
        for (int i = 0; i < num_hlights; i++) {
            if ((millis() - hlights[i].time) > 50)
                {
                hlights[i].pos = (hlights[i].pos+hlights[i].dir+NUM_LEDS)%NUM_LEDS; //update the position according to chase direction
                hlights[i].time = millis();

                //try to make them bounce
                    for (int j = 0; j < num_hlights; j++) {
                        if ( i != j && (abs(hlights[i].pos-hlights[j].pos)<2) )// || (pos0 == 0) || (pos0 == (NUM_LEDS-1)))
                        {
                            hlights[i].dir = -hlights[i].dir;
                        }
                    }
                }
                else
                {
                //leds[i].setHSV(18,255,255);
                myArray[hlights[i].pos][0] = hlights[i].hue;  // H orange
                //myArray[pos0][0] = 171;  // H blue
                myArray[hlights[i].pos][1] = 255; // S
                myArray[hlights[i].pos][2] = 255; // V;
                }
        }

        for ( int i = 0; i < NUM_LEDS; i++ )
        {
            
            //write myArray to the LED object
            leds[i].setHSV(myArray[i][0],myArray[i][1],myArray[i][2]);

            //fade myArray by a small percentage each pass
            if (myArray[i][2] > 0)
            {
                myArray[i][2] = myArray[i][2]*.95;
            }
            else
            {
                myArray[i][2] = 0;
            }
        }
        
        //RANDOM WHITE
        if (millis() > time3)
            {
            leds[pos3].setRGB( 255, 255, 255);
            }
        if (millis() > (time3+25)) //flash full bright for this amount of milliseconds
            {
            leds[pos3].setRGB(32,32,32);
            time3 = (millis()+random16(250)); //rate of white flashes
            pos3 = random16(NUM_LEDS);
            }
            

            LED_show(); //update LEDs

            
    } 
};

LIGHT_SKETCHES::REGISTER<HALLOWEEN> halloween("halloween");

#endif