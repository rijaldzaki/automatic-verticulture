#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <Arduino.h>
#include "config.h"

void setupLevel() {
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
}

float getWaterLevelPct() {
    digitalWrite(PIN_TRIG, LOW); delayMicroseconds(2);
    digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    
    long duration = pulseIn(PIN_ECHO, HIGH, 26000); // 26ms timeout
    float distance = duration * 0.034 / 2;
    
    if (distance <= 0) return 0;
    // Rumus persentase: (1 - (jarak_baca / tinggi_wadah)) * 100
    float pct = (1.0 - (distance / TANK_HEIGHT)) * 100.0;
    return constrain(pct, 0.0, 100.0);
}

#endif