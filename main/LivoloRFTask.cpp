#include "LivoloRFTask.h"
#include "esp_log.h"

#define TAG "Livolo"
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)

LivoloRFTask::LivoloRFTask(gpio_num_t pin) : txPin(pin) {
    gpio_pad_select_gpio(pin);
//    gpio_set_pull_mode(pin, GPIO_FLOATING);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    txPin = pin;
    ESP_LOGI(TAG, "RF init on pin %i", pin);
}

void LivoloRFTask::sendSignal(unsigned int remoteId, byte keycode) {
    ESP_LOGI(TAG, "{ remoteId: %i, keycode: %i }", remoteId, keycode);
    for (byte pulse = 0; pulse <= 180; pulse = pulse + 1) { // how many times to transmit a command
        sendPulse(1); // Start
        this->high = true; // first pulse is always high

        for (byte i = 16; i > 0; --i) { // transmit remoteID
            byte txPulse = bitRead(remoteId, i - 1); // read bits from remote ID
            selectPulse(txPulse);
        }

        for (byte i = 7; i > 0; i--) { // transmit keycode
            byte txPulse = bitRead(keycode, i - 1); // read bits from keycode
            selectPulse(txPulse);
        }

        vTaskDelay(0);
        gpio_set_level(this->txPin, 0);
        this->high = false;
    }
}

void LivoloRFTask::sendPulse(byte pulse) {
    switch (pulse) { // transmit pulse
        case 1: // Start
            gpio_set_level(this->txPin, 1);
            ets_delay_us(525); // 525 | 550
            break;
        case 2: // "High Zero"
            gpio_set_level(this->txPin, 0);
            ets_delay_us(110); // 110
            gpio_set_level(this->txPin, 1);
            break;
        case 3: // "High One"
            gpio_set_level(this->txPin, 0);
            ets_delay_us(305); // 305 | 303
            gpio_set_level(this->txPin, 1);
            break;
        case 4: // "Low Zero"
            gpio_set_level(this->txPin, 1);
            ets_delay_us(110);
            gpio_set_level(this->txPin, 0);
            break;
        case 5: // "Low One"
            gpio_set_level(this->txPin, 1);
            ets_delay_us(305); // 305 | 290
            gpio_set_level(this->txPin, 0);
            break;
        default:
            break;
    }
}

void LivoloRFTask::selectPulse(byte bit) {
    switch (bit) {
        case 0:
            for (byte i = 1; i < 3; i++) {
                if (high) {   // if current pulse should be high, send High Zero
                    sendPulse(2);
                } else {              // else send Low Zero
                    sendPulse(4);
                }
                high = !high; // invert next pulse
            }
            break;
        case 1:                // if current pulse should be high, send High One
            if (high) {
                sendPulse(3);
            } else {             // else send Low One
                sendPulse(5);
            }
            high = !high; // invert next pulse
            break;
        default:
            break;
    }

}

void LivoloRFTask::run() {
    vTaskDelete(nullptr);
}
