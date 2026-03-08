#include "config.h"
#include "soil.h"
#include "temp.h"
#include "level.h"
#include "flow.h"
#include "valve.h"
#include "network.h"

// Global Data Structure
struct SystemState {
    float t1, t2, flow, level; 
    int s1, s2; 
    bool solenoid;
} currentData;

SemaphoreHandle_t dataMutex;

// TASK CONTROL (Core 1: Sensor & Logic)
void TaskControl(void *pv) {
    setupTemp(); 
    setupLevel();
    setupFlow();
    setupSolenoid();

    for(;;) {
        TempData td = readTemperature();
        SoilData sd = readSoilMoisture();
        float lvl = getWaterLevelPct();
        float flow = getFlowRate();

        bool drySoil  = (sd.s1 < SOIL_MIN) || (sd.s2 < SOIL_MIN);  // Primary Trigger
        bool tempValid    = (td.t1 > TEMP_MIN) && (td.t1 < TEMP_MAX); // Validator
        bool emergencyTemp = (td.t1 > TEMP_MAX); // Bypass Trigger
        bool emergencyLevel = (lvl < LEVEL_MIN);  
        bool FinalDecision = (drySoil && tempValid) || emergencyTemp || emergencyLevel; // Final Decision Trigger 
        
        bool statusSolenoid = false;

        // Serial.print("Temp 1: "); Serial.print(td.t1); Serial.print(" °C | ");
        // Serial.print("Temp 2: "); Serial.print(td.t2); Serial.println(" °C | ");
        // Serial.print("Soil 1: "); Serial.print(sd.s1); Serial.println(" % | ");
        // Serial.print("Soil 2: "); Serial.print(sd.s2); Serial.println(" % | ");
        // Serial.print("Flow: "); Serial.print(flow); Serial.println(" L/min ");
        // Serial.print("Level: "); Serial.print(lvl); Serial.println(" % | ");
        

        if (FinalDecision && (lvl < LEVEL_MAX)) {
            statusSolenoid = true;
        } else {
            statusSolenoid = false; // kondisi pemutus
        }
        
        controlSolenoid(statusSolenoid);

        // Data Global Update (Mutex)
        if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
            currentData = {td.t1, td.t2, flow, lvl, sd.s1, sd.s2, statusSolenoid};
            xSemaphoreGive(dataMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Cek kondisi (1s)
    }
}

// --- TASK TELEMETRY (Core 0: MQTT & WiFi) ---
void TaskTelemetry(void *pv) {
    setupNetwork();
    uint32_t lastPublish = 0;
    // const uint32_t interval = 60000; // Kirim data tiap 1 mnt
    const uint32_t interval = 5000;

    for(;;) {
        maintainConnection(); // LWT "OFFLINE" jika ESP32 OFF

        if (millis() - lastPublish > interval) {
            if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
                sendTelemetry(
                    currentData.t1, currentData.t2,
                    currentData.s1, currentData.s2,
                    currentData.level, currentData.flow,
                    currentData.solenoid
                );
                xSemaphoreGive(dataMutex);
            }
            lastPublish = millis();
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Time for Wifi Stack
    }
}

void setup() {
    Serial.begin(115200);
    dataMutex = xSemaphoreCreateMutex();
    
    // Core 1: Sensing
    xTaskCreatePinnedToCore(TaskControl, "Control", 4096, NULL, 3, NULL, 1);
    
    // Core 0: MQTT & WiFi
    xTaskCreatePinnedToCore(TaskTelemetry, "Network", 8192, NULL, 1, NULL, 0);
}

void loop() { 
    vTaskDelete(NULL); 
}