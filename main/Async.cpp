#include <esp_log.h>
#include "Async.h"

void Async::run() {
    ESP_LOGW("[ASYNC]", "Running.....");
    vTaskDelete(nullptr);
}
