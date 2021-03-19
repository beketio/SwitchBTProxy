#include "bt_hid_host.h"

// init static vars
std::unordered_map<esp_hidh_dev_t*, BtHidInputDevice*> BtHidHost::device_map;
HidInputDeviceConnectedCallback BtHidHost::_connected_callback;
HidInputDeviceDisconnectedCallback BtHidHost::_disconnected_callback;

void BtHidHost::hidCallback(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    ESP_LOGI(HIDH_TAG, "HID Event: %d", event_id);
    // Get event id and data
    esp_hidh_event_t event = (esp_hidh_event_t) event_id;
    esp_hidh_event_data_t *param = (esp_hidh_event_data_t *) event_data;

    // Get device id from open event struct (every union has dev value, so any substruct works)
    esp_hidh_dev_t *device_id = param->open.dev;

    if(device_id == NULL)
        ESP_LOGI(TAG, "Null device id");

    ESP_LOGI(TAG, "Starting open event");
    if(event == ESP_HIDH_OPEN_EVENT) {
        const esp_hid_device_config_t *config = esp_hidh_dev_config_get(param->open.dev);
        ESP_LOGI(TAG, "got config");
        // Log event
        const uint8_t *dev_addr = esp_hidh_dev_bda_get(param->open.dev);
        ESP_LOGI(TAG, "got addr");
        const char *dev_name = esp_hidh_dev_name_get(param->open.dev);
        ESP_LOGI(TAG, "got name");
        if(config && config->device_name)
            ESP_LOGI(TAG, ESP_BD_ADDR_STR " OPEN: %s", ESP_BD_ADDR_HEX(dev_addr), config->device_name);
        else
            ESP_LOGI(TAG, ESP_BD_ADDR_STR " OPEN (Config null): %s", ESP_BD_ADDR_HEX(dev_addr), dev_name);
        esp_hidh_dev_dump(param->open.dev, stdout);
        
        // Notify conencted device callback
        if(_connected_callback)
            _connected_callback(config);
        else
            ESP_LOGW(HIDH_TAG, "No connected HID device callback specified");
        return;
    }

    if(event == ESP_HIDH_CLOSE_EVENT) {
        // Notify disconnected device callback
        if(_disconnected_callback)
            _disconnected_callback(device_id);
        else
            ESP_LOGW(HIDH_TAG, "No connected HID device callback specified");
        // Free memory
        esp_hidh_dev_free(device_id);
        return;
    }

    BtHidInputDevice *device = device_map[device_id];

    // Confirm device is registered
    if(!device) {
        const uint8_t *dev_addr = esp_hidh_dev_bda_get(param->open.dev);
        const char *dev_name = esp_hidh_dev_name_get(param->open.dev);
        ESP_LOGI(HIDH_TAG, ESP_BD_ADDR_STR " Device not registered: %s", ESP_BD_ADDR_HEX(dev_addr), dev_name);
        return;
    }

    if(event == ESP_HIDH_BATTERY_EVENT) {
        device->onBatteryEvent(param->battery.level);
    } 
    else if(event == ESP_HIDH_INPUT_EVENT) {
        device->onInputEvent(param->input.data, param->input.length);
    }
    else if(event == ESP_HIDH_FEATURE_EVENT) {
        device->onFeatureEvent(param->input.data, param->input.length);
    }
    else {
        ESP_LOGE(HIDH_TAG, "Unhandled event: %d", event);
    }
}

void BtHidHost::init(HidInputDeviceConnectedCallback connected_callback, HidInputDeviceDisconnectedCallback disconnected_callback) {
    _connected_callback = connected_callback;
    _disconnected_callback = disconnected_callback;

    // esp_hidh checks for this, even in BT_CLASSIC mode ಠ╭╮ಠ
    // If a gattc callback isn't registered, epd_
    #if CONFIG_GATTC_ENABLE
        ESP_ERROR_CHECK( esp_ble_gattc_register_callback(esp_hidh_gattc_event_handler) );
    #endif /* CONFIG_GATTC_ENABLE */

    esp_hidh_config_t config = {
        .callback = hidCallback,
    };

    //esp_err_t ret;
    //if((ret = esp_hidh_init(&config)) != ESP_OK) {
    //    ESP_LOGE(HIDH_TAG, "%s start discovery failed: %s\n", __func__, esp_err_to_name(ret));
    //}

    ESP_ERROR_CHECK( esp_hidh_init(&config) );
}

void BtHidHost::connect(esp_bd_addr_t address) {
    esp_hidh_dev_open(address, ESP_HID_TRANSPORT_BT, BLE_ADDR_TYPE_PUBLIC);
}

void BtHidHost::disconnect(esp_hidh_dev_t* device_id) {
    esp_hidh_dev_close(device_id);
    device_map[device_id] = nullptr;
}

void BtHidHost::registerInputDevice(BtHidInputDevice* device) {
    device_map[device->getDeviceId()] =  device;
}