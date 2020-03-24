#ifndef HELLO_WORLD_MQTTQUEUE_H
#define HELLO_WORLD_MQTTQUEUE_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include "MqttEvent.h"

class MqttQueue {
public:
    MqttQueue();

    esp_err_t add(const MqttEvent *event);

    esp_err_t add(unsigned int remoteId, unsigned char key, DeviceState state);

    MqttEvent *pop(); //blocking

    virtual ~MqttQueue();

private:
    xQueueHandle queue;
};


#endif //HELLO_WORLD_MQTTQUEUE_H
