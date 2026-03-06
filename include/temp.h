#ifndef __TEMP_H__
#define __TEMP_H__

#include <OneWire.h>
#include <DallasTemperature.h>
#include "config.h"

OneWire _oneWire(PIN_DS18B20);
DallasTemperature _sensors(&_oneWire);

void setupTemp() {
    _sensors.begin();
}

struct TempData { float t1; float t2; };

TempData readTemperature() {
    _sensors.requestTemperatures();
    float temp1 = _sensors.getTempCByIndex(0);
    float temp2 = _sensors.getTempCByIndex(1);

    // sensor error nilainya -127
    if(temp1 == DEVICE_DISCONNECTED_C) temp1 = 0;
    if(temp2 == DEVICE_DISCONNECTED_C) temp2 = 0;

    return { temp1, temp2 };
}

#endif