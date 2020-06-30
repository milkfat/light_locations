
 const char* ssid     = "moonraker";
 const char* password = "pastrylikesyou";
//const char* ssid     = "Matthew’s iPhone";
//const char* password = "appleapp";

#include <ESPmDNS.h>
//#include <WiFiUdp.h>
#include <Wifi.h>

volatile int handling_ota = 0;

void wifi_check() {
  if ( WiFi.status() !=  WL_CONNECTED )
  {
    // wifi down, reconnect here
    WiFi.begin();
    int WLcount = 0;
    int UpCount = 0;
    while (WiFi.status() != WL_CONNECTED && WLcount < 200 ) 
    {
        delay( 100 );
        Serial.printf(".");
        if (UpCount >= 60)  // just keep terminal from scrolling sideways
        {
            UpCount = 0;
            Serial.printf("\n");
        }
        ++UpCount;
        ++WLcount;
    }
  }
}

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

}
