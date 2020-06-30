#ifndef LIGHTS_FIREFLIES_H
#define LIGHTS_FIREFLIES_H


#define FIREFLY_AMOUNT 17 //amount of fireflies
#define MAX_FIREFLY_AMOUNT 255

#include "light_sketches.h"

class FIREFLY {
  
  public:
  
   int startpos; //firefly's start position
   int endpos; //firefly's end position
   unsigned int pos; //firefly's current position
   int stp; //255 "steps" from startpos to endpos
   int spd; //speed... number of steps to move for each loop (higher for shorter distances)
   int hue; //hue of firefly
   int dist; //distance between startpos and endpos
   int dist1; //non-zero distance used to calculate speed
   int flash; //-1 = no flash, 1 = flash (blink)
   int levels; //amount of interpolation between lights (smoother movement)
   unsigned long time0;

   void init(){
     endpos=random(NUM_LEDS-1); //set the pre-initial endpoint
     hue=random(255); //set a random hue
     newtarget(); //give the firefly its initial target position
     time0=millis(); //flash time
     flash=-1; //to flash or not to flash
     levels=255; //levels of interpolation
   }
   
   void newtarget(){
      stp=0; //current "step", 255 steps between positions
      startpos=(endpos+NUM_LEDS)%NUM_LEDS; //new startpoint is old endpoint
      
      if (random(255) > 253)
       {
        endpos=startpos+random(NUM_LEDS)-NUM_LEDS/2; //make the firefly move to a random position on the string (infrequently)
       }
       else
       {
        endpos=startpos+random(3)-random(3); //make the firefly "flutter in place"
       }
       
      dist=endpos-startpos; //distance between positions
      
      dist1=dist; //we need a non-zero distance in order to calculate our speed
      if (dist == 0)
       {
         dist1 = 1;
       }
      spd=10/(abs(dist1)); //keep the speed relatively constant regardless of distance
      if (spd == 0) //make sure we have a non-zero speed
       {
        spd = 1;
       }
   }

   void upd(){
     pos = startpos*levels+ease8InOutCubic(stp)*dist/(255.0/levels); //ease in and out of motion
     //leds[pos] += CHSV(hue,255,128); //light our LED
    // leds[pos/10] = CRGB(104+flash*92,38+flash*32,0);
     int pos1 = adjust(pos/levels+1);
     int pos2 = adjust(pos/levels);
     if (flash==1)
      {
        if (pos1 >= 0 && pos1 < NUM_LEDS) {
          leds[pos1] = CRGB((pos%levels)*(138)/levels,(pos%levels)*(54)/levels,0);
        }
        if (pos2 >= 0 && pos2 < NUM_LEDS) {
          leds[pos2] = CRGB(((levels-1)-(pos%levels))*(138)/levels,((levels-1)-(pos%levels))*(54)/levels,0);
        }
      } else
      {
        if (pos1 >= 0 && pos1 < NUM_LEDS) {
          leds[pos1] = CRGB((pos%levels)*(18)/levels,(pos%levels)*(9)/levels,0);
        }
        if (pos2 >= 0 && pos2 < NUM_LEDS) {
          leds[pos2] = CRGB(((levels-1)-(pos%levels))*(18)/levels,((levels-1)-(pos%levels))*(9)/levels,0);
        }
      }
     if ( millis() > time0 )
      {
        flash = -flash;
        if (flash == 1)
         {
          time0 = millis() + random(500); //stay lit for short amount of time
         }
         else
         {
          time0 = millis() + random(4000); //stay dim for longer amount of time
         }
      }
     if (stp > 254-spd) //give the firefly a new target when at the end of motion
     {
      newtarget();
     }
     else
     {
      stp += spd; //increment the firefly step (move it)
     }
   }

     
};

class FIREFLIES: public LIGHT_SKETCH {
  uint8_t number_of_flies = FIREFLY_AMOUNT;
  public:
    FIREFLIES() {setup();}
    ~FIREFLIES() {}

    void reset() {}
    void next_effect() {}

    FIREFLY firefly[MAX_FIREFLY_AMOUNT]; //create our firefly objects

    void setup() {

      for (int i = 0; i < MAX_FIREFLY_AMOUNT; i++)
        {
          firefly[i].init(); //initialize our fireflies
        }
      control_variables.add(number_of_flies, "Number of fireflies", 0, 255);
    }

    void loop() { //firefly

      LED_show(); //update the LED string
      LED_black();

      for (int i = 0; i < number_of_flies; i++)
        {
          firefly[i].upd(); //update each firefly
        }
      
  
    }
};

LIGHT_SKETCHES::REGISTER<FIREFLIES> fireflies("fireflies");

#endif