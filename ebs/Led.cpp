#include "Led.h"
#include <Arduino.h>

Led::Led(int pin_) {
  pin = pin_;
  pinMode(pin, OUTPUT);
}

void Led::set_state(bool on) {
  digitalWrite(pin, (on ? 1 : 0));
}