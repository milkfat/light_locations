class BUSHES {




    int XY(int x, int y) {
    int new_x = x%20;
    if (y%2) {
        new_x = 9+(10-new_x);
    }
    return (y/2)*20+new_x;
    }

    uint16_t vals[NUM_LEDS];
    int8_t dirs[NUM_LEDS];
    uint8_t hues[NUM_LEDS];
    uint8_t next_hue = 0;



  public:

    
    void setup() {}

    void loop() { //fire
    //uint8_t hue = 160;

    static uint32_t time0 = 0;
    if (millis() > time0) {
        time0 = millis()+16;
        
        static int z = 0;
        z++;
        for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
        }
        for (int y = 0; y < 50; y++) {
        for (int x = 0; x < 10; x++) {
            uint8_t val = inoise8(x*128,y*128,z*2+x*100);
            val = (val*val)/256;
            val = (val*val)/256;
            leds[XY(x,y)] = CHSV(hues[XY(x,y)],255,val);
        }
        }

        for (int y = 0; y < 50; y++) {
        for (int x = 0; x < 10; x++) {
            uint8_t val = inoise8(x*100,y*100,10000+z*10);
            val = (val*val)/256;
            val = (val*val)/256;
            val = (val*val)/256;
            leds[XY(x,y)] += CHSV(hues[XY(x,y)],255,val);
        }
        }

        for (int y = 0; y < 50; y++) {
        for (int x = 0; x < 10; x++) {
            uint8_t val = inoise8(x*100,y*100,20000+z*20);
            val = (val*val)/256;
            val = (val*val)/256;
            val = (val*val)/256;
            leds[XY(x,y)] += CHSV(hues[XY(x,y)],255,val);
        }
        }

        for (int y = 0; y < 50; y++) {
        for (int x = 0; x < 10; x++) {
            uint8_t val = inoise8(x*100,y*100,30000+z*25);
            val = (val*val)/256;
            val = (val*val)/256;
            val = (val*val)/256;
            leds[XY(x,y)] += CHSV(hues[XY(x,y)],255,val);
        }
        }

        for (int y = 0; y < 50; y++) {
        for (int x = 0; x < 10; x++) {
            uint8_t val = inoise8(x*100,y*100,40000+z*30);
            val = (val*val)/256;
            val = (val*val)/256;
            val = (val*val)/256;
            leds[XY(x,y)] += CHSV(hues[XY(x,y)],255,val);
        }
        }
    for (int i= 0; i < NUM_LEDS; i++) {
        leds[i]*=3;
    }
    static int time1 = 0;
    static int pos = 0;
    if (millis() > time1) {
        pos = 0;
        time1 = millis() + 10000;
        next_hue = random(256);
    }
    uint8_t cnt = 0;
    
    while (cnt < 8) {
        if (pos < NUM_LEDS/2) {
        dirs[NUM_LEDS/2+pos] = 1;
        dirs[NUM_LEDS/2-pos] = 1;
        pos++;
        }
        cnt++;
    }
    
    for (int i = 0; i < NUM_LEDS; i++) {
        if (dirs[i] == -1) {
        vals[i] *= .87f;
        } else if (dirs[i] == 1) {
        vals[i] = _min(vals[i]+5000, 65535);
        if (vals[i] == 65535) {
            dirs[i] = -1;
            hues[i] = next_hue;
        }
        }
        
        int val = vals[i]/256;
        val = (val*val)/256;
        CRGB rgb = CHSV(next_hue, 255, 255);
        nblend(leds[i], rgb, vals[i]/256);
    }
        // debug_led_position%=NUM_LEDS;
        // leds[debug_led_position].r = 255;
        // leds[debug_led_position].g = 255;
        // leds[debug_led_position].b = 255;
        FastLED.show();
    }
    } 
};

BUSHES bushes;