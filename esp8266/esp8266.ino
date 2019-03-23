#include "FS.h"
#include "const.h"
#include "webserver.h"
#include "functions.h"

MODULE_MODE operation_mode;

Webserver server;
IPAddress ip;

void setup() {
  Serial.begin(115200);
  Serial.println("============");
  Serial.println("Setup");
  delay(10);

  SPIFFS.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // LED
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);
  turnLedOn(0, 0, 0);

  if (!isAppletConfigured()) {
    Serial.println("Applet not Configured");
    operation_mode = CONFIGURATION_MODE;
    WiFi.mode(WIFI_AP_STA);

    IPAddress ap_ip(18, 13, 0, APPLET_IP_IDENTIFIER);
    WiFi.softAPConfig(ap_ip, IPAddress(18, 13, 0, 0), IPAddress(255, 255, 255, 0));
    WiFi.softAP("MusicBox");

    Serial.println("MusicBox 18.13.0.25");
    server.begin();
  }
  else {
    Serial.print("Applet Configured!!!");
    WiFi.mode(WIFI_STA);
    WiFi.begin(getSavedSSID().c_str(), getSavedPassword().c_str());

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      turnLedOn(0, 0, 255);
      delay(200);
      turnLedOn(0, 0, 0);
      delay(200);
    }
    
    server.begin();

    ip = WiFi.localIP();

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(getSavedSSID());
    Serial.print("IP address: ");
    Serial.println(ip);
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());

    operation_mode = WORKING_MODE;
    //resetConfiguration();
  }
}

void loop() {
  if (operation_mode == CONFIGURATION_MODE) {
    turnLedOn(0, 0, 255);
    server.getServerObject()->handleClient();
  }
  else if (operation_mode == WORKING_MODE) {
    turnLedOn(255, 255, 255);
    server.getServerObject()->handleClient();
  }
}

