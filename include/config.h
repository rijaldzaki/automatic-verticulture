#ifndef __CONFIG_H__
#define __CONFIG_H__

// ID Pot 
#define POT_ID "POT-01" 
#define TOPIC_PREFIX "verticulture/devices/"

// Pin Config
#define PIN_SOIL_1 33 
#define PIN_SOIL_2 32
#define PIN_DS18B20 26
#define PIN_FLOW 19
#define PIN_TRIG 27
#define PIN_ECHO 14
#define PIN_SOLENOID 25

#define SOIL_DRY 3200      
#define SOIL_WET 1200  

#define TANK_HEIGHT 100.0
#define SENSOR_OFFSET 20.0

// Threshold Config
#define SOIL_MIN 30
#define TEMP_MAX 30.5
#define TEMP_MIN 28.0
#define LEVEL_MAX 90.0  
#define LEVEL_MIN 10.0

// Network Config
#define WIFI_SSID "Kangkung IOT"
#define WIFI_PASS "siramterus"
#define MQTT_BROKER "10.71.238.37"

#endif