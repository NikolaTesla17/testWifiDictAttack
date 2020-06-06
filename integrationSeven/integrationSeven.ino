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

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

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

//mystuff====================
bool mySiteCleared = false;
const char* ssid = "nayanCard";
const char* password = "12345678";
//==============================

void setup() {
    while(!mySiteCleared)
    {
      AsyncWebServer server(80);
      SPIFFS.begin();
      Serial.begin(115200);
      WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
      Serial.println(WiFi.softAPIP());

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

  server.on("/indexDeauth", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/generic.html", String(), false);
  mySiteCleared = !mySiteCleared;
  });

  server.on("/banner.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/banner.jpg", String(), false);
  });

  server.on("/indexMyStuff.html", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/indexMyStuff.html", String(), false);
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




  
    // for random generator
    randomSeed(os_random());

    // start serial
    Serial.begin(115200);
    Serial.println();

    // start SPIFFS
    prnt(SETUP_MOUNT_SPIFFS);
    bool spiffsError = !SPIFFS.begin();
    prntln(spiffsError ? SETUP_ERROR : SETUP_OK);

    // Start EEPROM
    EEPROMHelper::begin(EEPROM_SIZE);

#ifdef FORMAT_SPIFFS
    prnt(SETUP_FORMAT_SPIFFS);
    SPIFFS.format();
    prntln(SETUP_OK);
#endif // ifdef FORMAT_SPIFFS

#ifdef FORMAT_EEPROM
    prnt(SETUP_FORMAT_EEPROM);
    EEPROMHelper::format(EEPROM_SIZE);
    prntln(SETUP_OK);
#endif // ifdef FORMAT_EEPROM

    // Format SPIFFS when in boot-loop
    if (spiffsError || !EEPROMHelper::checkBootNum(BOOT_COUNTER_ADDR)) {
        prnt(SETUP_FORMAT_SPIFFS);
        SPIFFS.format();
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
