#ifndef CONFIG_H
#define CONFIG_H

#define MICROSWITCH_PIN 4
#define SSR_CONTROL_PIN 13

const char* WIFI_SSID = "TheForce2.0";
const char* WIFI_PWD = "qTEr5123arck";
const bool PREINFUSION_ENABLED_DEFAULT = true;
const unsigned long PREINFUSION_DURATION_DEFAULT = 10000;
const unsigned long FILL_PULSE_DURATION_DEFAULT = 2000;
const unsigned long EXTRACTION_DURATION_DEFAULT = 25000;
const int WEB_SERVER_PORT = 80;

#endif