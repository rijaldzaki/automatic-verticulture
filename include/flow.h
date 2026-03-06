#ifndef __FLOW_H__
#define __FLOW_H__

#include <Arduino.h>
#include "config.h"

volatile int _pulseCount = 0;
void IRAM_ATTR _flowInterrupt() { _pulseCount++; }

void setupFlow() {
    pinMode(PIN_FLOW, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_FLOW), _flowInterrupt, FALLING);
}

float getFlowRate() {
    noInterrupts();
    int pulses = _pulseCount;
    _pulseCount = 0;
    interrupts();

    // Konstanta YF-S201C: F = 7.5 * Q (L/min)
    // If : Fungsi ini dipanggil setiap 1000ms di TaskControl:
    float flowRate = (float)pulses / 7.5; 
    return flowRate;
}
#endif