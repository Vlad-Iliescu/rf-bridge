#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_log.h>

#include "reset_task.h"
#include "sdkconfig.h"
#include "livolo.h"

#define APP_UNUSED(x) (void)(x)

/**
 * @param reset_handle handler for factory_reset
 */
#define TAG "[RESET TASK]"

EventGroupHandle_t app_event_group;

/**
 * Waits for the reset bit. Then cleans all saved credentials and restarts the board.
 * @param args unused
 */
static void factory_reset_task(void *args) {
    APP_UNUSED(args);

    app_event_group = xEventGroupCreate();

    while (true) {
        xEventGroupWaitBits(app_event_group,
                                                 BIT9 /*bits*/,
                                                 false /* clear it */,
                                                 false, /* wait for all*/
                                                 portMAX_DELAY /* delay */
        );

        Livolo livolo(static_cast<gpio_num_t>(CONFIG_DATA_GPIO));
        livolo.sendButton(19787, 8);

        vTaskDelay(50 / portTICK_RATE_MS);

        xEventGroupClearBits(app_event_group, BIT9);

    }
    vTaskDelete(NULL);
}

/**
 * Factory reset button interrupt. When the button is pressed an event
 * is triggered signaling the reset of the board and cleaning all saved credentials
 *  high pin => button release
 *  low pin => button press
 * @param args unused
 */
static void IRAM_ATTR pinhandler(void *args) {
    APP_UNUSED(args);

    int new_state = gpio_get_level(static_cast<gpio_num_t>(CONFIG_SET_GPIO));
    if (new_state == 1) {
        // half bouncing same state
        return;
    }

    xEventGroupSetBitsFromISR(app_event_group, BIT9, NULL);
}

void init_factory_reset_task(gpio_num_t reset_pin) {
    ESP_LOGI(TAG, "Setting up factory reset...");

    gpio_pad_select_gpio(reset_pin);
    ESP_ERROR_CHECK(gpio_set_direction(reset_pin, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(reset_pin, GPIO_PULLUP_ONLY));
    ESP_ERROR_CHECK(gpio_set_intr_type(reset_pin, GPIO_INTR_ANYEDGE));
    ESP_ERROR_CHECK(gpio_intr_enable(reset_pin));
    ESP_ERROR_CHECK(gpio_install_isr_service(GPIO_INTR_ANYEDGE));
    ESP_ERROR_CHECK(gpio_isr_handler_add(reset_pin, pinhandler, NULL));

    xTaskCreate(&factory_reset_task, "factory_reset_task", 1 << 11, NULL, 10, NULL);
}
