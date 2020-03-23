#ifndef HELLO_WORLD_MQTTQUEUE_H
#define HELLO_WORLD_MQTTQUEUE_H

#include <freertos/FreeRTOS.h>
#include "freertos/queue.h"

#define QUEUE_SIZE 10

typedef enum {
    OFF = 0,
    ON = 1
} DeviceState;

class MqttEvent {
public:
    MqttEvent(unsigned int remoteId, unsigned char key, DeviceState state);

    MqttEvent();

    virtual ~MqttEvent();

public:
    unsigned int remoteId{};
    unsigned char key{};
    DeviceState state;
};

class MqttQueue {
public:
    MqttQueue();

    esp_err_t add(const MqttEvent *event);

    esp_err_t add(unsigned int remoteId, unsigned char key, DeviceState state);

    MqttEvent * pop(); //blocking

private:
    xQueueHandle queue;
};


#endif //HELLO_WORLD_MQTTQUEUE_H
