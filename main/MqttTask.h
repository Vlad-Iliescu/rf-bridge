#ifndef HELLO_WORLD_MQTTTASK_H
#define HELLO_WORLD_MQTTTASK_H

#include "interfaces/Task.h"
#include "mqtt_client.h"

class MqttTask : public Task {
public:
    explicit MqttTask(const char *url);

    void run() override;

private:
    const char *url;

    static void event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

    static esp_err_t event_handler_cb(esp_mqtt_event_handle_t event);
};


#endif //HELLO_WORLD_MQTTTASK_H
