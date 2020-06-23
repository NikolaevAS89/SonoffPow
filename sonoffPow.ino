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

void setup() {
   
// Последовательный порт для отладки
   uart = new UARTHandler();
   uart->addListener('b', test);
   //WiFi_begin();
// Старт внутреннего WEB-сервера  
   //HTTP_begin( );
   eeprom = new EEPROMHandler();
   char* t = (char*)eeprom->getWifiStSsid();
   Serial.print(t);
   eeprom->setWifiStSsid("Columbia", 8);
   eeprom->save();
   //power = new ESP8266PowerClass();
   //power->selectMeasureCurrentOrVoltage(VOLTAGE);
   //power->startMeasure();

   led = new Led();
   led->turnOff();

   button = new Button();
   button->addListener(blink);
}

void test(char* data) {
  led->blinkOnce(0B01010101);
  Serial.print(data);
  Serial.print("\n");
}

void blink(BUTTON_CLICK_EVENT event) {
  switch(event) {
    case SB_CLICK: led->blinkOnce(0B00000101); break;
    case SB_LONG_CLICK: led->blink(0B00011011); break;
    default: led->blinkOnce(0B11111111); break;
  }
}

void loop() {
  /*unsigned long currentMillis = millis();
  if((currentMillis - previousMillis) >= 3000) {
      previousMillis = currentMillis;   
      Serial.println(power->getPower());
      Serial.println(power->getCurrent());
      Serial.println(power->getVoltage());
  }*/
  //HTTP_loop( );
  led->loop();
  button->loop();
  uart->loop();
}
