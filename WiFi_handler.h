
#ifndef WiFi_handler_h
#define WiFi_handler_h
#include <arduino.h>
#include <ESP8266WiFi.h>

extern bool isDebug;

void WiFi_begin(void);
bool WiFi_CreateAP(char* ssid, char* pass);
bool WiFi_ConnectToAP(char* ssid, char* pass);
#endif
