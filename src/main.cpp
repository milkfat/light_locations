int debug_led_position = 0;
//#define DEBUG true
//#define FASTLED_ESP32_I2S true

#define FASTLED_USE_GLOBAL_BRIGHTNESS 1
#include <FastLED.h>

int debug_loop_count = 0;
int debug_light_count = 0;

//#define DISABLE_DEFAULT_SKETCHES


#include "control_variables.h"
//#include "location_branches.h"
//#include "location_window.h"
//#include "location_moon.h"
#include "location_tree.h"
//#include "location_tower.h"

#include "wifi_stuff.h"
#include "udp_stuff.h"
//#include "config.h"
#include <webserver.h>

#include "ota.h"


#include "time.h"
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -3600*8;
const int   daylightOffset_sec = 3600;
//const int   daylightOffset_sec = 0;


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

    ota_setup();

    udp_server.start();

    #ifdef DEBUG
    Serial.println("initializing secure web server");
    #endif
    secureServer_setup();
    #ifdef DEBUG
    Serial.println("secure web server initialized");
    #endif
    vTaskDelay(100); //the main loop seems to get stuck without a bit of delay here


    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
    //check WiFi status and reconnect if necessary
    wifi_check();
    debug_loop_count++;
    static uint32_t loop_sketch_time = 0;
    udp_server.loop();
    insecureServer.loop();
    if (millis() > loop_sketch_time) {

        loop_sketch_time = millis()+16;

        static int ota_blackout = 0;
        if( !handling_ota ) {
            location_loop();
            handle_text();
            ota_blackout = 0;
        } else if (!ota_blackout) {
            LED_black();
            LED_show();
            ota_blackout = 1;
        } else {
            //vTaskDelay(10);
        }
        debug_light_count++;
    }
    //vTaskDelay(1);
} 
