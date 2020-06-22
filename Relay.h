#ifndef _USER_Relay_H_
#define _USER_Relay_H_

#include <arduino.h>

#ifndef
#define PIN_RELAY 12
#endif

class Relay {
    public:
        Relay(uint8_t pin=PIN_RELAY);
        void turnOff();
        void turnOn();
    private:
        uint8_t pin;
};

#endif
