#include "Relay.h"

Relay::Relay(uint8_t pin) {
    this->pin = pin;
    pinMode(this->pin, OUTPUT);
}

void Relay::turnOff() {
    digitalWrite(this->pin, LOW);
}

void Relay::turnOn() {
    digitalWrite(this->pin,HIGH);
}