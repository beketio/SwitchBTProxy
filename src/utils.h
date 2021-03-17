#pragma once

#include <stdint.h>
#include "esp_bt_device.h"


namespace Utils {

    // Converts a BT/MAC Address to a string representation
    static char *AddrToStr(esp_bd_addr_t bda, char *str, size_t size);

    // Converts a UUID to a string representation
    static char *UUIDToStr(esp_bt_uuid_t *uuid, char *str, size_t size);

}