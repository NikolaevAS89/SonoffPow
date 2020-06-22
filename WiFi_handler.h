
#ifndef WiFi_handler_h
#define WiFi_handler_h
#include <arduino.h>
#include <ESP8266WiFi.h>

extern bool isDebug;

typedef struct {
	String vSSID;
	String vRSSI;
	char vEncryptionType;
} SSID;

static SSID WIFI_NOT_FOUND[0];

void WiFi_begin(void);
bool WiFi_CreateAP(char* ssid, char* pass);
bool WiFi_ConnectToAP(char* ssid, char* pass);
SSID* WiFi_list(void);
#endif
