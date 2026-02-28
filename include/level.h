#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <Arduino.h>
#include "config.h"

void setupLevel() {
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
}

float getWaterLevelPct() {
    long duration;
    float distance;
    
    digitalWrite(PIN_TRIG, LOW); delayMicroseconds(2);
    digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    
    duration = pulseIn(PIN_ECHO, HIGH, 30000); // Timeout 30ms
    distance = duration * 0.034 / 2;

    // Filter JSN-SR04T: Abaikan pembacaan di bawah 20cm (blind spot) atau di atas tangki
    if (distance < 20 || distance > TANK_HEIGHT + SENSOR_OFFSET) return 0.0;

    // Kalkulasi persentase dengan offset pemasangan
    float actualWaterDist = distance - SENSOR_OFFSET; 
    float pct = (1.0 - (actualWaterDist / TANK_HEIGHT)) * 100.0;
    
    return constrain(pct, 0.0, 100.0);
}
#endif