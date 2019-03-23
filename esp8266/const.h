#ifndef _H_CONSTANT
#define _H_CONSTANT

#define APPLET_IP_IDENTIFIER          25
#define TIMEOUT_WIFI_CONNECT          20000

#define PIN_MQ_THRESHOLD              A0

#define PIN_LED_RED         D0
#define PIN_LED_GREEN       D1
#define PIN_LED_BLUE        D2
#define PIN_RESET           3

enum MODULE_MODE {
  CONFIGURATION_MODE,
  WORKING_MODE
};

#endif
