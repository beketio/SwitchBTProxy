#include "bt_stack.h"

// Callback function for GAP events
void GapCallback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {

}

/*
    Initializes and enables bluetooth
*/
void StartBluetooth(const char *device_name) {
    // Don't start if already started
    static uint8_t initialized = 0;
    if(initialized)
        return;
    initialized = 1;

    // Error return value
    esp_err_t ret;

    // Releases BLE controller memory. BLE cannot be used after calling this.
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(GAP_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // Enable the BT controller in classic mode (version < 4.0)
    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK) {
        ESP_LOGE(GAP_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_init()) != ESP_OK) {
        ESP_LOGE(GAP_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_enable()) != ESP_OK) {
        ESP_LOGE(GAP_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // Set the device name
    esp_bt_dev_set_device_name(device_name);

    /* register GAP callback function */
    esp_bt_gap_register_callback(GapCallback);

    /* set discoverable and connectable mode, wait to be connected */
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);    

}

void StartDiscovery(int timeout) {
    esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
}

void SetDeviceFoundCallback(void *device_found_callback) {
    
}