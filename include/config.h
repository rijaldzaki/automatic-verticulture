#ifndef __CONFIG_H__
#define __CONFIG_H__

// Ubah ID
#define POT_ID "POT-04" 
#define TOPIC_PREFIX "verticulture/devices/"

// Pemetaan Pin (Tetap sama)
#define PIN_SOIL_1 33 // (atas) ADC6
#define PIN_SOIL_2 32  // (bawah) ADC7
#define PIN_DS18B20 26
#define PIN_FLOW 19
#define PIN_TRIG 27
#define PIN_ECHO 14
#define PIN_SOLENOID 25

// Kalibrasi
#define SOIL_DRY 3200      
#define SOIL_WET 1200  

#define TANK_HEIGHT 100.0 // Tinggi dalam cm
#define SENSOR_OFFSET 20.0 // JSN-SR04T tidak bisa baca < 20cm, pasang lebih tinggi dari air

// Set Parameter
#define TEMP_MAX 30.5      
#define LEVEL_SAFETY 95.0  
#define LEVEL_CRITICAL 15.0

// Network
#define WIFI_SSID "Kangkung IOT"
#define WIFI_PASS "cloudnyiram"
#define MQTT_BROKER "10.71.238.37" // IP broker MQTT

#endif