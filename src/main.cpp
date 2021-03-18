#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"

#include "bluetooth/bt_gap.h"
#include "bluetooth/bt_hid_host.h"
#include "gamepad.h"
#include "switch_pro_controller.h"

#define SERVICE_UUID        "99a9bda3-ca61-44ce-a28d-d3f01785d5f7"
#define CHARACTERISTIC_UUID "4d364d37-ea00-4bbd-836e-24994d5b438e"

uint8_t pro_mac[] = { 0x94, 0x58, 0xCB, 0xCA, 0xDA, 0xB8 };

#define MAIN_TAG          "MAIN"

extern "C" {
	void app_main(void);
}

static xSemaphoreHandle discover_semaphore = nullptr;
static esp_bd_addr_t *discovered_addr;

void deviceFoundCallback(bt_discovered_device *device) {
    // Check if HID device
    if(device->major_class == ESP_BT_COD_MAJOR_DEV_PERIPHERAL) {
        ESP_LOGI(MAIN_TAG, "Found HID device: %s %s ", device->address_str, device->name);
        discovered_addr = &device->address;
        BtGap::stopDiscovery();
        xSemaphoreGive(discover_semaphore);
    }
}

void hidInputDeviceConnectedCallback(const esp_hid_device_config_t *connected_device) {
    ESP_LOGI(MAIN_TAG, "HID Device connected:");
    if(connected_device->device_name)
        ESP_LOGI(MAIN_TAG, " -- Manufacturer: %s", connected_device->device_name);
    if(connected_device->manufacturer_name)
        ESP_LOGI(MAIN_TAG, " -- Manufacturer: %s", connected_device->manufacturer_name);
    ESP_LOGI(MAIN_TAG, " -- Product ID: %d", connected_device->product_id);
    ESP_LOGI(MAIN_TAG, " -- Vender ID: %d", connected_device->vendor_id);
    if(connected_device->serial_number)
        ESP_LOGI(MAIN_TAG, " -- Serial: %s", connected_device->serial_number);
    ESP_LOGI(MAIN_TAG, " -- Version: %d", connected_device->version);
}

void hidInputDeviceDisconnectedCallback(esp_hidh_dev_t *disconnected_device) {
    ESP_LOGI(MAIN_TAG, "HID Device disconnected");
}

void app_main(void)
{
    ESP_LOGI(MAIN_TAG, "Hello world!");

    /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    discover_semaphore = xSemaphoreCreateBinary();

    BtGap::startBluetooth("EEE ESS PEE");
    ESP_LOGI(MAIN_TAG, "Bluetooth started");

    ESP_LOGI(MAIN_TAG, "Starting HID Host...");
    BtHidHost::init(hidInputDeviceConnectedCallback, hidInputDeviceDisconnectedCallback);
    ESP_LOGI(MAIN_TAG, "HID Host started...");

    ESP_LOGI(MAIN_TAG, "Discovering...");
    BtGap::setDeviceFoundCallback(deviceFoundCallback);
    BtGap::startDiscovery(10);

    xSemaphoreTake(discover_semaphore, portMAX_DELAY);

    BtHidHost::connect(*discovered_addr);
    ESP_LOGI(MAIN_TAG, "Connected to HID Device");


}

/*void setup() {
  SwitchProController cont;
  //app_main();
  //Serial.begin(115200);



}

void loop() {
  

}*/