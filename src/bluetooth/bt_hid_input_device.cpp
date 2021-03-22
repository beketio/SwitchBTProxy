#include "bt_hid_input_device.h"

BtHidInputDevice::BtHidInputDevice(esp_hidh_dev_t *id) {
    device_id = id;

    // Map reports by type
    size_t num_reports = 0;
    esp_hid_report_item_t *reports;
    esp_hidh_dev_reports_get(id, &num_reports, &reports);
    for(int i = 0; i < num_reports; i++) {
        if(reports[i].report_type == ESP_HID_REPORT_TYPE_OUTPUT) {
            output_report_map[reports[i].report_id] = reports[i].map_index;
        } else if (reports[i].report_type == ESP_HID_REPORT_TYPE_FEATURE) {
            feature_report_map[reports[i].report_id] = reports[i].map_index;
        }
    }
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

void BtHidInputDevice::sendOutputReport(uint8_t report_id, uint8_t *data, uint8_t length) {
    // TODO: Replace with bt_hid_host.h method
    esp_hidh_dev_output_set(device_id, output_report_map[report_id], report_id, data + 1, length - 1);
}

void BtHidInputDevice::sendFeatureReport(uint8_t report_id, uint8_t *data, uint8_t length) {
    // TODO: Replace with bt_hid_host.h method
    esp_hidh_dev_feature_set(device_id, feature_report_map[report_id], report_id, data + 1, length - 1);
}

