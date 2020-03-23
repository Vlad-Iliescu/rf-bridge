#include "MqttQueue.h"
#include "esp_log.h"

#define TAG "[MQTT_QUEUE]"

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


MqttQueue::MqttQueue() {
    this->queue = xQueueCreate(QUEUE_SIZE, sizeof(MqttEvent*));

    ESP_ERROR_CHECK(this->queue == nullptr ? ESP_FAIL : ESP_OK);
    ESP_LOGI(TAG, "Queue initialised!");
}

esp_err_t MqttQueue::add(const MqttEvent *event) {
    return xQueueSendToBack(this->queue, (void *) &event, 1000 / portTICK_RATE_MS);
}

esp_err_t MqttQueue::add(unsigned int remoteId, unsigned char key, DeviceState state) {
    auto *event = new MqttEvent(remoteId, key, state);
    return xQueueSendToBack(this->queue, (void *) &event, 1000 / portTICK_RATE_MS);;
}

MqttEvent *MqttQueue::pop() {
    MqttEvent *event;
    if (xQueueReceive(this->queue, &event, portMAX_DELAY)) {
        return event;
    }
    return nullptr;
}
