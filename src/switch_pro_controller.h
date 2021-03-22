//
// Created by timur on 3/13/2021.
//

#pragma once

#include "gamepad.h"
#include "bluetooth/bt_hid_input_device.h"

#define S_PRO_BUTTON1_Y         0
#define S_PRO_BUTTON1_X         1
#define S_PRO_BUTTON1_B         2
#define S_PRO_BUTTON1_A         3
// Joycon specific              4
// Joycon specific              5
#define S_PRO_BUTTON1_R         6
#define S_PRO_BUTTON1_RZ        7

#define S_PRO_BUTTON2_MINUS     0
#define S_PRO_BUTTON2_PLUS      1
#define S_PRO_BUTTON2_RS        2
#define S_PRO_BUTTON2_LS        3
#define S_PRO_BUTTON2_HOME      4
#define S_PRO_BUTTON2_CAPTURE   5
#define S_PRO_BUTTON2_MIN_MIN   6
#define S_PRO_BUTTON2_GRIP      7

#define S_PRO_BUTTON3_DPAD_D    0
#define S_PRO_BUTTON3_DPAD_U    1
#define S_PRO_BUTTON3_DPAD_R    2
#define S_PRO_BUTTON3_DPAD_L    3
// Joycon specific              4
// Joycon specific              5
#define S_PRO_BUTTON3_L         6
#define S_PRO_BUTTON3_LZ        7

class SwitchProController: public Gamepad, public BtHidInputDevice {

public:
    SwitchProController(esp_hidh_dev_t *hid_device_id);
    void onInputEvent(uint8_t report_id, uint8_t *data, uint16_t length);
    void onFeatureEvent(uint8_t report_id, uint8_t *data, uint16_t length);

private:
    void readStandardInput(uint8_t *data, uint16_t length);
    void readSimpleInput(uint8_t *data, uint16_t length);
    void readSubcommandData(uint8_t *data, uint16_t length);
    void readNfcData(uint8_t *data, uint16_t length);
    void readImuData(uint8_t *data, uint16_t length);

    void sendSubcommand(uint8_t command_id, uint8_t *data, uint16_t length);
    void SendBtData(uint8_t *data);

    xSemaphoreHandle subcommand_semaphore;
    uint8_t global_packet_num;
    

    /*
        For more information see this report. Special thanks to all the contributors.
        https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering/blob/master/bluetooth_hid_notes.md 
    */

    enum InputReportType : uint8_t {
        // Standard input reports with subcommand reply data.
        rinput_standard_subcommand = 0x21, 
         // NFC/IR MCU FW update input report.
        rinput_nfc_ir_fw = 0x23,
        // Standard input reports with IMU data.
        rinput_standard_imu = 0x30,
        // Standard input reports with NFC/IR MCU data.
        rinput_standard_nfc_ir = 0x31,
        // Unknown. Sends standard input reports.
        rinput_standard_unknown_32 = 0x32,
        // Unknown. Sends standard input reports.
        rinput_standard_unknown_33 = 0x33,
        // Simple input reports, provides the "normal controller" interface for the OS.
        rinput_simple = 0x3F
    };

    enum OutputReportType : uint8_t {
        // Normal subcommands, also includes rumble data.
        routput_subcommand = 0x01,
        // NFC/IR MCU FW Update packet.
        routput_nfc_ir_fw = 0x03,
        // Rumble only.
        routput_rumble = 0x10,
        // Request specific data from the NFC/IR MCU. Can also send rumble.
        routput_nfc_ir_rumble = 0x11,
        // Unknown. Does the same thing with 0x28 subcmd.
        routput_unknown_12 = 0x12
    };

    enum FeatureReportType : uint8_t {
        // Get last subcommand reply
        rfeature_last_subcommand = 0x02,
        // Enable OTA FW upgrade
        rfeature_enable_ota = 0x70,
        // Setup memory read
        rfeature_setup_mem_read = 0x71,
        // Memory read
        rfeature_mem_read = 0x72,
        // Memory sector erase
        rfeature_mem_sector_erase = 0x73,
        // Memory write
        rfeature_mem_write = 0x74,
        // Launch (Reboot)
        rfeature_launch_reboot = 0x75,
        rfeature_unknown_cc = 0xCC,
        rfeature_unknown_fe = 0xFE
    };

    /* 
        Subcommands have their own report.
        https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering/blob/master/bluetooth_hid_subcommands_notes.md
    */

    enum SubcommandType : uint8_t {
        // Get Only Controller State
        rsubcommand_controller_state = 0x00,
        // Bluetooth manual pairing
        rsubcommand_bt_manual_pairing = 0x01,
        // Request device info
        rsubcommand_request_device_info = 0x02,
        // Set input report mode
        rsubcommand_set_input_report_mode = 0x03,
        // Trigger buttons elapsed time
        rsubcommand_trigger_buttons_time = 0x04,
        // Get page list state
        rsubcommand_page_list_state = 0x05,
        // Set HCI state (disconnect/page/pair/turn off)
        rsubcommand_set_hci_state = 0x06,
        // Reset pairing info
        rsubcommand_reset_pairing = 0x07,
        // Set shipment low power state
        rsubcommand_set_shipment = 0x08,
        // SPI flash read
        rsubcommand_spi_flash_read = 0x10,
        // SPI flash write
        rsubcommand_spi_flash_write = 0x11,
        // SPI flash sector erase
        rsubcommand_spi_flash_erase = 0x12,
        // Reset NFC/IR MCU
        rsubcommand_reset_nfc_ir = 0x20,
        // Set NFC/IR MCU configuration
        rsubcommand_set_nfc_ir_conf = 0x21,
        // Set NFC/IR MCU state
        rsubcommand_set_nfc_ir_state = 0x22,
        // Set unknown data (fw 3.86 and up)
        rsubcommand_unknown_24 = 0x24,
        // Reset 0x24 unknown data (fw 3.86 and up)
        rsubcommand_unknown_25 = 0x25,
        // Set unknown NFC/IR MCU data
        rsubcommand_unknown_nfc_ir = 0x28,
        // Get x28 NFC/IR MCU data
        rsubcommand_unknown_28_nfc_ir = 0x29,
        // Set GPIO Pin Output value (2 @Port 2)
        rsubcommand_set_gpio_2 = 0x2A,
        // Get x29 NFC/IR MCU data
        rsubcommand_unknown_29_nfc_ir = 0x2B,
        // Set player lights
        rsubcommand_set_player_lights = 0x30,
        // Get player lights
        rsubcommand_get_player_lights = 0x31,
        // Set HOME Light
        rsubcommand_set_home_light = 0x38,
        // Enable IMU (6-Axis sensor)
        rsubcommand_enable_imu = 0x40,
        // Set IMU sensitivity
        rsubcommand_set_imu_sens = 0x41,
        // Write to IMU registers
        rsubcommand_write_imu_reg = 0x42,
        // Read IMU registers
        rsubcommand_read_imu_reg = 0x43,
        // Enable vibration
        rsubcommand_enable_vibration = 0x48,
        // Get regulated voltage
        rsubcommand_get_voltage = 0x50,
        // Set GPIO Pin Output value (7 & 15 @Port 1)
        rsubcommand_set_gpio_7_15 = 0x51,
        // Get GPIO Pin Input/Output value
        rsubcommand_get_gpio_value = 0x52
    };

};