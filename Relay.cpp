#include "Relay.h"

Relay::Relay(uint8_t pin) {
    this->pin = pin;
    this->isOff = true;
    pinMode(this->pin, OUTPUT);
    digitalWrite(this->pin, LOW);
}

void Relay::turn() {
  if(this->isOn()) {
    this->turnOff();
  } else {
    this->turnOn();
  }
}

bool Relay::isOn() {
  return !this->isOff;
}

void Relay::turnOff() {
  this->isOff = true;
   digitalWrite(this->pin, LOW);
}

void Relay::turnOn() {
  this->isOff = false;
    digitalWrite(this->pin,HIGH);
}
