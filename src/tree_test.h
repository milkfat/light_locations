

class CANDY_CANE {

    uint8_t pos = 0;

  public:

    
    void setup() {}

    void loop() {
        pos++;

        for (int i = 0; i < NUM_LEDS; i++) {

            if (  abs(tree_coords[i].x - pos) < 32 ) {
                leds[i].r = 64;
            } else {
                leds[i].r = 0;
            }


            if (  abs(tree_coords[i].y - pos*256) < 256*10 ) {
                leds[i].g = 64;
            } else {
                leds[i].g = 0;
            }


            if (  abs(tree_coords[i].z - pos) < 32 ) {
                leds[i].b = 64;
            } else {
                leds[i].b = 0;
            }


        }

        FastLED.show();
    } 
};

CANDY_CANE candy_cane;