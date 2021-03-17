#include "bt_discovery.h"

void StartDiscovery(int timeout) {
    esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
}