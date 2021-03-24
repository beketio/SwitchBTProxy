#pragma once

#include <unordered_map>
#include <stdint.h>

#include "esp_hidh.h"

class BtHidInputDevice {
public:
    BtHidInputDevice(esp_hidh_dev_t *device);
    ~BtHidInputDevice();
    esp_hidh_dev_t *getDeviceId();
    uint8_t getBatteryLevel();
    void onBatteryEvent(uint8_t level);
    virtual void onInputEvent(uint8_t report_id, uint8_t *data, uint16_t length) = 0;
    virtual void onFeatureEvent(uint8_t report_id, uint8_t *data, uint16_t length) = 0;

protected:
    esp_hidh_dev_t *device_id;
    uint8_t battery_level;
    void sendOutputReport(uint8_t report_id, uint8_t *data, uint8_t length);
    void sendFeatureReport(uint8_t report_id, uint8_t *data, uint8_t length);

private:
    std::unordered_map<uint8_t, uint8_t> output_report_map; 
    std::unordered_map<uint8_t, uint8_t> feature_report_map; 
    
};