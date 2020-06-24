#ifndef _USER_Relay_H_
#define _USER_Relay_H_

#include <arduino.h>

#ifndef PIN_RELAY
#define PIN_RELAY 12
#endif

class Relay {
    public:
        Relay(uint8_t pin=PIN_RELAY);
        bool isOn();
        void turn();
        void turnOff();
        void turnOn();
    private:
        uint8_t pin;
        bool isOff;
};

#endif
