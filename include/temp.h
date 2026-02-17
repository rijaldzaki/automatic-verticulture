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
    return { _sensors.getTempCByIndex(0), _sensors.getTempCByIndex(1) };
}

#endif