#ifndef RF_BRIDGE_MQTTQUEUE_H
#define RF_BRIDGE_MQTTQUEUE_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <cJSON.h>
#include "MqttEvent.h"

class MqttQueue {
public:
    MqttQueue();

    esp_err_t add(const MqttEvent *event);

    esp_err_t add(unsigned int remoteId, unsigned char key, DeviceState state);

    esp_err_t add(const char *json);

    MqttEvent *pop(); //blocking

    virtual ~MqttQueue();

private:
    xQueueHandle queue;
};


#endif //RF_BRIDGE_MQTTQUEUE_H
