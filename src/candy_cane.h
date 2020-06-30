

class CANDY_CANE {

  public:
    CANDY_CANE () {setup();}
    ~CANDY_CANE () {}

  private:
    uint8_t pos = 0;
    CRGB rgb0 = CRGB(0,255,0);
    CRGB rgb1 = CRGB(0,0,255);


  public:

    

    void next_effect() {}

    void reset() {}

    void setup() {
        control_variables.add(rgb0, "Color 1");
        control_variables.add(rgb1, "Color 2");
    }


    void loop() {
        pos-=3;

        for (int i = 0; i < NUM_LEDS; i++) {

            leds[i].b = 0;

            //RED
            uint8_t twist = tree_coords[i].y/96;
            twist += pos;

            
            uint8_t twist_dist = _min(abs(tree_coords[i].x - twist), abs((tree_coords[i].x+256) - twist));
            twist_dist = _min(twist_dist, abs((tree_coords[i].x-256) - twist));
            if (  twist_dist < 48 ) {
                uint16_t inverse_twist_dist = 48-twist_dist;
                inverse_twist_dist = (inverse_twist_dist*inverse_twist_dist)/9;
                leds[i].r = (rgb0.r*inverse_twist_dist)/256;
                leds[i].g = (rgb0.g*inverse_twist_dist)/256;
                leds[i].b = (rgb0.b*inverse_twist_dist)/256;
            }

            //WHITE
            twist += 64;
            // uint8_t twist2 = twist+128;

            // twist_dist = _min(abs(tree_coords[i].x - twist), abs((tree_coords[i].x+256) - twist));
            // twist_dist = _min(twist_dist, abs((tree_coords[i].x-256) - twist));
            // if (  twist_dist < 32 ) {
            //     uint16_t inverse_twist_dist = 32-twist_dist;
            //     inverse_twist_dist = (inverse_twist_dist*inverse_twist_dist)/4;
            //     leds[i].b = inverse_twist_dist;
            // } else {
            //     uint8_t twist_dist2 = _min(abs(tree_coords[i].x - twist2), abs((tree_coords[i].x+256) - twist2));
            //     twist_dist2 = _min(twist_dist2, abs((tree_coords[i].x-256) - twist2));
            //     leds[i].b = 0;
            //     if (  twist_dist2 < 34 ) {
            //         uint16_t inverse_twist_dist2 = 34-twist_dist2;
            //         inverse_twist_dist2 = (inverse_twist_dist2*inverse_twist_dist2)/4;
            //         leds[i].b = inverse_twist_dist2;
            //     } else {
            //         leds[i].b = 0;
            //     }
            // }

            //GREEN
            twist += 64;

            twist_dist = _min(abs(tree_coords[i].x - twist), abs((tree_coords[i].x+256) - twist));
            twist_dist = _min(twist_dist, abs((tree_coords[i].x-256) - twist));
            if (  twist_dist < 48 ) {
                uint16_t inverse_twist_dist = 48-twist_dist;
                inverse_twist_dist = (inverse_twist_dist*inverse_twist_dist)/9;
                leds[i].r = (rgb1.r*inverse_twist_dist)/256;
                leds[i].g = (rgb1.g*inverse_twist_dist)/256;
                leds[i].b = (rgb1.b*inverse_twist_dist)/256;
            }





        }


        // for (int i = 0; i < NUM_LEDS; i++) { 
        //     if (leds[i].b > 0) {
        //         leds[i].g = leds[i].b;
        //         leds[i].r = leds[i].b;
        //     }
        // }

        LED_show();
        LED_black();
    } 
};

LIGHT_SKETCHES::REGISTER<CANDY_CANE> candy_cane("candy cane");