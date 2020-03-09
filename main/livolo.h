/*
  Morse.h - Library for Livolo wireless switches.
  Created by Sergey Chernov, October 25, 2013.
  Released into the public domain.
*/

#ifndef Livolo_h
#define Livolo_h

#include "driver/gpio.h"

typedef unsigned char byte;

class Livolo {
public:
    explicit Livolo(gpio_num_t pin);

    void sendButton(unsigned int remoteID, byte keycode);

private:
    gpio_num_t txPin;
    byte i; // just a counter
    byte pulse; // counter for command repeat
    bool high; // pulse "sign"
    void selectPulse(byte inBit);

    void sendPulse(byte txPulse);
};

#endif