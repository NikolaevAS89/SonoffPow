#include <arduino.h>
#include "WiFi_handler.h"
#include "HTTP_server.h"
#include "ESP8266PowerClass.h"
#include "Relay.h"
#include "Led.h"
#include "Button.h"

bool isDebug = true;
unsigned long previousMillis = 0;

ESP8266PowerClass* power;
Led* led;
Button *button;

void setup() {
   
// Последовательный порт для отладки
   Serial.begin(9600);
   WiFi_begin();
// Старт внутреннего WEB-сервера  
   HTTP_begin( );
   
   power = new ESP8266PowerClass();
   power->selectMeasureCurrentOrVoltage(VOLTAGE);
   power->startMeasure();

   led = new Led();
   led->turnOff();

   button = new Button();
   button->addListener(blink);
}

void blink(BUTTON_CLICK_EVENT event) {
  switch(event) {
    SB_CLICK: led->blinkOnce(0B01010101); break;
    SB_LONG_CLICK: led->blinkOnce(0B11110000); break;
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
  HTTP_loop( );
  led->loop();
  button->loop();
}
