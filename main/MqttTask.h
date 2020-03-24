#ifndef RF_BRIDGE_MQTTTASK_H
#define RF_BRIDGE_MQTTTASK_H

#include "interfaces/Task.h"
#include "mqtt_client.h"
#include "MqttQueue.h"

class MqttTask : public Task {
public:
    explicit MqttTask(const char *url, MqttQueue *queue);

    void run() override;

private:
    const char *url;

    MqttQueue *queue = nullptr;

    static void event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
};


#endif //RF_BRIDGE_MQTTTASK_H
