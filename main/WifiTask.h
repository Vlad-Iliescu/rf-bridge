#ifndef HELLO_WORLD_WIFITASK_H
#define HELLO_WORLD_WIFITASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event_base.h"
#include "interfaces/Task.h"

class WifiTask : public Task {
public:
    WifiTask(const char *ssid, const char *password);

    void run() override;

    bool isConnected();

    unsigned int wait_until_connected(unsigned int delay_ms);

    virtual ~WifiTask();

private:
    const char *ssid;
    const char *pass;
    const char *hostname = "WIFY";
    EventGroupHandle_t event_group;

    static void sta_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

    void network_set_connected(uint8_t flag);
};


#endif //HELLO_WORLD_WIFITASK_H
