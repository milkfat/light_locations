
class HALLOWEEN {
    unsigned char myArray[NUM_LEDS][3]; //create array to store HSV color

    unsigned long time0 = millis();
    int pos0 = 0;
    int dir0 = 1;
    unsigned long time1 = millis();
    int pos1 = 10;
    int dir1 = 1;
    unsigned long time2 = millis();
    int pos2 = 20;
    int dir2 = -1;
    unsigned long time3 = millis();
    int pos3 = 0;
    
    public:
    
    void setup() { 
    }

    void loop() {
        //ORANGE
        if ((millis() - time0) > 50)
            {
            pos0 = (pos0+dir0+NUM_LEDS)%NUM_LEDS; //update the position according to chase direction
            time0 = millis();

            //try to make them bounce
                if ((abs(pos0-pos1)<2) || (abs(pos0-pos2)<2) )// || (pos0 == 0) || (pos0 == (NUM_LEDS-1)))
                {
                    dir0 = -dir0;
                }
            }
            else
            {
            //leds[i].setHSV(18,255,255);
            myArray[pos0][0] = 16;  // H orange
            //myArray[pos0][0] = 171;  // H blue
            myArray[pos0][1] = 255; // S
            myArray[pos0][2] = 255; // V;
            }

        //GREEN
        if ((millis() - time1) > 30)
            {
            pos1 = (pos1+dir1+NUM_LEDS)%NUM_LEDS; //update the position according to chase direction
            time1 = millis();

            //try to make them bounce
                if ((abs(pos1-pos0)<2) || (abs(pos1-pos2)<2) )// || (pos1 == 0) || (pos1 == (NUM_LEDS-1)))
                {
                    dir1 = -dir1;
                }
            }
            else
            {
            //leds[i].setHSV(96,255,255);
            myArray[pos1][0] = 96;  // H
            myArray[pos1][1] = 255; // S
            myArray[pos1][2] = 255; // V;
            }

        //PURPLE
        if ((millis() - time2) > 40)
            {
            pos2 = (pos2+dir2+NUM_LEDS)%NUM_LEDS; //update the position according to chase direction
            time2 = millis();

            //try to make them bounce
                if ((abs(pos2-pos0)<2) || (abs(pos2-pos1)<2) )// || (pos2 == 0) || (pos2 == (NUM_LEDS-1)))
                {
                    dir2 = -dir2;
                }
            }
            else
            {
            //leds[i].setHSV(192,255,255);
            myArray[pos2][0] = 192;  // H purple
            myArray[pos2][1] = 255; // S
            myArray[pos2][2] = 255; // V;
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
            

            FastLED.show(); //update LEDs


            
            //fadeToBlackBy(leds, NUM_LEDS, 2); //fade the entire array by a small percentage on each pass
            delay(2);
    } 
};

HALLOWEEN halloween;