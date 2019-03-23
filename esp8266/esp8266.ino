#include "FS.h"
#include "const.h"
#include "webserver.h"
#include "functions.h"

MODULE_MODE operation_mode;

Webserver server;

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

  if (!isAppletConfigured()) {
    Serial.println("Applet not Configured");
    turnLedOn(255, 0, 0);
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
    turnLedOn(0, 0, 255);
    WiFi.mode(WIFI_STA);
    WiFi.begin(getSavedSSID().c_str(), getSavedPassword().c_str());

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }

    pinMode(PIN_MQ_THRESHOLD, INPUT);

    operation_mode = WORKING_MODE;
    //resetConfiguration();
  }
}

void loop() {
  if (operation_mode == CONFIGURATION_MODE) {
    server.getServerObject()->handleClient();
  }
  else if (operation_mode == WORKING_MODE) {
    turnLedOn(255, 255, 255);
    if (!digitalRead(PIN_MQ_THRESHOLD)) {
      String message =  "*PERIGO*\n\n";
      message += "\n\nO sensor detectou um nível anormal de gás inflamável, ";
      message += "com risco de combustão.";

      notifyRiven("MaXwEllDeN", message);

      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(5000);
    }   
  }
}



