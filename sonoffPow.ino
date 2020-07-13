#include <arduino.h>
#include <ESP8266WebServer.h>
#include "WiFi_handler.h"
#include "ESP8266PowerClass.h"
#include "Relay.h"
#include "Led.h"
#include "Button.h"
#include "EEPROMHandler.h"
#include "UARTHandler.h"
#include "WiFi_handler.h"
#include "jsmn.h"

#define MAXNUMBER_OF_TOKENS 8
#define MAX_TOKEN_LENGTH 64

ESP8266WebServer server(80);


bool isDebug = true;

ESP8266PowerClass* power;
Led* led;
Button *button;
EEPROMHandler *eeprom;
UARTHandler* uart;
Relay* relay;
WiFiHandler* wifiHandler;

jsmn_parser parser;
jsmntok_t tokens[MAXNUMBER_OF_TOKENS];
char keyString[MAX_TOKEN_LENGTH];
char Prev_keyString[MAX_TOKEN_LENGTH];

bool parseAndSetStConfig(char* message, uint8_t type) {  
  jsmn_init(&parser);  
  int resultCode = jsmn_parse(&parser, message, strlen(message), tokens, sizeof(tokens) / sizeof(tokens[0]));
  if(resultCode <=0 ) {
    return false;
  }
  uint16_t length = 0;    
  for (int i = 1; i < resultCode; i++) {    
    jsmntok_t key = tokens[i];
    length = key.end - key.start;
    length = (length<MAX_TOKEN_LENGTH-1) ? length :(MAX_TOKEN_LENGTH-1);
    memcpy(keyString, (char*)(message+key.start), length);
    keyString[length] = '\0';        
    if(strcmp(Prev_keyString, "ssid") == 0) {
      Serial.println(keyString);
      if(type==0) {
        eeprom->setWifiStSsid(keyString, length);
      } else {
        eeprom->setWifiApSsid(keyString, length);
      }
    } else if(strcmp(Prev_keyString, "password") == 0) {
      Serial.println(keyString);
      if(type==0) {
        eeprom->setWifiStPass(keyString, length); 
      } else {
        eeprom->setWifiApPass(keyString, length); 
      }
    }
    strcpy(Prev_keyString, keyString);        
  }
  eeprom->save();
  return true;
}

void httpHandleWiFiConfig(void) {
  if (server.hasArg("plain")== false) {
    server.send(400, "text/plain", "Support only POST request");
  } else {
    const char* message = server.arg("plain").c_str();    
    if(parseAndSetStConfig(message, 0)) {
      server.send(200, "text/plain", "SSID and password for WIFI connection was setted");
    } else {
      server.send(400, "text/plain", "Can't parse request body");
    }
  }
}

void httpHandleAPConfig(void) {
  if (server.hasArg("plain")== false) {
    server.send(400, "text/plain", "Support only POST request");
  } else {
    jsmn_init(&parser);
    const char* message = server.arg("plain").c_str();    
    int resultCode = jsmn_parse(&parser, message, strlen(message), tokens, sizeof(tokens) / sizeof(tokens[0]));
    if(resultCode <=0 ) {
      server.send(400, "text/plain", "Can't parse request body");
    } else {            
      uint16_t length = 0;    
      for (int i = 1; i < resultCode; i++) {    
        jsmntok_t key = tokens[i];
        length = key.end - key.start;
        length = (length<MAX_TOKEN_LENGTH-1) ? length :(MAX_TOKEN_LENGTH-1);
        memcpy(keyString, (char*)(message+key.start), length);
        keyString[length] = '\0';        
        if(strcmp(Prev_keyString, "ssid") == 0) {
          Serial.println(keyString);
          eeprom->setWifiApSsid(keyString, length);
        } else if(strcmp(Prev_keyString, "password") == 0) {
          Serial.println(keyString);
          eeprom->setWifiApPass(keyString, length);          
        }
        strcpy(Prev_keyString, keyString);        
      }
      eeprom->save();
      server.send(200, "text/plain", "SSID and password for WIFI access point was setted");
    }  
  }
}

void httpHandleRestart(void) {
    server.send(200, "text/plain", "Restarting....");
    ESP.restart();  
}

void httpHandleRoot(void) {
  led->blinkOnce(0B10101010);
  if(relay->isOn()) {
    server.send(200, "application/json", "{\"relay\": 1}");
  } else {
    server.send(200, "application/json", "{\"relay\": 0}");
  }
}

void httpHandleRele(void) {
  relay->turn();
  if(relay->isOn()) {
    server.send(200, "application/json", "{\"relay\": 1}");
  } else {
    server.send(200, "application/json", "{\"relay\": 0}");
  }
}

void httpHandleMeasure(void) {
  char str[20];
  String out = "";
  out += "{\"power\":";  
  sprintf(str,"%f", power->getPower());
  out += str;
  out += ",\"current\":";
  sprintf(str,"%f", power->getCurrent());
  out += str;
  out += ",\"voltage\":";
  sprintf(str,"%f", power->getVoltage());
  out += str;
  out += ",\"frequency\":";
  sprintf(str,"%f", power->getCurrFrequency());
  out += str;
  out += "}";
  server.send(200, "application/json", out);
}

void httpNotFound(void) {
  server.send(404, "text/plain", "Not found");
}

void dataReceive(char code, char* data) {
  led->blinkOnce(0B00000001);
  switch(code) {
    case 'r': relay->turn(); break;
    case 'd': 
       Serial.printf("Power:%f", power->getPower());
       Serial.printf("current:%f", power->getCurrent());
       Serial.printf("voltage:%f", power->getVoltage());
       Serial.printf("frequency:%f", power->getCurrFrequency());
       break;
    case 'w': wifiHandler->WiFiSwitchMode(); break;
    case 'p': 
       if(parseAndSetStConfig(data)){
          Serial.print("Access point configured");
       } else {
          Serial.print("Access point not configured");
       }
       break;
    case 's': Serial.print("station ssid@password"); break; //TODO
    case 'k': 
       Serial.print("restart");
       ESP.restart();
       break;
    default: 
      Serial.print(code);
      Serial.println(data);
      break;
  }
}

void buttonClick(BUTTON_CLICK_EVENT event) {
  led->blinkOnce(0B11001001);
  switch(event) {
    case SB_CLICK:
      relay->turn();
      break;
    case SB_LONG_CLICK:
      wifiHandler->WiFiSwitchMode();
      break;
    default: 
      led->blink(0B11111111); 
      break;
  }
}

void setup() {   
// Последовательный порт для отладки
   power = new ESP8266PowerClass();
   power->selectMeasureCurrentOrVoltage(VOLTAGE);
   power->startMeasure();

   uart = new UARTHandler();
   led = new Led();
   button = new Button();
   relay = new Relay();
   
   eeprom = new EEPROMHandler();
   eeprom->byDefault();
   
   wifiHandler = new WiFiHandler(eeprom);
   
   // Поднимаем WEB-сервер  
   server.on ( "/", httpHandleRoot );
   server.on ( "/relay", httpHandleRele);
   server.on ( "/measure", httpHandleMeasure);
   server.on ( "/restart", httpHandleRestart);
   
   server.on ( "/config/wifi", httpHandleWiFiConfig);
   server.on ( "/config/ap", httpHandleAPConfig);
   
   server.onNotFound ( httpNotFound );
    
   button->addListener(buttonClick);
   uart->addDefaultListener(dataReceive);
   
   server.begin();
   Serial.println("HTTP server started on http://192.168.4.1:80");
}

void loop() {
  server.handleClient();
  button->loop();
  uart->loop();
  led->loop();
}
