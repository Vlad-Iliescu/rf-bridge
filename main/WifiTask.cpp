#include "WifiTask.h"
#include <esp_log.h>
#include <esp_wifi.h>
#include <cstring>

#define TAG "[Wifi]"

WifiTask::WifiTask(const char *ssid, const char *password) : ssid(ssid), pass(password) {
    ESP_LOGI(TAG, "Got SSID <%s>", this->ssid);
    this->event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, WifiTask::sta_event_handler, this));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, WifiTask::sta_event_handler, this));
}

void WifiTask::sta_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    auto pWiFi = (WifiTask *) arg;

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "Event handler: WIFI_EVENT::WIFI_EVENT_STA_START");
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Event handler: WIFI_EVENT::WIFI_EVENT_STA_DISCONNECTED");
        esp_wifi_connect();
        pWiFi->network_set_connected(0);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "Event handler: IP_EVENT::IP_EVENT_STA_GOT_IP");
        auto *event = (ip_event_got_ip_t *) event_data;
        ESP_LOGI(TAG, "IP: "
                IPSTR, IP2STR(&event->ip_info.ip));
        pWiFi->network_set_connected(1);
    }
}

void WifiTask::run() {
    ESP_LOGI(TAG, "Network connect %s", this->ssid);

    wifi_config_t sta_config;
    strncpy((char *) sta_config.sta.ssid, this->ssid, sizeof(sta_config.sta.ssid) / sizeof(char));
    strncpy((char *) sta_config.sta.password, this->pass, sizeof(sta_config.sta.password) / sizeof(char));
    sta_config.sta.bssid_set = false;
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &sta_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    esp_netif_t *esp_netif = nullptr;
    esp_netif = esp_netif_next(esp_netif);
    ESP_ERROR_CHECK(esp_netif_set_hostname(esp_netif, this->hostname));
    ESP_LOGI(TAG, "Wifi_init_sta finished.");
}

void WifiTask::network_set_connected(uint8_t flag) {
    if (flag) {
        xEventGroupSetBits(this->event_group, WIFI_STA_CONNECTED_BIT);
        xEventGroupClearBits(this->event_group, WIFI_STA_DISCONNECTED_BIT);
    } else {
        xEventGroupSetBits(this->event_group, WIFI_STA_DISCONNECTED_BIT);
        xEventGroupClearBits(this->event_group, WIFI_STA_CONNECTED_BIT);
    }
}

bool WifiTask::isConnected() {
    return (xEventGroupGetBits(this->event_group) & WIFI_STA_CONNECTED_BIT) == WIFI_STA_CONNECTED_BIT;
}

unsigned int WifiTask::wait_until_connected(unsigned int delay_ms) {
    ESP_LOGI(TAG, "Waiting for IP...");
    unsigned int wait =
            xEventGroupWaitBits(this->event_group, WIFI_STA_CONNECTED_BIT, false, true, delay_ms / portTICK_PERIOD_MS) &
            WIFI_STA_CONNECTED_BIT;
    if (!wait) {
        ESP_LOGI(TAG, "Timeout..");
    }
    return wait;
}

WifiTask::~WifiTask() {
    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, WifiTask::sta_event_handler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, WifiTask::sta_event_handler));
    vEventGroupDelete(this->event_group);
}
