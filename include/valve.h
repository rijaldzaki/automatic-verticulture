#ifndef __VALVE_H__
#define __VALVE_H__

#include <Arduino.h>
#include "config.h"

void setupSolenoid() {
    pinMode(PIN_SOLENOID, OUTPUT);
    digitalWrite(PIN_SOLENOID, HIGH);
}

void controlSolenoid(bool state) {
    digitalWrite(PIN_SOLENOID, state ? LOW : HIGH); // LOW = ON, HIGH = OFF
}

#endif