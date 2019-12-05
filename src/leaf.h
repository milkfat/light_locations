class LEAF {
    byte stp = 0;
    unsigned int ypos = 0;

    

    class PLANET {
        
        int adjust (int p) {
            int po = p;
            if (po > 52) {
                po+=2;
            }
            if (po > 79) {
                po+=3;
            }
            if (po > 149) {
                po = 349-po;
            }
            if(po >= 0 && po < NUM_LEDS) {
                return po;
            } else {
                return NUM_LEDS;
            }
        }

    public:

        byte center = 72;
        int bri = 255;
        unsigned int finepos = 255U*58;
        int spd = 0;
        byte hue = 160;
        byte sat = 255;
        char dir = 1;
        byte weight = 3;
        
        void upd(byte i, unsigned int ypos){
        if ( (finepos >= 0) && (finepos <= 255U*(NUM_LEDS-6)) ) {
            int pos = finepos/255;
            int pos1 = adjust(pos+1);
            pos = adjust(pos);
            //leds[pos]=CHSV(hue,255,finebri/1000L);
            byte level0 = ease8InOutApprox(ease8InOutApprox(finepos % 255));
            byte level1 = 255-level0;
            CRGB rgb;
            hsv2rgb_rainbow(CHSV(hue,sat,bri), rgb);
            int d = 1;
            if (leds[pos].r > 0) {
                d=2;
            }
            int d2 = 1;
            if (leds[pos1].r > 0) {
                d2=2;
            }
            leds[pos].r = _min((leds[pos].r+((1U*level1*rgb.r)/255))/d,255);
            leds[pos].g = _min((leds[pos].g+((1U*level1*rgb.g)/255))/d,255);
            leds[pos].b = _min((leds[pos].b+((1U*level1*rgb.b)/255))/d,255);
            leds[pos1].r=_min((leds[pos1].r+((1U*level0*rgb.r)/255))/d2,255);
            leds[pos1].g=_min((leds[pos1].g+((1U*level0*rgb.g)/255))/d2,255);
            leds[pos1].b=_min((leds[pos1].b+((1U*level0*rgb.b)/255))/d2,255);
            //unsigned int oldfinepos = finepos;
            spd = (inoise8(pos*5, ypos+pos*10+10000)-60)/(weight/60.0); //moving (like clouds) 
            if (finepos > 255*113) {
                finepos += spd;
            } else {
                finepos -= spd;
            }
            if (bri < 255) {
                bri+=2;
            }
            
            
            
        }
        }
        
    };

    #define NUM_PLANETS 60
    PLANET planets[NUM_PLANETS];
    int current_planet = 0;
    unsigned long leaf_time0 = 0;
    unsigned long leaf_time1 = 0;

    public:

    void setup() { 

    
    for (int i = 0; i < NUM_PLANETS; i++) {
        planets[i].hue = 160;
        //planets[i].hue = random(0,255);
        planets[i].finepos =  0;
        //planets[i].finepos =  random(40*255U,90*255U);
        planets[i].center = 75;
    }
    
    };



    //unsigned long time0 = millis();
    void loop() {

        if (millis() > leaf_time0) {

            //planets[current_planet].finepos = 255U*124;
            planets[current_planet].finepos = random(255U*1,255U*(NUM_LEDS-6));
            planets[current_planet].hue = random(5,40);
            planets[current_planet].bri = 1; //blue
            planets[current_planet].weight = random(60,255);
        
            
            //planets[current_planet].hue = random(0,255); //colors!
            //planets[current_planet].sat = random(155,255); //random saturation
            //planets[current_planet].bri = random(96,255); //random brightness (HSV)
            planets[current_planet].spd = 0;
            leaf_time0 = millis() + random(450);
            current_planet = (current_planet+1) % NUM_PLANETS;
        }

        stp = (stp+1) % 255;
        //if (leaf_time0+100 < millis()) {
            for (int i = 0; i < NUM_PLANETS; i++) {
            planets[i].upd(i,ypos);
            }
            //leaf_time0 = millis();

        if (millis() > leaf_time1) {
            ypos+=2; //speed of brightness change
            //leaf_time1=millis()+30;
            }
            
        FastLED.show(); //update LEDs      
        fadeToBlackBy(leds, NUM_LEDS, 255);
        //}
        
    }

};

LEAF leaf;