#pragma once

#include <stdint.h>
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_hidd.h"

typedef struct {
    esp_bd_addr_t address;
    char device_name[ESP_BT_GAP_MAX_BDNAME_LEN + 1];
} bt_device;

class BtHidDeviceApi {
    static void init();
};
