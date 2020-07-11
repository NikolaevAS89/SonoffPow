#include <arduino.h>
#include "WiFi_handler.h"
#include "HTTP_server.h"
#include "ESP8266PowerClass.h"
#include "Relay.h"
#include "Led.h"
#include "Button.h"
#include "EEPROMHandler.h"
#include "UARTHandler.h"
#include "WiFi_handler.h"

bool isDebug = true;

ESP8266PowerClass* power;
Led* led;
Button *button;
EEPROMHandler *eeprom;
UARTHandler* uart;
Relay* relay;
WiFiHandler* wifiHandler;

void setup() {   
// Последовательный порт для отладки
   uart = new UARTHandler();
   led = new Led();
   button = new Button();
   relay = new Relay();
   eeprom = new EEPROMHandler();
   wifiHandler = new WiFiHandler(eeprom);
   
// Старт внутреннего WEB-сервера  
   //HTTP_begin( );
   
   //power = new ESP8266PowerClass();
   //power->selectMeasureCurrentOrVoltage(VOLTAGE);
   //power->startMeasure();

   
   button->addListener(buttonClick);
   uart->addDefaultListener(dataReceive);
}

void dataReceive(char code, char* data) {
  led->blinkOnce(0B00000001);
  switch(code) {
    case 'r': relay->turn(); break;
    case 'd': Serial.print("print data"); break;
    case 'w': wifiHandler->WiFiSwitchMode(); break;
    case 'u': Serial.print("access point ssid@password"); break;
    case 's': Serial.print("station ssid@password"); break;
    case 'k': Serial.print("restart"); break;
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

void loop() {
  //HTTP_loop( );
  button->loop();
  uart->loop();
  led->loop();
}
