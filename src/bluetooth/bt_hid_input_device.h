#pragma once

#include <stdint.h>
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_hidh.h"
#include "esp_hidh_bluedroid.h"

#define TAG "HID_HOST"

class BtHidInputDevice {
public:
    BtHidInputDevice(esp_hidh_dev_t *device);
    ~BtHidInputDevice();
    esp_hidh_dev_t *getDeviceId();
    uint8_t getBatteryLevel();
    void onBatteryEvent(uint8_t level);
    virtual void onInputEvent(uint8_t *data, uint16_t length) = 0;
    virtual void onFeatureEvent(uint8_t *data, uint16_t length) = 0;

protected:
    esp_hidh_dev_t *device_id;
    uint8_t battery_level;
};