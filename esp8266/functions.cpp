#include "functions.h"

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "FS.h"
#include "const.h"

void turnLedOn(int red, int green, int blue)
{
  analogWrite(PIN_LED_RED, red);
  analogWrite(PIN_LED_GREEN, green);
  analogWrite(PIN_LED_BLUE, blue);
}

int notifyRiven(String username, String message)
{
    HTTPClient http;
    http.begin("http://maxwellden.pythonanywhere.com/api/v1.0/chatbot/send_message");
    http.addHeader("Content-Type", "application/json");
    
    StaticJsonBuffer<1024> jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    
    json["username"] = username;
    json["message"] = message;

    String content;

    json.printTo(content);

    int httpCode = http.POST(content);
    http.end();

    return httpCode;
}

void saveConfiguration(String ssid, String password)
{
    StaticJsonBuffer<1024> jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    
    json["ssid"] = ssid;
    json["password"] = password;
    
    File configFile = SPIFFS.open("/config.json", "w");

    json.printTo(configFile);
    configFile.close();

    return;
}

void resetConfiguration()
{        
    saveConfiguration("", "");
    return;
}

String getSavedSSID()
{
    File configFile = SPIFFS.open("/config.json", "r");

    if (!configFile) {
        resetConfiguration();
        configFile = SPIFFS.open("/config.json", "r");
    }

    size_t size = configFile.size();    

    std::unique_ptr<char[]> buf(new char[size]);

    configFile.readBytes(buf.get(), size);
    
    StaticJsonBuffer<1024> jsonBuffer;    
    JsonObject& json = jsonBuffer.parseObject(buf.get());
    configFile.close();

    return json["ssid"];
}

String getSavedPassword()
{
    File configFile = SPIFFS.open("/config.json", "r");

    if (!configFile) {
        resetConfiguration();
        configFile = SPIFFS.open("/config.json", "r");
    }

    size_t size = configFile.size();    

    std::unique_ptr<char[]> buf(new char[size]);

    configFile.readBytes(buf.get(), size);
    
    StaticJsonBuffer<1024> jsonBuffer;    
    JsonObject& json = jsonBuffer.parseObject(buf.get());
    configFile.close();
    return json["password"];
}

bool isAppletConfigured()
{
    return getSavedSSID() != "";
}

void restartApplet()
{    
    optimistic_yield(500);
    delay(1000);
    ESP.restart();
}
