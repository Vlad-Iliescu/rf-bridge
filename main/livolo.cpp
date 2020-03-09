/*
  Livolo.cpp - Library for Livolo wireless switches.
  Created by Sergey Chernov, October 25, 2013.
  Released into the public domain.
*/

#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "livolo.h"
#include "esp32/rom/ets_sys.h"

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define TAG "[Livolo]"

Livolo::Livolo(gpio_num_t pin) {

    gpio_pad_select_gpio(pin);
//    gpio_set_pull_mode(pin, GPIO_FLOATING);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    txPin = pin;
    ESP_LOGI(TAG, "Construct [%i]" , pin);
}

// keycodes #1: 0, #2: 96, #3: 120, #4: 24, #5: 80, #6: 48, #7: 108, #8: 12, #9: 72; #10: 40, #OFF: 106
// real remote IDs: 6400; 19303
// tested "virtual" remote IDs: 10550; 8500; 7400
// other IDs could work too, as long as they do not exceed 16 bit
// known issue: not all 16 bit remote ID are valid
// have not tested other buttons, but as there is dimmer control, some keycodes could be strictly system
// use: sendButton(remoteID, keycode), see example blink.ino; 


void Livolo::sendButton(unsigned int remoteID, byte keycode) {
    ESP_LOGI(TAG, "{remoteId: %i, keycode: %i}" , remoteID, keycode);
    for (pulse = 0; pulse <= 180; pulse = pulse + 1) { // how many times to transmit a command
        sendPulse(1); // Start
        this->high = true; // first pulse is always high

//        byte x[17];

        for (i = 16; i > 0; i--) { // transmit remoteID
            byte txPulse = bitRead(remoteID, i - 1); // read bits from remote ID
//            ESP_LOGI(TAG, "Remote byte: %i" , txPulse);
//            x[17 - i-1] = 48 + txPulse;
            selectPulse(txPulse);
        }
//        x[16] = '\0';
//        ESP_LOGI(TAG, "Remote byte: %s" , x);
//
//        byte y[8];

        for (i = 7; i > 0; i--) { // transmit keycode
            byte txPulse = bitRead(keycode, i - 1); // read bits from keycode
//            y[8 - i-1] = 48 + txPulse;
//            ESP_LOGI(TAG, "key byte: %i" , txPulse);
            selectPulse(txPulse);
        }
//        y[7] = '\0';
//        ESP_LOGI(TAG, "key byte: %s" , y);
        vTaskDelay(0);
        gpio_set_level(this->txPin, 0);
    }
}

// build transmit sequence so that every high pulse is followed by low and vice versa

void Livolo::selectPulse(byte inBit) {
//    printf("%i", inBit);
    switch (inBit) {
        case 0:
            for (byte ii = 1; ii < 3; ii++) {
                if (high == true) {   // if current pulse should be high, send High Zero
                    sendPulse(2);
                } else {              // else send Low Zero
                    sendPulse(4);
                }
                high = !high; // invert next pulse
            }
            break;
        case 1:                // if current pulse should be high, send High One
            if (high == true) {
                sendPulse(3);
            } else {             // else send Low One
                sendPulse(5);
            }
            high = !high; // invert next pulse
            break;
    }
}

// transmit pulses
// slightly corrected pulse length, use old (commented out) values if these not working for you

void Livolo::sendPulse(byte txPulse) {

    switch (txPulse) { // transmit pulse
        case 1: // Start
            gpio_set_level(this->txPin, 1);
//            printf("|1");
            ets_delay_us(525); // 525 | 550
            break;
        case 2: // "High Zero"
            gpio_set_level(this->txPin, 0);
//            printf("2");
            ets_delay_us(110); // 110
            gpio_set_level(this->txPin, 1);
//            printf("|");
            break;
        case 3: // "High One"
            gpio_set_level(this->txPin, 0);
//            printf("3");
            ets_delay_us(305); // 305 | 303
            gpio_set_level(this->txPin, 1);
//            printf("|");
            break;
        case 4: // "Low Zero"
            gpio_set_level(this->txPin, 1);
//            printf("4");
            ets_delay_us(110);
            gpio_set_level(this->txPin, 0);
//            printf("|");
            break;
        case 5: // "Low One"
            gpio_set_level(this->txPin, 1);
//            printf("5");
            ets_delay_us(305); // 305 | 290
            gpio_set_level(this->txPin, 0);
//            printf("|");
            break;
    }
}