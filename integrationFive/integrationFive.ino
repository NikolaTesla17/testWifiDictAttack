#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>



//SSID and Password to your ESP Access Point
const char* ssid = "ESPWebServer";
const char* password = "12345678";

AsyncWebServer server(80);

//  Initialize SPIFFS
//  if(!SPIFFS.begin()){
//    //Serial.println("An Error has occurred while mounting SPIFFS");
//    return;
//  }
// 
void setup(void){
  SPIFFS.begin();
  Serial.begin(115200);
  //Serial.println("");
  //WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
 
  Serial.println(WiFi.softAPIP());
 
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("yay");
    request->send(SPIFFS, "/index.html", String(), false);
  });

  server.on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/main.css", String(), false);
  });

  server.on("/generic.html", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/generic.html", String(), false);
  });

  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/index.html", String(), false);
  });

  server.on("/indexMyStuff.html", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/indexMyStuff.html", String(), false);
  });

  server.on("/banner.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/banner.jpg", String(), false);
  });

  server.on("/02.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/02.jpg", String(), false);
  });

  server.on("/03.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/03.jpg", String(), false);
  });

  server.on("/05.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/05.jpg", String(), false);
  });

  server.on("/06.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/06.jpg", String(), false);
  });

  server.on("/07.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/07.jpg", String(), false);
  });

  server.on("/09.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/10.jpg", String(), false);
  });

  server.on("/10.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/10.jpg", String(), false);
  });

  server.on("/faviconPinaple.png", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/faviconPinaple.png", String(), false);
  });

  server.on("/FontAwesome.otf", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/FontAwesome.otf", String(), false);
  });

  server.on("/fontawesome.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/fontawesome.min.css", String(), false);
  });

  server.on("/fontawesomewebfont.woff2", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/fontawesomewebfont.woff2", String(), false);
  });

  server.on("/js/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/js/jquery.min.js", String(), false);
  });
  server.on("/js/jquery.poptrox.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/js/jquery.poptrox.min.js", String(), false);
  });
  server.on("/js/jquery.scrollex.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/js/jquery.scrollex.min.js", String(), false);
  });
  server.on("/js/jquery.scrolly.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/js/jquery.scrolly.min.js", String(), false);
  });
  server.on("/js/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/js/main.js", String(), false);
  });
  server.on("/js/skel.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/js/skel.min.js", String(), false);
  });
  server.on("/js/util.js", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/js/util.js", String(), false);
  });



  
  
  server.begin();                  //Start server
  //Serial.println("HTTP server started");
    Serial.println(WiFi.softAPIP());
}
//===============================================================
//                     LOOP
//===============================================================
void loop(void){
  //server.handleClient();          //Handle client requests
  //Serial.println(WiFi.softAPIP());
}
//#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
//#include <ESP8266WiFiMulti.h>
//#include <ESP8266mDNS.h>
//#include <ESP8266WebServer.h>
//#include <FS.h>   // Include the SPIFFS library
//
//ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
//
//ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
//
//String getContentType(String filename); // convert the file extension to the MIME type
//bool handleFileRead(String path);       // send the right file to the client (if it exists)
//
//void setup() {
//  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
//  delay(10);
//  Serial.println('\n');
//
//  wifiMulti.addAP("KNAM", "Ny17Ash23!");   // add Wi-Fi networks you want to connect to
//  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
//  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");
//
//  Serial.println("Connecting ...");
//  int i = 0;
//  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
//    delay(250);
//    Serial.print('.');
//  }
//  Serial.println('\n');
//  Serial.print("Connected to ");
//  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
//  Serial.print("IP address:\t");
//  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
//
//  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
//    Serial.println("mDNS responder started");
//  } else {
//    Serial.println("Error setting up MDNS responder!");
//  }
//
//  SPIFFS.begin();                           // Start the SPI Flash Files System
//  
//  server.onNotFound([]() {                              // If the client requests any URI
//    if (!handleFileRead(server.uri()))                  // send it if it exists
//      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
//  });
//
//  server.begin();                           // Actually start the server
//  Serial.println("HTTP server started");
//}
//
//void loop(void) {
//  server.handleClient();
//}
//
//String getContentType(String filename) { // convert the file extension to the MIME type
//  if (filename.endsWith(".html")) return "text/html";
//  else if (filename.endsWith(".css")) return "text/css";
//  else if (filename.endsWith(".js")) return "application/javascript";
//  else if (filename.endsWith(".ico")) return "image/x-icon";
//  return "text/plain";
//}
//
//bool handleFileRead(String path) { // send the right file to the client (if it exists)
//  Serial.println("handleFileRead: " + path);
//  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
//  String contentType = getContentType(path);            // Get the MIME type
//  if (SPIFFS.exists(path)) {                            // If the file exists
//    File file = SPIFFS.open(path, "r");                 // Open it
//    size_t sent = server.streamFile(file, contentType); // And send it to the client
//    file.close();                                       // Then close the file again
//    return true;
//  }
//  Serial.println("\tFile Not Found");
//  return false;                                         // If the file doesn't exist, return false
//}
