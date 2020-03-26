#include <esp_log.h>
#include "MqttQueue.h"
#include "common.h"

#define TAG "[MQTT_QUEUE]"

MqttQueue::MqttQueue() {
    this->queue = xQueueCreate(IT_QUEUE_SIZE, sizeof(MqttEvent *));

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

MqttQueue::~MqttQueue() {
    vQueueDelete(this->queue);
}

esp_err_t MqttQueue::add(const char *json) {
    auto *root = cJSON_Parse(json);

    int remoteId = cJSON_GetObjectItem(root, "remoteId")->valueint;
    unsigned char key = cJSON_GetObjectItem(root, "key")->valueint;
    DeviceState state = cJSON_IsTrue(cJSON_GetObjectItem(root, "state")) ? DeviceState::ON : DeviceState::OFF;

    cJSON_Delete(root);

    return this->add(remoteId, key, state);
}
