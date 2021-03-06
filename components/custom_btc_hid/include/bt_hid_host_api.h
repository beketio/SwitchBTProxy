#pragma once

#include <unordered_map>
#include <stdint.h>

#include "esp_hidh.h"

#include "bt_hid_input_device.h"

typedef void (*HidInputDeviceConnectedCallback)(esp_hidh_dev_t *connected_device, const esp_hid_device_config_t *device_info);
typedef void (*HidInputDeviceDisconnectedCallback)(esp_hidh_dev_t *disconnected_device);

class BtHidHostApi {

public:
    static void init(HidInputDeviceConnectedCallback connected_callback, HidInputDeviceDisconnectedCallback disconnected_callback);
    static void connect(esp_bd_addr_t address);
    static void disconnect(esp_hidh_dev_t* device_id);
    static void registerInputDevice(BtHidInputDevice* device);
    
private:
    static std::unordered_map<esp_hidh_dev_t*, BtHidInputDevice*> device_map;
    static HidInputDeviceConnectedCallback _connected_callback;
    static HidInputDeviceDisconnectedCallback _disconnected_callback;
    static void hidCallback(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

};

