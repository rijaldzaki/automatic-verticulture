#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"

WiFiClient _espClient;
PubSubClient _mqtt(_espClient);

void setupNetwork() {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    _mqtt.setServer(MQTT_BROKER, 1883);
}

void maintainConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
    if (!_mqtt.connected()) {
        if (_mqtt.connect(POT_ID)) {
            Serial.println("Connected to MQTT Broker");
        }
    }
    _mqtt.loop();
}

void sendTelemetry(float t1, float t2, int soil, float level, float flow, bool sol) {
    StaticJsonDocument<256> doc;
    doc["id"] = POT_ID;
    doc["t_air"] = t1; doc["t_amb"] = t2;
    doc["hum"] = soil; doc["lvl"] = level;
    doc["flw"] = flow; doc["vlv"] = sol;

    char buffer[256];
    serializeJson(doc, buffer);
    _mqtt.publish(MQTT_TOPIC, buffer);
}

#endif