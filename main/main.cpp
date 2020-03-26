#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <nvs.h>
#include <nvs_flash.h>
#include "common.h"
#include "LivoloRFTask.h"
#include "MqttQueue.h"
#include "MqttTask.h"
#include "WifiTask.h"

#define TAG "[Main]"

extern "C" {
void app_main(void);
}

void app_main() {
    ESP_LOGI(TAG, "START");

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    auto wifiTask = new WifiTask(IT_WIFI_SSID, IT_WIFI_PASS);
    wifiTask->run();
    wifiTask->wait_until_connected(10 * 1000);

    ESP_LOGI(TAG, "Connected? %i", wifiTask->isConnected());

    auto queue = MqttQueue();
    auto mqttTask = MqttTask(IT_MQTT_HOST, &queue);
    xTaskCreate(MqttTask::runAsync, "mqtt_task", 1 << 14 /* 16384 */, &mqttTask, 5, nullptr);

    auto livolo = LivoloRFTask(static_cast<gpio_num_t>(IT_RF_GPIO));

    while (true) {
        auto *event = queue.pop();
        livolo.sendSignal(event->remoteId, event->key);
        ESP_LOGI(TAG, "%i", event->remoteId);
        delete event;

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    vTaskDelete(nullptr);
}
