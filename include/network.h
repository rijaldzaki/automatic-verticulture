#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"

WiFiClient _espClient;
PubSubClient _mqtt(_espClient);

// topik MQTT: farm/pot/01/sensors
String getTopic(String suffix) {
    return String(TOPIC_PREFIX) + String(POT_NUM) + "/" + suffix;
}

void setupNetwork() {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    _mqtt.setServer(MQTT_BROKER, 1883);
    _mqtt.setKeepAlive(60); // Heartbeat MQTT protokol
}

void maintainConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Reconnecting WiFi...");
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        int retries = 0;
        while (WiFi.status() != WL_CONNECTED && retries < 20) {
            vTaskDelay(pdMS_TO_TICKS(500));
            retries++;
        }
    }

    // Wifi Ready -> MQTT Connect
    if (WiFi.status() == WL_CONNECTED && !_mqtt.connected()) {
        String lwtTopic = getTopic("sensors");
        if (_mqtt.connect(POT_ID, lwtTopic.c_str(), 1, true, "false")) {
            Serial.println("Connected to MQTT Broker");
            _mqtt.publish(lwtTopic.c_str(), "true", true);  // JSON boolean literal
        }
    }
    if (WiFi.status() == WL_CONNECTED) {
        _mqtt.loop();
    }
}

void sendTelemetry(float t1, float t2, int s1, int s2, float level, float flow, bool sol) {
    StaticJsonDocument<512> doc;
    doc["pot_id"] = POT_ID;
    doc["temperature_up"] = round(t1 * 100.0) / 100.0;
    doc["temperature_down"] = round(t2 * 100.0) / 100.0;
    doc["soil_moisture_up"] = s1;
    doc["soil_moisture_down"] = s2;
    doc["water_level"] = level;
    doc["flow_rate"] = flow;
    doc["valve_status"] = sol ? 1 : 0;

    char buffer[512];
    serializeJson(doc, buffer);
    _mqtt.publish(getTopic("sensors").c_str(), buffer);

}

#endif