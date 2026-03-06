#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"

WiFiClient _espClient;
PubSubClient _mqtt(_espClient);

// topik MQTT: verticulture/devices/POT-01/data
String getTopic(String suffix) {
    return String(TOPIC_PREFIX) + String(POT_ID) + "/" + suffix;
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
        String lwtTopic = getTopic("status");
        if (_mqtt.connect(POT_ID, lwtTopic.c_str(), 1, true, "OFFLINE")) {
            Serial.println("Connected to MQTT Broker");
            _mqtt.publish(lwtTopic.c_str(), "ONLINE", true);
        }
    }

    if (WiFi.status() == WL_CONNECTED) {
        _mqtt.loop();
    }
}

void sendTelemetry(float t1, float t2, int s1, int s2, float level, float flow, bool sol) {
    StaticJsonDocument<512> doc;
    doc["id"] = POT_ID;
    doc["t_atas"] = round(t1 * 100.0) / 100.0;
    doc["t_bawah"] = round(t2 * 100.0) / 100.0; 
    doc["h_atas"] = s1; 
    doc["h_bawah"] = s2;
    doc["lvl"] = level;
    doc["flw"] = flow;
    doc["vlv"] = sol ? 1 : 0;

    char buffer[512];
    serializeJson(doc, buffer);
    _mqtt.publish(getTopic("data").c_str(), buffer);

}

#endif