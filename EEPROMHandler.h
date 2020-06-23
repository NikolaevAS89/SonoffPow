#ifndef _USER_EEPROMHandler_H_
#define _USER_EEPROMHandler_H_

#include <EEPROM.h>
#include <arduino.h>

struct SonofPowCofig {
   uint8_t ecsrc;
   uint8_t wifiStSsid[60];
   uint8_t wifiStPass[60];
   uint8_t httpClientUser[60];
   uint8_t httpClientPass[60];
   uint8_t httpServerUser[60];
   uint8_t httpServerPass[60];
};

class EEPROMHandler {
public:
	EEPROMHandler();
	~EEPROMHandler();

	void save();

	uint8_t  isLoaded();

	uint8_t* getWifiStSsid();
	uint8_t* getWifiStPass();

	uint8_t* getServerPass();
	uint8_t* getServerUser();

	uint8_t* getClientUser();
	uint8_t* getClientPass();

	EEPROMHandler* setWifiStSsid(char* ssid, uint8_t length);
	EEPROMHandler* setWifiStPass(char* pass, uint8_t length);

	EEPROMHandler* setServerUser(char* user, uint8_t length);
	EEPROMHandler* setClientUser(char* user, uint8_t length);

	EEPROMHandler* setServerPass(char* pass, uint8_t length);
	EEPROMHandler* setClientPass(char* pass, uint8_t length);
private:
	SonofPowCofig* config;
	bool loaded;
	uint16_t ecsrc();
	void load();
	void byDefault();
};

#endif
