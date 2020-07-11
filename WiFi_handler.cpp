#include "WiFi_handler.h"

WiFiHandler::WiFiHandler(EEPROMHandler* eeprom) {
    this->eeprom = eeprom;
    if(this->WiFiSwitchMode()) {
        Serial.print("Connected\n");
    } else {
        Serial.print("Connect fail\n");
    }
}

WiFiHandler::~WiFiHandler(void) {
  this->WiFiShutdown();
}

bool WiFiHandler::WiFiSwitchMode(void) {
    if(isDebug) {
        Serial.print("WiFiSwitchMode()\n");
    }
    this->WiFiShutdown();
    if(WiFi.getMode() == WIFI_AP) {
        if( this->WiFiConnectToAP() ) {
            Serial.print("Connected to access point\n");
            return true;
        }
    }
    Serial.print("Try create access point\n");
    return this->WiFiCreateAP();
}

bool WiFiHandler::WiFiShutdown(void) {
    if(isDebug){
      Serial.print("WiFiShutdown(void)\n");
    }
    for ( int j = 0; j < WIFI_CNT_TRY; j++ ) {
        if(WiFi.getMode() == WIFI_STA) {
            Serial.print("Try disconnect as station\n");
            if(WiFi.disconnect(true)) {
                return true;
            }
        } else {
            Serial.print("Try disconnect as access point\n");
            if(WiFi.softAPdisconnect(true)) {
                return true;
            }
        }
        delay(WIFI_DELAY_TRY);
    }
    return false;
}

bool WiFiHandler::WiFiCreateAP(void) {
  Serial.print("WiFiCreateAP(void)\n");
  char* ssid = (char*)this->eeprom->getWifiAPSsid();
  char* pass = (char*)this->eeprom->getWifiAPPass();
  Serial.print(ssid);
  Serial.print("\n");
  Serial.print(pass);
  Serial.print("\n");
  IPAddress localIP(192,168,1,1); //TODO
  IPAddress gateway(192,168,0,1); //TODO
  IPAddress subnet(255,255,255,0); //TODO
  WiFi.mode(WIFI_AP);
  for ( int j = 0; j < 0; j++ ) {
      delay(WIFI_DELAY_TRY);
      if( WiFi.softAPConfig(localIP, gateway, subnet) ) {
        Serial.print("Configuration was setted\n");
        break;
      } else {
        Serial.print("Configuration not setted\n");
      }
  }
  return WiFi.softAP(ssid, pass);
}

bool WiFiHandler::WiFiConnectToAP(void) {
    Serial.print("WiFiConnectToAP(void)\n");
  if(!this->eeprom->isWifiStValid()) {
    Serial.print("Not valid configuration\n");
    return false;
  }
  char* ssid = (char*)this->eeprom->getWifiStSsid();
  char* pass = (char*)this->eeprom->getWifiStPass();
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, pass, 0);
  for ( int j = 0; j < WIFI_CNT_TRY; j++ ) {
    delay(WIFI_DELAY_TRY);
    if (WiFi.status() == WL_CONNECTED) {
      WiFi.printDiag(Serial);
      return true;
    }
    Serial.print(WiFi.status());
  }
  Serial.print("Connect WiFi failed ...\n");
  return false;
}
