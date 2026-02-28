#include "config.h"
#include "soil.h"
#include "temp.h"
#include "level.h"
#include "flow.h"
#include "valve.h"
#include "network.h"

// Struktur data
struct SystemState {
    float t1, t2, level, flow;
    int s1, s2; 
    bool solenoid;
} currentData;

SemaphoreHandle_t dataMutex;

// --- TASK CONTROL (Core 1: Sensor & Logic) ---
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

        // LOGIKA KONTROL (OR STRATEGY)
        // 1. Jika tanah atas kering (<30%) OR
        // 2. Jika tanah bawah kering (<30%) OR
        // 3. Jika suhu udara terlalu panas (>TEMP_MAX) OR
        // 4. Jika level air di pot sangat rendah (<LEVEL_CRITICAL)
        bool kondisiHaus = (sd.s1 < 30) || (sd.s2 < 30) || (td.t1 > TEMP_MAX) || (lvl < LEVEL_CRITICAL);
        
        // (Overflow Protection: solenoid mati jika air sudah mencapai batas atas )
        bool statusSolenoid = false;
        if (kondisiHaus && (lvl < LEVEL_SAFETY)) {
            statusSolenoid = true;
        }
        
        controlSolenoid(statusSolenoid);

        // Update data global dengan Mutex
        if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
            currentData = {td.t1, td.t2, lvl, flow, sd.s1, sd.s2, statusSolenoid};
            xSemaphoreGive(dataMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Cek kondisi setiap 1 s
    }
}

// --- TASK TELEMETRY (Core 0: MQTT & WiFi) ---
void TaskTelemetry(void *pv) {
    setupNetwork();
    uint32_t lastPublish = 0;
    const uint32_t interval = 60000; // Kirim data tiap 1 mnt

    for(;;) {
        maintainConnection(); // LWT "OFFLINE" jika ESP32 mati

        if (millis() - lastPublish > interval) {
            if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
                // fungsi di network.h menerima 7 parameter
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
        vTaskDelay(pdMS_TO_TICKS(100)); // Beri waktu untuk WiFi Stack
    }
}

void setup() {
    Serial.begin(115200);
    dataMutex = xSemaphoreCreateMutex();
    
    // Core 1: Fokus ke sensing
    xTaskCreatePinnedToCore(TaskControl, "Control", 4096, NULL, 3, NULL, 1);
    
    // Core 0: Fokus ke MQTT & WiFi
    xTaskCreatePinnedToCore(TaskTelemetry, "Network", 8192, NULL, 1, NULL, 0);
}

void loop() { 
    // Kontrol via FreeRTOS, loop() utama delete
    vTaskDelete(NULL); 
}