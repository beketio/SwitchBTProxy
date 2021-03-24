#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_gap_ble_api.h"

#include "bt_gap_api.h"
#include "bt_hid_host_api.h"

#include "vendor_product_ids.h"
#include "gamepad.h"
#include "switch_pro_controller.h"

#define SERVICE_UUID        "99a9bda3-ca61-44ce-a28d-d3f01785d5f7"
#define CHARACTERISTIC_UUID "4d364d37-ea00-4bbd-836e-24994d5b438e"

uint8_t pro_mac[] = { 0x94, 0x58, 0xCB, 0xCA, 0xDA, 0xB8 };

static const char *tag_main = "MAIN";

extern "C" {
	void app_main(void);
}

static esp_bd_addr_t *discovered_addr;

void deviceFoundCallback(BtGapApi::DiscoveredDevice *device) {
    // Check if HID device
    if(device->major_class == ESP_BT_COD_MAJOR_DEV_PERIPHERAL) {
        ESP_LOGI(tag_main, "Found HID device: %s %s ", device->address_str, device->name);
        discovered_addr = &device->address;
        BtGapApi::stopDiscovery();
    }
}

void hidInputDeviceConnectedCallback(esp_hidh_dev_t *connected_device, const esp_hid_device_config_t *device_info) {
    ESP_LOGI(tag_main, "HID Device connected:");
    if(device_info->device_name)
        ESP_LOGI(tag_main, " -- Manufacturer: %s", device_info->device_name);
    if(device_info->manufacturer_name)
        ESP_LOGI(tag_main, " -- Manufacturer: %s", device_info->manufacturer_name);
    ESP_LOGI(tag_main, " -- Product ID: %d", device_info->product_id);
    ESP_LOGI(tag_main, " -- Vender ID: %d", device_info->vendor_id);
    if(device_info->serial_number)
        ESP_LOGI(tag_main, " -- Serial: %s", device_info->serial_number);
    ESP_LOGI(tag_main, " -- Version: %d", device_info->version);

    if(device_info->vendor_id == VID_NINTENDO && device_info->product_id == PID_NINTENDO_PRO_CONTROLLER) {
        SwitchProController *controller = new SwitchProController(connected_device);
        BtHidHostApi::registerInputDevice(controller);
    }
}

void hidInputDeviceDisconnectedCallback(esp_hidh_dev_t *disconnected_device) {
    ESP_LOGI(tag_main, "HID Device disconnected");
}

void app_main(void)
{
    ESP_LOGI(tag_main, "Hello world!");

    /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    BtGapApi::startBluetooth("EEE ESS PEE");
    ESP_LOGI(tag_main, "Bluetooth started");

    ESP_LOGI(tag_main, "Starting HID Host...");
    BtHidHostApi::init(hidInputDeviceConnectedCallback, hidInputDeviceDisconnectedCallback);
    ESP_LOGI(tag_main, "HID Host started...");

    ESP_LOGI(tag_main, "Discovering...");
    BtGapApi::setDeviceFoundCallback(deviceFoundCallback);
    //BtGapApi::startDiscovery(10, true);

    //BtHidHostApi::connect(*discovered_addr);
    ESP_LOGI(tag_main, "Connected to HID Device");


}

/*void setup() {
  SwitchProController cont;
  //app_main();
  //Serial.begin(115200);



}

void loop() {
  

}*/