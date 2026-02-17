#ifndef __CONFIG_H__
#define __CONFIG_H__

// Identitas Node
#define POT_ID "POT-001"
#define MQTT_TOPIC "hydro/data"

// Pemetaan Pin (ESP32)
#define PIN_SOIL_1 34
#define PIN_SOIL_2 35
#define PIN_DS18B20 13
#define PIN_FLOW 4
#define PIN_TRIG 12
#define PIN_ECHO 14
#define PIN_SOLENOID 26

// Kalibrasi & Ambang Batas
#define SOIL_DRY 3200      // Nilai ADC saat tanah kering
#define SOIL_WET 1200      // Nilai ADC saat tanah basah
#define TEMP_MAX 30.5      // Derajat Celcius
#define TANK_HEIGHT 30.0   // Tinggi pot/tangki dalam cm
#define LEVEL_SAFETY 90.0  // Mati otomatis jika air 90%

// Jaringan
#define WIFI_SSID "NAMA_WIFI"
#define WIFI_PASS "PASS_WIFI"
#define MQTT_BROKER "192.168.1.100"

#endif