#include <esp_log.h>
#include "MqttEvent.h"

#define TAG "[MQTT_EVENT]"

MqttEvent::MqttEvent(unsigned int remoteId, unsigned char key, DeviceState state) : remoteId(remoteId), key(key),
                                                                                    state(state) {
    ESP_LOGW(TAG, "INIT");
}

MqttEvent::~MqttEvent() {
    ESP_LOGW(TAG, "END");
}

MqttEvent::MqttEvent() {
    ESP_LOGW(TAG, "INIT");
};