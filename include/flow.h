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
    // Rumus debit YF-S201C: $Q = \frac{Pulsa}{7.5}$ (Liter/menit)
    float rate = (_pulseCount / 7.5);
    _pulseCount = 0; // Reset untuk pengukuran berikutnya
    return rate;
}

#endif