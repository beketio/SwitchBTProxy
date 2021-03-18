#include "bt_gap.h"

DeviceFoundCallback device_found_callback = nullptr;

void BtGap::onDeviceDiscover(esp_bt_gap_cb_param_t *param) {

    // Create discovered device struct
    bt_discovered_device device;

    //char device_addr_str[18];
    //uint32_t device_class = 0;
    int32_t device_rssi = -129; /* invalid value */

    //char device_name[ESP_BT_GAP_MAX_BDNAME_LEN + 1];
    //device_name[0] = '\0';
    uint8_t device_name_len = 0;

    uint8_t eir_data[ESP_BT_GAP_EIR_DATA_LEN];
    uint8_t eir_data_len = 0;

    // Copy address
    memcpy(device.address, param->disc_res.bda, sizeof(esp_bd_addr_t));

    // Read MAC address as a string
    BtUtils::AddrToStr(param->disc_res.bda, device.address_str, 18);

    ESP_LOGI(GAP_TAG, "Device found: %s", device.address_str);
    esp_bt_gap_dev_prop_t *p;
    for (int i = 0; i < param->disc_res.num_prop; i++) {
        p = param->disc_res.prop + i;
        switch (p->type) {
        // Read Class of Device
        case ESP_BT_GAP_DEV_PROP_COD:
            device.cod = *(uint32_t *)(p->val);
            ESP_LOGI(GAP_TAG, "--Class of Device: 0x%x", device.cod);
            break;
        // Read RSSI
        case ESP_BT_GAP_DEV_PROP_RSSI:
            device_rssi = *(int8_t *)(p->val);
            ESP_LOGI(GAP_TAG, "--RSSI: %d", device_rssi);
            break;
        // Read device name
        case ESP_BT_GAP_DEV_PROP_BDNAME:
            device_name_len = (p->len > ESP_BT_GAP_MAX_BDNAME_LEN) ? ESP_BT_GAP_MAX_BDNAME_LEN : (uint8_t)p->len;
            memcpy(device.name, (uint8_t *)(p->val), device_name_len);
            device.name[device_name_len] = '\0';
            ESP_LOGI(GAP_TAG, "--Name: %s", device.name);
            break;
        // Read EIR Data
        case ESP_BT_GAP_DEV_PROP_EIR:
            memcpy(eir_data, (uint8_t *)(p->val), p->len);
            eir_data_len = p->len;
            break;
        default:
            break;
        }
    }

    // Check if type is peripheral (a controller)
    if (!esp_bt_gap_is_valid_cod(device.cod) ||
            !(esp_bt_gap_get_cod_major_dev(device.cod) == ESP_BT_COD_MAJOR_DEV_PERIPHERAL)) {
        return;
    }

    device.major_class = esp_bt_gap_get_cod_major_dev(device.cod);
    
    // Get name from eir
    if (device_name_len == 0 && eir_data_len > 0) {
        uint8_t *eir_device_name = NULL;
        uint8_t eir_device_name_len = 0;

        // Try reading local name
        eir_device_name = esp_bt_gap_resolve_eir_data(eir_data, ESP_BT_EIR_TYPE_CMPL_LOCAL_NAME, &eir_device_name_len);
        // If no local name, try reading short local name
        if (!eir_device_name) {
            eir_device_name = esp_bt_gap_resolve_eir_data(eir_data, ESP_BT_EIR_TYPE_SHORT_LOCAL_NAME, &eir_device_name_len);
        }

        // Copy to device name if a name was returned
        if (eir_device_name) {
            if (eir_device_name_len > ESP_BT_GAP_MAX_BDNAME_LEN) {
                eir_device_name_len = ESP_BT_GAP_MAX_BDNAME_LEN;
            }
            device_name_len = eir_device_name_len;
            memcpy(device.name, eir_device_name, device_name_len);
            device.name[device_name_len] = '\0';
        }
    }

    if(device_found_callback)
        device_found_callback(&device);

/*
    // Check if Pro Controller
    if(strncmp(device.name, "Pro Controller", device_name_len) == 0) {
        ESP_LOGI(GAP_TAG, "Found a pro controller at address %s", device.address_str);
        ESP_LOGI(GAP_TAG, "Stop controller discovery...");
        esp_bt_gap_cancel_discovery();
    } else {
        ESP_LOGI(GAP_TAG, "Found an HID device at address %s, name: \'%s\'\n", device.address_str, device.name);
    }*/


}

// Callback function for GAP events
void BtGap::gapCallback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
    ESP_LOGI(GAP_TAG, "GAP callback %d", event);
    switch (event) {
    case ESP_BT_GAP_DISC_RES_EVT: {
        onDeviceDiscover(param);
        break;
    }
    case ESP_BT_GAP_DISC_STATE_CHANGED_EVT: {
        if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STOPPED) {
            ESP_LOGI(GAP_TAG, "Device discovery stopped.");
            // get device stuff
        } else if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STARTED) {
            ESP_LOGI(GAP_TAG, "Discovery started.");
        }
        break;
    }
    case ESP_BT_GAP_RMT_SRVCS_EVT: {
        /*if (memcmp(param->rmt_srvcs.bda, p_dev->bda, ESP_BD_ADDR_LEN) == 0 &&
                p_dev->state == APP_GAP_STATE_SERVICE_DISCOVERING) {
            p_dev->state = APP_GAP_STATE_SERVICE_DISCOVER_COMPLETE;
            if (param->rmt_srvcs.stat == ESP_BT_STATUS_SUCCESS) {
                ESP_LOGI(GAP_TAG, "Services for device %s found", Utils::AddrToStr(p_dev->bda, bda_str, 18));
                for (int i = 0; i < param->rmt_srvcs.num_uuids; i++) {
                    esp_bt_uuid_t *u = param->rmt_srvcs.uuid_list + i;
                    ESP_LOGI(GAP_TAG, "--%s", Utils::UUIDToStr(u, uuid_str, 37));
                    // ESP_LOGI(GAP_TAG, "--%d", u->len);
                }
            } else {
                ESP_LOGI(GAP_TAG, "Services for device %s not found", Utils::AddrToStr(p_dev->bda, bda_str, 18));
            }
        }*/
        break;
    }
    case ESP_BT_GAP_RMT_SRVC_REC_EVT:
    default:
        ESP_LOGI(GAP_TAG, "Unhandled event: %d\n", event);
        break;
    }
    return;

}

/*
    Initializes and enables bluetooth
*/
void BtGap::startBluetooth(const char *device_name) {
    // Don't start if already started
    static uint8_t initialized = 0;
    if(initialized)
        return;
    initialized = 1;

    ESP_LOGI(GAP_TAG, "Starting Bluetooth...");

    // Error return value
    esp_err_t ret;

    // Releases BLE controller memory. BLE cannot be used after calling this.
    // ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    ESP_LOGI(GAP_TAG, "initialize controller...");
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(GAP_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // Enable the BT controller in classic mode (version < 4.0)
    ESP_LOGI(GAP_TAG, "enable controller...");
    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_BTDM)) != ESP_OK) {
        ESP_LOGE(GAP_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(GAP_TAG, "initialize bluedroid...");
    if ((ret = esp_bluedroid_init()) != ESP_OK) {
        ESP_LOGE(GAP_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(GAP_TAG, "enable bluedroid...");
    if ((ret = esp_bluedroid_enable()) != ESP_OK) {
        ESP_LOGE(GAP_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_IO;
    esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));
    /*
     * Set default parameters for Legacy Pairing
     * Use fixed pin code
     */
    esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_FIXED;
    esp_bt_pin_code_t pin_code;
    pin_code[0] = '1';
    pin_code[1] = '2';
    pin_code[2] = '3';
    pin_code[3] = '4';
    esp_bt_gap_set_pin(pin_type, 4, pin_code);

    // Set the device name
    esp_bt_dev_set_device_name(device_name);

    /* register GAP callback function */
    esp_bt_gap_register_callback(gapCallback);

    /* set discoverable and connectable mode, wait to be connected */
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);    

}

void BtGap::startDiscovery(uint8_t timeout) {
    esp_err_t ret;
    if((ret = esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, timeout, 0)) != ESP_OK) {
        ESP_LOGE(GAP_TAG, "%s start discovery failed: %s\n", __func__, esp_err_to_name(ret));
    }
}

void BtGap::stopDiscovery() {
    esp_err_t ret;
    if((ret = esp_bt_gap_cancel_discovery()) != ESP_OK) {
        ESP_LOGE(GAP_TAG, "%s stop discovery failed: %s\n", __func__, esp_err_to_name(ret));
    }
}

void BtGap::setDeviceFoundCallback(DeviceFoundCallback callback) {
    device_found_callback = callback;
}