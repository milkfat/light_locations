//OTA (Over the Air programming)

 const char* ssid     = "moonraker";
 const char* password = "pastrylikesyou";
//const char* ssid     = "Matthew’s iPhone";
//const char* password = "appleapp";

#include <ESPmDNS.h>
//#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Wifi.h>

volatile int handling_ota = 0;

void wifi_setup() {

    //scan for available WiFi networks
    int numberOfNetworks = WiFi.scanNetworks();
    #ifdef DEBUG
    Serial.print("Number of networks found: ");
    Serial.println(numberOfNetworks);
    #endif


    //Check to see if our ssid exists. If so, then connect.
    for (int i = 0; i < numberOfNetworks; i++) {
        #ifdef DEBUG
        Serial.println(WiFi.SSID(i));
            #endif
            if(WiFi.SSID(i) == ssid) {
            char hostname[] = LIGHTS_HOSTNAME;
            WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
            WiFi.setHostname(hostname);
            WiFi.mode(WIFI_STA);
            WiFi.setSleep(false); //disable power saving mode to speed up response times
            //dnsServer.start(DNS_PORT, "*", apIP);
            WiFi.begin(ssid, password);
            #ifdef DEBUG
            Serial.println("CONNECTING TO MOONRAKER!!!");
            #endif
            while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                Serial.print(".");
            }

            #ifdef DEBUG
            Serial.println("");
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
            #endif


            // if (!MDNS.begin("window")) {
            //     #ifdef DEBUG
            //     Serial.println("Error setting up MDNS responder!");
            //     #endif
            //     while(1){
            //         delay(1000);
            //     }
            // }
            // // Add service to MDNS-SD
            // //mdns_hostname_set("window");
            // mdns_service_add("window", "_https", "_tcp", 443, NULL, 0);

        
            break;
        }
    }

    WiFi.scanDelete();

    
    ArduinoOTA
        .onStart([]() {
        handling_ota = 1;
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else // U_SPIFFS
            type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        #ifdef DEBUG
        Serial.println("Start updating " + type);
        #endif
        })
        .onEnd([]() {
        handling_ota = 0;
        #ifdef DEBUG
        Serial.println("\nEnd");
        #endif
        })
        .onProgress([](unsigned int progress, unsigned int total) {
        #ifdef DEBUG
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        #endif
        })
        .onError([](ota_error_t error) {
        handling_ota = 0;
        #ifdef DEBUG
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
        #endif
        });

    ArduinoOTA.begin();
    #ifdef DEBUG
    Serial.println("OTA Ready");
    #endif
}
