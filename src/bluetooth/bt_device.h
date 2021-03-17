#pragma once

#include <stdint.h>
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"


typedef struct {
    esp_bd_addr_t address;
} bt_device;