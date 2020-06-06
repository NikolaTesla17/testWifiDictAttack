#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//#include <DNSServer.h>
//#include <ESP8266mDNS.h>
#include <FS.h>



//SSID and Password to your ESP Access Point
const char* ssid = "nayanCard";
const char* password = "12345678";


ESP8266WebServer server(80);

void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();

String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)

void setup(void){
  
  SPIFFS.begin();
  Serial.begin(115200);
  delay(10);
  //Serial.println("");
  //WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
 
  Serial.println(WiFi.softAPIP());

    server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });
  
  server.begin();
  Serial.println("HTTP server started");
  Serial.println(WiFi.softAPIP());
}
void loop(void){
  server.handleClient();          //Handle client requests
  //Serial.println(WiFi.softAPIP());
}


String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".jpg")) return "image/jpg";
  else if (filename.endsWith(".png")) return "image/png";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}
