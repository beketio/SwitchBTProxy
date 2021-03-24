#include "bt_hid_device_api.h"

//#include "bta/bta_hh_api.h"
#include "btc/btc_task.h"

static const char *esp_log_tag = "BtHidDevApi";

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

    //if (esp_bluedroid_get_status() != ESP_BLUEDROID_STATUS_ENABLED) {
    //    return ESP_ERR_INVALID_STATE;
    //}
    
    //btc_msg_t msg;
    //esp_hidd_args_t args;
    //memset(&args, 0, sizeof(esp_hidd_args_t));
    //args.callbacks = 0;

    //msg.sig = BTC_SIG_API_CALL;
    //msg.pid = BTC_PID_HD;
    //msg.act = BTC_HD_INIT_EVT;

    /* Switch to BTC context */
    //bt_status_t stat = btc_transfer_context(&msg, &args, sizeof(esp_hidd_args_t), NULL);
    
}   

