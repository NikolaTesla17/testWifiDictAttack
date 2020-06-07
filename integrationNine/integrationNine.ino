/*
   ===========================================
      Copyright (c) 2018 Stefan Kremser
             github.com/spacehuhn
   ===========================================
 */

extern "C" {
    // Please follow this tutorial:
    // https://github.com/spacehuhn/esp8266_deauther/wiki/Installation#compiling-using-arduino-ide
    // And be sure to have the right board selected
  #include "user_interface.h"
}

#include "EEPROMHelper.h"

#include <ArduinoJson.h>
#if ARDUINOJSON_VERSION_MAJOR != 5
// The software was build using ArduinoJson v5.x
// version 6 is still in beta at the time of writing
// go to tools -> manage libraries, search for ArduinoJSON and install version 5
#error Please upgrade/downgrade ArduinoJSON library to version 5!
#endif // if ARDUINOJSON_VERSION_MAJOR != 5

#include "oui.h"
#include "language.h"
#include "functions.h"
#include "Settings.h"
#include "Names.h"
#include "SSIDs.h"
#include "Scan.h"
#include "Attack.h"
#include "CLI.h"
#include "DisplayUI.h"
#include "A_config.h"
#include "webfiles.h"

#include "LED.h"

//my includes================================================
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

const char* ssidME = "nayanCard";
const char* passwordME = "12345678";

ESP8266WebServer server(80);

String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)

bool myWeb = true;
//my includes================================================

// Run-Time Variables //
LED led;
Settings settings;
Names    names;
SSIDs    ssids;
Accesspoints accesspoints;
Stations     stations;
Scan   scan;
Attack attack;
CLI    cli;
DisplayUI displayUI;

#include "wifi.h"

uint32_t autosaveTime = 0;
uint32_t currentTime  = 0;

bool booted = false;

void setup() {
     // start serial
    Serial.begin(115200);
    Serial.println();

    // start SPIFFS
    prnt(SETUP_MOUNT_SPIFFS);
    bool spiffsError = !SPIFFS.begin();
    prntln(spiffsError ? SETUP_ERROR : SETUP_OK);


    //my AP may need fixing(restate to their AP)===================================================
    WiFi.softAP(ssidME, passwordME); 
    Serial.println(WiFi.softAPIP());
    //WiFi.softAP(ssidME, passwordME);  //Start HOTspot removing password will disable security

    server.onNotFound([]() {                              // If the client requests any URI
     if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
      Serial.println(server.uri());
    });

     server.on(String(F("/indexDeauth.html")).c_str(), HTTP_GET, [] () {
            //sendProgmem(indexhtml, sizeof(indexhtml), W_HTML);
            myWeb = false;
            Serial.println("deauth accessed");
            Serial.println(myWeb);
     });
  
  server.begin();
  Serial.println("HTTP server started");
  Serial.println(WiFi.softAPIP());


    while(myWeb)
    {
      //Serial.println("inLoop");
        server.handleClient();          //Handle client requests
    }
          Serial.println("OutLoop");
          WiFi.softAPdisconnect (true);
          delay(2000);
//          SPIFFS.end();
//          delay(200);
    //=================================================================
    
    // for random generator
    randomSeed(os_random());


    // Start EEPROM
    EEPROMHelper::begin(EEPROM_SIZE);

#ifdef FORMAT_SPIFFS
    prnt(SETUP_FORMAT_SPIFFS);
    //SPIFFS.format();
    prntln(SETUP_OK);
    prntln("the joke is, spiffs doesnt actually get formated");
#endif // ifdef FORMAT_SPIFFS

#ifdef FORMAT_EEPROM
    prnt(SETUP_FORMAT_EEPROM);
    EEPROMHelper::format(EEPROM_SIZE);
    prntln(SETUP_OK);
#endif // ifdef FORMAT_EEPROM

    // Format SPIFFS when in boot-loop
    if (spiffsError || !EEPROMHelper::checkBootNum(BOOT_COUNTER_ADDR)) {
        prnt(SETUP_FORMAT_SPIFFS);
        //SPIFFS.format();
        prntln("WE  ARE  BOOTLOOPING");
        prntln(SETUP_OK);

        prnt(SETUP_FORMAT_EEPROM);
        EEPROMHelper::format(EEPROM_SIZE);
        prntln(SETUP_OK);

        EEPROMHelper::resetBootNum(BOOT_COUNTER_ADDR);
    }

    // get time
    currentTime = millis();

    // load settings
    #ifndef RESET_SETTINGS
    settings.load();
    #else // ifndef RESET_SETTINGS
    settings.reset();
    settings.save();
    #endif // ifndef RESET_SETTINGS

    // set mac address
    wifi_set_macaddr(STATION_IF, (uint8_t*)settings.getWifiSettings().mac_st);
    wifi_set_macaddr(SOFTAP_IF, (uint8_t*)settings.getWifiSettings().mac_ap);



    // start WiFi
    WiFi.mode(WIFI_OFF);
    wifi_set_opmode(STATION_MODE);
    wifi_set_promiscuous_rx_cb([](uint8_t* buf, uint16_t len) {
      Serial.println("wifi fo brr==================================");
        scan.sniffer(buf, len);
    });

    // start display
    if (settings.getDisplaySettings().enabled) {
        displayUI.setup();
        displayUI.mode = displayUI.DISPLAY_MODE::INTRO;
    }

    // copy web files to SPIFFS
    copyWebFiles(false);

    // load everything else
    names.load();
    ssids.load();
    cli.load();

    // create scan.json
    scan.setup();

    // set channel
    setWifiChannel(settings.getWifiSettings().channel);

    // load Wifi settings: SSID, password,...
    loadWifiConfigDefaults();

    // dis/enable serial command interface
    if (settings.getCLISettings().enabled) {
        cli.enable();
    } else {
        prntln(SETUP_SERIAL_WARNING);
        Serial.flush();
        Serial.end();
    }

    // start access point/web interface
    if (settings.getWebSettings().enabled) startAP();

    // STARTED
    prntln(SETUP_STARTED);

    // version
    prntln(DEAUTHER_VERSION);

    // setup LED
    led.setup();
}

void loop() {
    currentTime = millis();

    led.update();    // update LED color
    wifiUpdate();    // manage access point
    attack.update(); // run attacks
    displayUI.update();
    cli.update();    // read and run serial input
    scan.update();   // run scan
    ssids.update();  // run random mode, if enabled

    // auto-save
    if (settings.getAutosaveSettings().enabled
        && (currentTime - autosaveTime > settings.getAutosaveSettings().time)) {
        autosaveTime = currentTime;
        names.save(false);
        ssids.save(false);
        settings.save(false);
    }

    if (!booted) {
        booted = true;
        EEPROMHelper::resetBootNum(BOOT_COUNTER_ADDR);
#ifdef HIGHLIGHT_LED
        displayUI.setupLED();
#endif // ifdef HIGHLIGHT_LED
    }
}

//String getContentType(String filename) { // convert the file extension to the MIME type
//  if (filename("indexDeauth.html")) myWeb = !myWeb;//may need to create a file name indexDeauth.html==================================================
//  else if (filename.endsWith(".html")) return "text/html";
//  else if (filename.endsWith(".css")) return "text/css";
//  else if (filename.endsWith(".js")) return "application/javascript";
//  else if (filename.endsWith(".ico")) return "image/x-icon";
//  else if (filename.endsWith(".jpg")) return "image/jpg";
//  else if (filename.endsWith(".png")) return "image/png";
//  return "text/plain";
//}

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
