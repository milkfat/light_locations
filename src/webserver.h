/**
 * Example for the ESP32 HTTP(S) Webserver
 *
 * IMPORTANT NOTE:
 * To run this script, you need to
 *  1) Enter your WiFi SSID and PSK below this comment
 *  2) Make sure to have certificate data available. You will find a
 *     shell script and instructions to do so in the library folder
 *     under extras/
 *
 * This script will install an HTTPS Server on your ESP32 with the following
 * functionalities:
 *  - Show a chat interface on the root node /
 *  - Use a websocket to allow multiple clients to pass messages to each other
 */
//#define HTTPS_DISABLE_SELFSIGNING 1
#include <sstream>
#include <string>

/** Check if we have multiple cores */

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define HTTPS_LOGLEVEL 2
// Max clients to be connected to the chat
#define MAX_CLIENTS 10

// Includes for the server
#include <HTTPSServer.hpp>
#include <HTTPServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <WebsocketHandler.hpp>

#include "cert.h"
#include "private_key.h"
#include <SPIFFS.h>

// The HTTPS Server comes in a separate namespace. For easier use, include it here.
using namespace httpsserver;

// Create an SSL certificate object from the files included above ...
SSLCert cert = SSLCert(
  server_crt_DER, server_crt_DER_len,
  server_key_DER, server_key_DER_len
);
// ... and create a server based on this certificate.
// The constructor has some optional parameters like the TCP port that should be used
// and the max client count. For simplicity, we use a fixed amount of clients that is bound
// to the max client count.
//HTTPSServer secureServer = HTTPSServer(&cert, 443, MAX_CLIENTS);
HTTPSServer secureServer = HTTPSServer(&cert);
HTTPServer insecureServer = HTTPServer();

// Declare some handler functions for the various URLs on the server
// The signature is always the same for those functions. They get two parameters,
// which are pointers to the request data (read request body, headers, ...) and
// to the response data (write response, set status code, ...)
void handleRoot(HTTPRequest * req, HTTPResponse * res);
void handle404(HTTPRequest * req, HTTPResponse * res);
void handleSVG(HTTPRequest * req, HTTPResponse * res);
void handleLights(HTTPRequest * req, HTTPResponse * res);
void handleDraw(HTTPRequest * req, HTTPResponse * res);
void handleVideo(HTTPRequest * req, HTTPResponse * res);

// We declare a function that will be the entry-point for the task that is going to be
// created.
void serverTask(void *params);

// As websockets are more complex, they need a custom class that is derived from WebsocketHandler
class VidHandler : public WebsocketHandler {
public:
  // This method is called by the webserver to instantiate a new handler for each
  // client that connects to the websocket endpoint
  static WebsocketHandler* create();

  // This method is called when a message arrives
  void onMessage(WebsocketInputStreambuf * input);

  // Handler function on connection close
  void onClose();
};

// As websockets are more complex, they need a custom class that is derived from WebsocketHandler
class socketHandler : public WebsocketHandler {
public:
  // This method is called by the webserver to instantiate a new handler for each
  // client that connects to the websocket endpoint
  static WebsocketHandler* create();

  // This method is called when a message arrives
  void onMessage(WebsocketInputStreambuf * input);

  // Handler function on connection close
  void onClose();
};

// Simple array to store the active clients:
VidHandler* activeClients[MAX_CLIENTS];
socketHandler* activeClients2[MAX_CLIENTS];

void secureServer_setup() {

  // Initialize the slots
  for(int i = 0; i < MAX_CLIENTS; i++) activeClients[i] = nullptr;
  for(int i = 0; i < MAX_CLIENTS; i++) activeClients2[i] = nullptr;
/*
  // For every resource available on the server, we need to create a ResourceNode
  // The ResourceNode links URL and HTTP method to a handler function
  ResourceNode * nodeRoot    = new ResourceNode("/", "GET", &handleRoot);
  ResourceNode * node404     = new ResourceNode("", "GET", &handle404);
  ResourceNode * nodeSVG     = new ResourceNode("/test.svg", "GET", &handleSVG);
  ResourceNode * nodeLights     = new ResourceNode("/lights", "GET", &handleLights);
  ResourceNode * nodeDraw     = new ResourceNode("/draw", "GET", &handleDraw);
  ResourceNode * nodeVideo     = new ResourceNode("/video", "GET", &handleVideo);

  // Add the root node to the server
  secureServer.registerNode(nodeRoot);
  secureServer.setDefaultNode(node404);
  WebsocketNode * vidNode = new WebsocketNode("/vid", &VidHandler::create);
  secureServer.registerNode(nodeSVG);
  secureServer.registerNode(nodeLights);
  secureServer.registerNode(nodeDraw);
  secureServer.registerNode(nodeVideo);

  Serial.println("Starting server...");
  secureServer.start();
  if (secureServer.isRunning()) {
    Serial.print("Server ready. Open the following URL in multiple browser windows to start chatting: https://");
    Serial.println(WiFi.localIP());
  }
*/

    // We pass:
  // serverTask - the function that should be run as separate task
  // "https443" - a name for the task (mainly used for logging)
  // 6144       - stack size in byte. If you want up to four clients, you should
  //              not go below 6kB. If your stack is too small, you will encounter
  //              Panic and stack canary exceptions, usually during the call to
  //              SSL_accept.
  xTaskCreatePinnedToCore(serverTask, "https443", 40144, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  
}






void serverTask(void *params) {
  // In the separate task we first do everything that we would have done in the
  // setup() function, if we would run the server synchronously.

  // Note: The second task has its own stack, so you need to think about where
  // you create the server's resources and how to make sure that the server
  // can access everything it needs to access. Also make sure that concurrent
  // access is no problem in your sketch or implement countermeasures like locks
  // or mutexes.


  // Create nodes
  ResourceNode * nodeRoot    = new ResourceNode("/", "GET", &handleRoot);
  ResourceNode * node404     = new ResourceNode("", "GET", &handle404);
  ResourceNode * nodeSVG     = new ResourceNode("/test.svg", "GET", &handleSVG);
  ResourceNode * nodeLights     = new ResourceNode("/lights", "GET", &handleLights);
  ResourceNode * nodeDraw     = new ResourceNode("/draw", "GET", &handleDraw);
  ResourceNode * nodeVideo     = new ResourceNode("/video", "GET", &handleVideo);
  WebsocketNode * vidNode = new WebsocketNode("/vid", &VidHandler::create);
  WebsocketNode * socketNode = new WebsocketNode("/socket", &socketHandler::create);

  // Add nodes to the server
  secureServer.registerNode(nodeRoot);
  secureServer.setDefaultNode(node404);
  secureServer.registerNode(vidNode);
  secureServer.registerNode(socketNode);
  secureServer.registerNode(nodeSVG);
  secureServer.registerNode(nodeLights);
  secureServer.registerNode(nodeDraw);
  secureServer.registerNode(nodeVideo);


  insecureServer.registerNode(nodeRoot);
  insecureServer.setDefaultNode(node404);
  insecureServer.registerNode(vidNode);
  insecureServer.registerNode(socketNode);
  insecureServer.registerNode(nodeSVG);
  insecureServer.registerNode(nodeLights);
  insecureServer.registerNode(nodeDraw);
  insecureServer.registerNode(nodeVideo);

  SPIFFS.begin();
  secureServer.start();
  insecureServer.start();

  if (secureServer.isRunning() && insecureServer.isRunning()) {

    // "loop()" function of the separate task
    while(true) {
      // This call will let the server do its work
      insecureServer.loop();
      secureServer.loop();
      ArduinoOTA.handle();
    }
  }
}







void handle404(HTTPRequest * req, HTTPResponse * res) {
  // Discard request body, if we received any
  // We do this, as this is the default node and may also server POST/PUT requests
  req->discardRequestBody();

  // Set the response status
  res->setStatusCode(404);
  res->setStatusText("Not Found");

  // Set content type of the response
  res->setHeader("Content-Type", "text/html");

  // Write a tiny HTTP page
  res->println("<!DOCTYPE html>");
  res->println("<html>");
  res->println("<head><title>Not Found</title></head>");
  res->println("<body><h1>404 Not Found</h1><p>The requested resource was not found on this server.</p></body>");
  res->println("</html>");
}











// In the create function of the handler, we create a new Handler and keep track
// of it using the activeClients array
WebsocketHandler * VidHandler::create() {
  VidHandler * handler = new VidHandler();
  for(int i = 0; i < MAX_CLIENTS; i++) {
    if (activeClients[i] == nullptr) {
      activeClients[i] = handler;
      break;
    }
  }
  return handler;
}

// When the websocket is closing, we remove the client from the array
void VidHandler::onClose() {
  for(int i = 0; i < MAX_CLIENTS; i++) {
    if (activeClients[i] == this) {
      activeClients[i] = nullptr;
    }
  }
}

// Finally, passing messages around. If we receive something, we send it to all
// other clients
void VidHandler::onMessage(WebsocketInputStreambuf * inbuf) {
  // Get the input message
  std::ostringstream ss;
  std::string msg;
  ss << inbuf;
  msg = ss.str();
  strcpy((char*)canvas, msg.c_str());

}









// In the create function of the handler, we create a new Handler and keep track
// of it using the activeClients array
WebsocketHandler * socketHandler::create() {
  socketHandler * handler = new socketHandler();
  for(int i = 0; i < MAX_CLIENTS; i++) {
    if (activeClients2[i] == nullptr) {
      activeClients2[i] = handler;
      break;
    }
  }
  return handler;
}

// When the websocket is closing, we remove the client from the array
void socketHandler::onClose() {
  for(int i = 0; i < MAX_CLIENTS; i++) {
    if (activeClients2[i] == this) {
      activeClients2[i] = nullptr;
    }
  }
}

// Finally, passing messages around. If we receive something, we send it to all
// other clients
void socketHandler::onMessage(WebsocketInputStreambuf * inbuf) {
  // Get the input message
  std::ostringstream ss;
  std::string msg;
  ss << inbuf;
  debug_led_position++;
  ss << debug_led_position;
  msg = ss.str();
  //Serial.println(msg.c_str());
  current_light_sketch++;

  // Send it back to every client
  for(int i = 0; i < MAX_CLIENTS; i++) {
    if (activeClients2[i] != nullptr) {
      activeClients2[i]->send(msg, SEND_TYPE_TEXT);
    }
  }

  //strcpy((char*)canvas, msg.c_str());

}









void handleRoot(HTTPRequest * req, HTTPResponse * res) {
    res->setHeader("Content-Type", "text/html");
    char temp[410];
    int sec = millis() / 1000;
    int min = sec / 60;
    int hr = min / 60;

    snprintf(temp, 410,

             "<html>\
<head>\
  <meta http-equiv='refresh' content='5'/>\
  <title>ESP32 Demo</title>\
  <style>\
    body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
  </style>\
</head>\
<body>\
  <h1>Hello from ESP32!</h1>\
  <p>Uptime: %02d:%02d:%02d</p>\
  <p>Loop Count: %d</p>\
  <p>Light Count: %d</p>\
  <img src=\"/test.svg\" />\
  <a href=\"/lights\">LIGHTS!</a>\
  <a href=\"/draw\">DRAW!</a>\
  <a href=\"/video\">VIDJA!</a>\
  <p>%s</p>\
</body>\
</html>",

             hr, min % 60, sec % 60, debug_loop_count, debug_light_count, WiFi.getHostname()
            );
    res->print(temp);

  };







//TEST SVG IMAGE
void handleSVG(HTTPRequest * req, HTTPResponse * res) {
  res->setHeader("Content-Type", "image/svg+xml");
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x += 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";
  res->print(out);
};
































//HANDLE MAIN LIGHT PAGE


void handleLights(HTTPRequest * req, HTTPResponse * res) {
    res->setHeader("Content-Type", "text/html");
    res->print(
      F(
        "\n"
        "<html><head>\n"
        "<title>LIGHTS!</title>"
        "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
        "<style>\n"
        "body { -webkit-appearance: none;font-size:32px;}\n"
        "button,input[type='text'] { -webkit-appearance: none;font-size:32px;width:100% }\n"
        "input[type='range'] { width:100%;transform: scaleY(2); }\n"
        "input[type='checkbox'] {transform: scale(2);}\n"
        "html { touch-action: manipulation;}\n"
        "</style>\n"
        "<script>\n"
        "function sf(sketch,flag) {\n"
        "    var http = new XMLHttpRequest();\n"
        "    http.open('POST', 'setsketch', true);\n"
        "    http.setRequestHeader('Content-type','application/x-www-form-urlencoded');\n"
        "    var params = 'sketch=' + sketch + '&flag=' + flag;\n"
        "    http.send(params);\n"
        "}\n"
        "document.onkeydown = checkKey;\n"
        "\n"
        "function checkKey(e) {\n"
        "\n"
        "    e = e || window.event;\n"
        "\n"
        "    if (e.keyCode == '38') {\n"
        "      amInline(\"dir\",\"up\")\n"
        "    }\n"
        "    else if (e.keyCode == '40') {\n"
        "      amInline(\"dir\",\"down\")\n"
        "    }\n"
        "    else if (e.keyCode == '37') {\n"
        "      amInline(\"dir\",\"left\")\n"
        "    }\n"
        "    else if (e.keyCode == '39') {\n"
        "      amInline(\"dir\",\"right\")\n"
        "    }\n"
        "\n"
        "}\n"
        "</script></head><body>\n"
      )
    );


    //generate buttons
    int array_size = sizeof(config_options)/sizeof(config_options[0]);                                     
    for (int i = 0;i < array_size;i++) {
      if (config_options[i].option_type == BUTTON) {
        res->print("<button onmousedown='amInline(\"");
        res->print(config_options[i].key);
        res->print("\",\"");
        res->print(config_options[i].val);
        res->print("\");'>");
        res->print(config_options[i].name);
        res->print("</button><br />\n");
      }
    }                                

    //generate checkboxes                                   
    for (int i = 0;i < array_size;i++) {
      if (config_options[i].type == BOOL) {
        res->print(config_options[i].name);
        res->print(": <input type='checkbox' name='");
        res->print(config_options[i].key);
        res->print("' id='");
        res->print(config_options[i].key);
        res->print("' onchange='amBoolean(\"");
        res->print(config_options[i].key);
        res->print("\",\"");
        res->print(config_options[i].key);
        res->print("\"); return false;' ");
        if(config_options[i].value() == "1") {
          res->print("CHECKED");
        }
        res->print("><br>\n");
      }
    }
                                      
    //generate ranges                                     
    for (int i = 0;i < array_size;i++) {
      if (config_options[i].option_type == RANGE) {
        res->print(config_options[i].name);
        res->print(": <input type='range' min='");
        res->print(config_options[i].range_min);
        res->print("' max='");
        res->print(config_options[i].range_max);
        res->print("' value='");
        res->print(config_options[i].value());
        res->print("' class='slider' id='");
        res->print(config_options[i].key);
        res->print("' oninput='am(\"");
        res->print(config_options[i].key);
        res->print("\",\"");
        res->print(config_options[i].key);
        res->print("\")'><br>\n");
      }
    }  

    //generate ranges                                     
    for (int i = 0;i < array_size;i++) {
      if (config_options[i].option_type == TEXT) {
        res->print("<textarea cols=24 rows=2 id='");
        res->print(config_options[i].key);
        res->print("' placeholder='type your message' oninput='am(\"");
        res->print(config_options[i].key);
        res->print("\",\"");
        res->print(config_options[i].key);
        res->print("\")'>");
        res->print(config_options[i].value());
        res->print("</textarea>\n");
      }
    }

    //the rest of the page
    res->print(
      F(
        "\n"
            "<p id = 'status'>Not connected</p>\n"
            "<p>Clients connected: <span id = 'clients'>\n"
               "</span></p>\n"
            "<ul id = 'log'></ul>\n"
        "\n"
        "  </article>\n"
        "<script>\n"
                       "connected = document.getElementById(\"connected\");\n"
                       "log = document.getElementById(\"log\");\n"
                       "chat = document.getElementById(\"text\");\n"
                       "form = chat.form;\n"
                       "state = document.getElementById(\"status\");\n"
                       "\n"
                       "if (window.WebSocket === undefined) {\n"
                          "state.innerHTML = \"sockets not supported\";\n"
                          "state.className = \"fail\";\n"
                       "}else {\n"
                           "if (typeof String.prototype.startsWith != \"function\") {\n"
                              "String.prototype.startsWith = function (str) {\n"
                                 "return this.indexOf(str) == 0;\n"
                              "};\n"
                           "}\n"
        
                         "window.addEventListener(\"load\", onLoad, false);\n"
                      "}\n"
        
                       "function onLoad() {\n"
                          "websocket = new WebSocket(((window.location.protocol === \"https:\") ? \"wss://\" : \"ws://\") + window.location.host + \"/socket\");\n"
                          "websocket.onopen = function(evt) { onOpen(evt) };\n"
                          "websocket.onclose = function(evt) { onClose(evt) };\n"
                          "websocket.onmessage = function(evt) { onMessage(evt) };\n"
                          "websocket.onerror = function(evt) { onError(evt) };\n"
                       "}\n"
        
                       "function onOpen(evt) {\n"
                          "state.className = \"success\";\n"
                          "state.innerHTML = \"Connected to server\";\n"
                       "}\n"
        
                       "function onClose(evt) {\n"
                          "state.className = \"fail\";\n"
                          "state.innerHTML = \"Not connected. Attempting to reconnect...\";\n"
                          "connected.innerHTML = \"0\";\n"
                          // Try to reconnect in 5 seconds\n"
                          "websocket = null;\n"
                          "setTimeout(function(){onLoad()}, 500);\n"
                       "}\n"
        
        
                       "function onMessage(evt) {\n"
                          "var message = evt.data;\n"
        
                          "log.innerHTML = '<li class = \"message\">' + \n"
                         "message + \"</li>\" + log.innerHTML;\n"
                         "var elem = document.querySelector('li:nth-child(11)');\n"
                         "if (elem) {\n"
                          "elem.parentNode.removeChild(elem);\n"
                         "}\n"
                         "var json_stuff = JSON.parse(evt.data);\n"
                         "for (x in json_stuff) {\n"
                           "x2 = document.getElementById(x);\n"
                           "if (typeof json_stuff[x] == typeof true){\n"
                             // variable is a boolean
                             "x2.checked = json_stuff[x];\n"
                           "} else if (x == 'clients') {\n"
                             "x2.innerHTML = '';\n"
                             "for (y in json_stuff[x]) {\n"
                             "x2.innerHTML += '<br />' + json_stuff[x][y][0] + '.' + json_stuff[x][y][1] + '.' + json_stuff[x][y][2] + '.' + json_stuff[x][y][3] + ': ' + json_stuff[x][y][4];\n"
                             "}\n"
                           "} else {\n"
                             // variable is not a boolean
                             "x2.value = json_stuff[x];\n"
                           "}\n"
                         "}\n"
                      "}\n"
        
                      "function onError(evt) {\n"
                         "state.className = \"fail\";\n"
                         "state.innerHTML = \"Communication error\";\n"
                      "}\n"
        
                      "function am(jkey,jid) {\n"
                         "var message = '{\"' + jkey + '\":\"' + document.getElementById(jid).value + '\"}';\n"
                         "websocket.send(message);\n"
                      "}\n"
        
                      "function amInline(jkey,jval) {\n"
                         "var message = '{\"' + jkey + '\":\"' + jval + '\"}';\n"
                         "websocket.send(message);\n"
                      "}\n"
        
                      "function amBoolean(jkey,jid) {\n"\
                         "var val = document.getElementById(jid).checked;\n"
                         "var message = '{\"' + jkey + '\":' + val + '}';\n"
                         "websocket.send(message);\n"
                      "}\n"
                   "</script>\n"
        "</body>\n"
        "</html>"
      )
    );
    
  };


  
//END HANDLE MAIN LIGHT PAGE





//HANDLE VIDEO PAGE, stored in the SPIFFS file system thingy

void handleVideo(HTTPRequest * req, HTTPResponse * res) {
    // Check if the file exists
    if (!SPIFFS.exists("/video.html")) {
      // Send "405 Method not allowed" as response
      res->setStatusCode(404);
      res->setStatusText("Not found");
      res->println("404 Not Found");
      return;
    }

    File file = SPIFFS.open("/video.html");

    // Set length
    //res->setHeader("Content-Length", "" + file.size());
    res->setHeader("Content-Type", "text/html");

    // Read the file and write it to the response
    uint8_t buffer[256];
    size_t length = 0;
    do {
      length = file.read(buffer, 256);
      res->write(buffer, length);
    } while (length > 0);

    file.close();
};





//HANDLE DRAWING PAGE, stored in the SPIFFS file system thingy

void handleDraw(HTTPRequest * req, HTTPResponse * res) {
    // Check if the file exists
    if (!SPIFFS.exists("/draw.html")) {
      // Send "405 Method not allowed" as response
      res->setStatusCode(404);
      res->setStatusText("Not found");
      res->println("404 Not Found");
      return;
    }

    File file = SPIFFS.open("/draw.html");

    // Set length
    //res->setHeader("Content-Length", "" + file.size());
    res->setHeader("Content-Type", "text/html");

    // Read the file and write it to the response
    uint8_t buffer[256];
    size_t length = 0;
    do {
      length = file.read(buffer, 256);
      res->write(buffer, length);
    } while (length > 0);

    file.close();
};