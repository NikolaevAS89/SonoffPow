#include <arduino.h>
#include "WiFi_handler.h"
#include "HTTP_server.h"
#include "ESP8266PowerClass.h"
#include "Relay.h"
#include "Led.h"
#include "Button.h"
#include "EEPROMHandler.h"
#include "UARTHandler.h"

bool isDebug = true;
unsigned long previousMillis = 0;

ESP8266PowerClass* power;
Led* led;
Button *button;
EEPROMHandler *eeprom;
UARTHandler* uart;
Relay* relay;

void setup() {
   
// Последовательный порт для отладки
   uart = new UARTHandler();
   //
   relay = new Relay();
   //WiFi_begin();
// Старт внутреннего WEB-сервера  
   //HTTP_begin( );
   eeprom = new EEPROMHandler();
   eeprom->save();
   
   //power = new ESP8266PowerClass();
   //power->selectMeasureCurrentOrVoltage(VOLTAGE);
   //power->startMeasure();

   led = new Led();
   button = new Button();
   
   button->addListener(buttonClick);
   uart->addDefaultListener(dataReceive);
}

void dataReceive(char code, char* data) {
  led->blinkOnce(0B00000001);
  Serial.print(code);
  Serial.println(data);
}

void buttonClick(BUTTON_CLICK_EVENT event) {
  switch(event) {
    case SB_CLICK:
      relay->turn();
      break;
    case SB_LONG_CLICK:
      led->blinkOnce(0B00000101);
      break;
    default: 
      led->blink(0B11111111); 
      break;
  }
}

void loop() {
  //HTTP_loop( );
  led->loop();
  button->loop();
  uart->loop();
}
