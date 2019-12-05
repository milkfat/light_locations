int debug_led_position = 0;
//#define DEBUG true
//#define FASTLED_ESP32_I2S true

#include <FastLED.h>

int debug_loop_count = 0;
int debug_light_count = 0;


//#include "location_window.h"
#include "location_tree.h"

#include "wifi_stuff.h"
#include "config.h"
#include "webserver.h"

void setup() { 
     #ifdef DEBUG
     Serial.begin(115200);
     #endif
    long rando = 0;
    long rando2 = 0;
    for (int i = 0; i < 32; i++) {
        rando2 = analogRead(0); //randomness from unconnected analog pin
        rando2 &= 1; //mask all but the first bit of randomness
        rando2 <<= i; //shift the bit to a new position
        rando ^= rando2; //XOR the bit to our random number
    }
    randomSeed(rando);

    location_setup();

    wifi_setup();
    secureServer_setup();
}

void loop() {

    debug_loop_count++;
    static uint32_t sketch_time = 0;
    if (millis() > sketch_time) {
        debug_light_count++;
        sketch_time = millis()+16;

        static int ota_blackout = 0;
        if( !handling_ota ) {

            location_loop();

            ota_blackout = 0;
        } else if (!ota_blackout) {
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i].r = 0;
                leds[i].g = 0;
                leds[i].b = 0;
            }
            FastLED.show();
            ota_blackout = 1;
        } else {
            vTaskDelay(10);
        }
    }
    
} 
