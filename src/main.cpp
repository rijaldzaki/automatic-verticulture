#include "config.h"
#include "soil.h"
#include "temp.h"
#include "level.h"
#include "flow.h"
#include "valve.h"
#include "network.h"

// Struktur untuk berbagi data antar core
struct SystemState {
    float t1, t2, level, flow;
    int soil;
    bool solenoid;
} currentData;

SemaphoreHandle_t dataMutex;

// --- TASK CONTROL (Core 1) ---
void TaskControl(void *pv) {
    setupTemp(); setupLevel(); setupFlow(); setupSolenoid();
    for(;;) {
        TempData td = readTemperature();
        float lvl = getWaterLevelPct();
        int soil = readSoilAverage();
        float flow = getFlowRate();

        // LOGIKA KONTROL
        bool statusSolenoid = false;
        if ((soil > SOIL_DRY || td.t1 > TEMP_MAX) && lvl < LEVEL_SAFETY) {
            statusSolenoid = true;
        }
        controlSolenoid(statusSolenoid);

        // Simpan ke Global Struct (Gunakan Mutex)
        if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
            currentData = {td.t1, td.t2, lvl, flow, soil, statusSolenoid};
            xSemaphoreGive(dataMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Sampling setiap 1 detik
    }
}

// --- TASK TELEMETRY (Core 0) ---
void TaskTelemetry(void *pv) {
    setupNetwork();
    uint32_t lastPublish = 0;
    for(;;) {
        maintainConnection();
        if (millis() - lastPublish > 60000) { // Kirim tiap 1 menit
            if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
                sendTelemetry(currentData.t1, currentData.t2, currentData.soil, currentData.level, currentData.flow, currentData.solenoid);
                xSemaphoreGive(dataMutex);
            }
            lastPublish = millis();
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Beri nafas untuk background WiFi stack
    }
}

void setup() {
    Serial.begin(115200);
    dataMutex = xSemaphoreCreateMutex();
    
    xTaskCreatePinnedToCore(TaskControl, "Control", 4096, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(TaskTelemetry, "Network", 8192, NULL, 1, NULL, 0);
}

void loop() { vTaskDelete(NULL); }