#ifndef __SOIL_H__
#define __SOIL_H__

#include <Arduino.h>
#include "config.h"

struct SoilData { int s1; int s2; };

SoilData readSoilMoisture() {
    // Sampling Atas
    long sum1 = 0;
    for(int i = 0; i < 10; i++) { sum1 += analogRead(PIN_SOIL_1); delay(2); }
    int pct1 = map(sum1/10, SOIL_DRY, SOIL_WET, 0, 100);

    // Sampling Bawah
    long sum2 = 0;
    for(int i = 0; i < 10; i++) { sum2 += analogRead(PIN_SOIL_2); delay(2); }
    int pct2 = map(sum2/10, SOIL_DRY, SOIL_WET, 0, 100);

    return { constrain(pct1, 0, 100), constrain(pct2, 0, 100) };
}

#endif