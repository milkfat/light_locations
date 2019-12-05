

class CANDY_CANE {

    uint8_t pos = 0;

  public:

    
    void setup() {}


    void loop() {
        pos-=3;

        for (int i = 0; i < NUM_LEDS; i++) {

            leds[i].b = 0;

            uint8_t twist = tree_coords[i].y/96;
            twist += pos;

            
            uint8_t twist_dist = _min(abs(tree_coords[i].x - twist), abs((tree_coords[i].x+256) - twist));
            twist_dist = _min(twist_dist, abs((tree_coords[i].x-256) - twist));
            if (  twist_dist < 48 ) {
                uint16_t inverse_twist_dist = 48-twist_dist;
                inverse_twist_dist = (inverse_twist_dist*inverse_twist_dist)/9;
                leds[i].r = inverse_twist_dist;
            } else {
                leds[i].r = 0;
            }

            twist += 128;

            twist_dist = _min(abs(tree_coords[i].x - twist), abs((tree_coords[i].x+256) - twist));
            twist_dist = _min(twist_dist, abs((tree_coords[i].x-256) - twist));
            if (  twist_dist < 48 ) {
                uint16_t inverse_twist_dist = 48-twist_dist;
                inverse_twist_dist = (inverse_twist_dist*inverse_twist_dist)/9;
                leds[i].g = inverse_twist_dist;
            } else {
                leds[i].g = 0;
            }



        }

        FastLED.show();
    } 
};

CANDY_CANE candy_cane;