#ifndef __SOIL_H__
#define __SOIL_H__

#include <Arduino.h>
#include "config.h"

// Membaca nilai mentah ADC dan mengembalikan nilai rata-rata
int readSoilAverage() {
    int s1 = analogRead(PIN_SOIL_1);
    int s2 = analogRead(PIN_SOIL_2);
    return (s1 + s2) / 2;
}

#endif