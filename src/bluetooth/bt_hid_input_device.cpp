#include "bt_hid_input_device.h"

BtHidInputDevice::BtHidInputDevice(esp_hidh_dev_t *id) {
    device_id = id;
}

BtHidInputDevice::~BtHidInputDevice() {
    
}

esp_hidh_dev_t *BtHidInputDevice::getDeviceId() {
    return device_id;
}

uint8_t BtHidInputDevice::getBatteryLevel() {
    return battery_level;
}

void BtHidInputDevice::onBatteryEvent(uint8_t level) {
    battery_level = level;
}

void BtHidInputDevice::sendOutputReport(uint8_t repord_id, uint8_t *data, uint8_t length) {
    esp_hidh_dev_output_set(device_id, 0, repord_id, data, length);
}

void BtHidInputDevice::sendFeatureReport(uint8_t repord_id, uint8_t *data, uint8_t length) {
    esp_hidh_dev_feature_set(device_id, 0, repord_id, data, length);
}

