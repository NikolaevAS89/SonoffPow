#include "EEPROMHandler.h"

EEPROMHandler::EEPROMHandler() {
    this->config = new SonofPowCofig();
    size_t size  = sizeof(SonofPowCofig);
    EEPROM.begin(size);
    this->load();
    uint8_t ecsrc = this->ecsrc();
    if(ecsrc!=this->config->ecsrc) {
        Serial.write("ecsrc not equal!");
        this->byDefault();
        this->loaded = false;
    } else {
        if(ecsrc>0){
            this->loaded = true;
        } else {
            this->loaded = false;
        }
    }
}

EEPROMHandler::~EEPROMHandler() {
    delete this->config;
}

void EEPROMHandler::save() {
    this->config->ecsrc = this->ecsrc();
    uint8_t* p = (uint8_t*) this->config;
    size_t sz1 = sizeof(SonofPowCofig);
    for( size_t i=0; i<sz1; i++) {
        EEPROM.write(i, *(p + i));
    }
    EEPROM.commit();
}

void EEPROMHandler::load() {
   size_t sz1 = sizeof(SonofPowCofig);
   uint8_t* p = (uint8_t*) this->config;
   for( size_t i=0; i<sz1; i++ ) {
       uint8_t c = EEPROM.read(i);
       *(p + i) = c;
    }
}

void EEPROMHandler::byDefault() {
   size_t sz1 = sizeof(SonofPowCofig);
   uint8_t* p = (uint8_t*) this->config;
   for( size_t i=0; i<sz1; i++ ) {
       *(p + i) = 0;
    }
}

uint16_t EEPROMHandler::ecsrc(void) {
   uint16_t src = 0;
   size_t sz1 = sizeof(SonofPowCofig);
   uint8_t* p = (uint8_t*)this->config;
   uint16_t ecsrc = this->config->ecsrc;
   this->config->ecsrc = 0;
   for( size_t i=0; i<sz1; i++){
    src +=*(p++);
   }
   this->config->ecsrc = ecsrc;
   return src;  
}

uint8_t EEPROMHandler::isLoaded() {
    return this->loaded;
}

uint8_t* EEPROMHandler::getWifiStSsid() {
    return this->config->wifiStSsid;
}

uint8_t* EEPROMHandler::getServerPass() {
    return this->config->httpServerPass;
}

uint8_t* EEPROMHandler::getServerUser() {
    return this->config->httpServerUser;
}

uint8_t* EEPROMHandler::getClientUser() {
    return this->config->httpClientUser;
}

uint8_t* EEPROMHandler::getClientPass() {
    return this->config->httpClientPass;
}

EEPROMHandler* EEPROMHandler::setWifiStSsid(char* ssid, uint8_t length) {
    uint8_t* dest = this->config->wifiStSsid;
    memcpy(dest, ssid, length);
    dest[length] = 0;
    return this;
}

EEPROMHandler* EEPROMHandler::setWifiStPass(char* pass, uint8_t length) {
    uint8_t* dest = this->config->wifiStPass;
    memcpy(dest, pass, length);
    dest[length] = 0;
    return this;
}

EEPROMHandler* EEPROMHandler::setServerUser(char* user, uint8_t length) {
    uint8_t* dest = this->config->httpServerUser;
    memcpy(dest, user, length);
    dest[length] = 0;
    return this;
}

EEPROMHandler* EEPROMHandler::setClientUser(char* user, uint8_t length) {
    uint8_t* dest = this->config->httpClientUser;
    memcpy(dest, user, length);
    dest[length] = 0;
    return this;
}

EEPROMHandler* EEPROMHandler::setServerPass(char* pass, uint8_t length) {
    uint8_t* dest = this->config->httpServerPass;
    memcpy(dest, pass, length);
    dest[length] = 0;
    return this;
}

EEPROMHandler* EEPROMHandler::setClientPass(char* pass, uint8_t length) {
    uint8_t* dest = this->config->httpClientPass;
    memcpy(dest, pass, length);
    dest[length] = 0;
    return this;
}
