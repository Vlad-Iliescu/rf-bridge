#include "freertos/FreeRTOS.h"
#include <esp_log.h>
#include "freertos/task.h"
#include "driver/gpio.h"
#include "common.h"
#include "WifiTask.h"
#include <nvs.h>
#include <nvs_flash.h>

//#define RF_GPIO CONFIG_DATA_GPIO
#define SET_GPIO CONFIG_SET_GPIO
#define TAG "[Main]"

extern "C" {
void app_main(void);
}

void app_main() {
    ESP_LOGI(TAG, "START");
//    init_factory_reset_task(static_cast<gpio_num_t>(SET_GPIO));


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

    vTaskDelay(10000 / portTICK_PERIOD_MS);


//    return;
    /*
    printf("Hello world!\n");

    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");


    Livolo livolo(static_cast<gpio_num_t>(RF_GPIO));
//    livolo.sendButton(19787, 8);

//    for (int i = 10; i >= 0; i--) {
//        printf("Restarting in %d seconds...\n", i);
//        vTaskDelay(1000 / portTICK_PERIOD_MS);
//    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();

//    gpio_pad_select_gpio(RF_GPIO);
//    // Set the GPIO as a push/pull output
//    gpio_set_direction(static_cast<gpio_num_t>(RF_GPIO), GPIO_MODE_OUTPUT);
//    while(true) {
//        // Blink off (output low)
//        printf("Turning off the LED\n");
//        gpio_set_level(static_cast<gpio_num_t>(RF_GPIO), 0);
//        vTaskDelay(1000 / portTICK_PERIOD_MS);
//        // Blink on (output high)
//        printf("Turning on the LED\n");
//        gpio_set_level(static_cast<gpio_num_t>(RF_GPIO), 1);
//        vTaskDelay(1000 / portTICK_PERIOD_MS);
//    }
*/
}
