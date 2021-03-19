#pragma once

#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_gap_ble_api.h"

#include "bt_utils.h"

#define GAP_TAG "GAP"




class BtGap {

public:
    typedef struct {
        esp_bd_addr_t address;
        char address_str[18];
        uint32_t cod;
        uint32_t major_class;
        char name[ESP_BT_GAP_MAX_BDNAME_LEN + 1];
    } bt_discovered_device;

    typedef void (*DeviceFoundCallback)(bt_discovered_device *device);

    static void startBluetooth(const char *name);
    static void startDiscovery(uint8_t timeout, bool blocking);
    static void stopDiscovery();
    static void setDeviceFoundCallback(DeviceFoundCallback callback);

    


private:
    static void gapCallback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);
    static void onDeviceDiscover(esp_bt_gap_cb_param_t *param);

    static DeviceFoundCallback device_found_callback;
    static xSemaphoreHandle discover_semaphore;

};