#ifndef _H_FUNCTIONS
#define _H_FUNCTIONS

#include "Arduino.h"

int getGasSensorReading();
void saveConfiguration(String ssid, String password);
void resetConfiguration();
String getSavedSSID();
String getSavedPassword();
bool isAppletConfigured();
void restartApplet();
int notifyRiven(String username, String message);

#endif
