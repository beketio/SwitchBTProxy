#include "bt_hid_device_api.h"

void BtHidDeviceApi::init() {
    esp_hid_device_config_t config = {
        .vendor_id = 1,
        .product_id = 1,                    /*!< HID Product ID */
        .version = 1,                       /*!< HID Product Version */
        .device_name = "name",                /*!< HID Device Name */
        .manufacturer_name = "manu",          /*!< HID Manufacturer */
        .serial_number = "serial"              /*!< HID Serial Number */
        //esp_hid_raw_report_map_t *report_maps;  /*!< Array of the raw HID report maps */
        //uint8_t report_maps_len;                /*!< number of raw report maps in the array */
    };
    esp_hidd_dev_init()
}

