#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "app.h"
#include "credentials_store.h"
#include "reset_task.h"

/**
 * @param reset_handle handler for factory_reset
 */
#define TAG "[RESET TASK]"

DRAM_ATTR int state;
DRAM_ATTR time_t now;

/**
 * Waits for the reset bit. Then cleans all saved credentials and restarts the board.
 * @param args unused
 */
static void factory_reset_task(void *args) {
    APP_UNUSED(args);

    while (true) {
        EventBits_t uxBits = xEventGroupWaitBits(app_event_group,
                                                 (RESET_BIT | REBOOT_BIT) /*bits*/,
                                                 false /* clear it */,
                                                 false, /* wait for all*/
                                                 portMAX_DELAY /* delay */
        );

        if ((uxBits & RESET_BIT) != 0) {
            ESP_LOGI(TAG, "Reset requested");
            credentials_store_clear();
            esp_restart();
        }

        if ((uxBits & REBOOT_BIT) != 0) {
            ESP_LOGI(TAG, "Reboot requested");
            esp_restart();
        }

        vTaskDelay(50 / portTICK_RATE_MS);

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

    int new_state = gpio_get_level(DEVICE_FACTORY_RESET_PIN);
    if (new_state == state) {
        // half bouncing same state
        return;
    }

    // check state
    state = new_state;
    if (state == 0) { // button pressed
        now = time(NULL);
    } else { // button released
        if (time(NULL) - now >= 4 /* time to hold until reset in secs */) {
            xEventGroupSetBitsFromISR(app_event_group, RESET_BIT, NULL);
        } else {
            xEventGroupSetBitsFromISR(app_event_group, REBOOT_BIT, NULL);
        }
    }
}

void init_factory_reset_task(gpio_num_t reset_pin) {
    ESP_LOGI(TAG, "Setting up factory reset...");

    state = 1; // button start as high
    now = time(NULL);

    gpio_pad_select_gpio(reset_pin);
    ESP_ERROR_CHECK(gpio_set_direction(reset_pin, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(reset_pin, GPIO_PULLUP_ONLY));
    ESP_ERROR_CHECK(gpio_set_intr_type(reset_pin, GPIO_INTR_ANYEDGE));
    ESP_ERROR_CHECK(gpio_intr_enable(reset_pin));
    ESP_ERROR_CHECK(gpio_install_isr_service(GPIO_INTR_ANYEDGE));
    ESP_ERROR_CHECK(gpio_isr_handler_add(reset_pin, pinhandler, NULL));

    xTaskCreate(&factory_reset_task, "factory_reset_task", 1 << 11, NULL, 10, NULL);
}
