//
// Created by timur on 3/13/2021.
//

#include "switch_pro_controller.h"

SwitchProController::SwitchProController(esp_hidh_dev_t *hid_device_id) : BtHidInputDevice(hid_device_id) {
    button_map[GamepadData::button_a] = S_PRO_BUTTON1_A;
    button_map[GamepadData::button_b] = S_PRO_BUTTON1_B;
    button_map[GamepadData::button_x] = S_PRO_BUTTON1_X;
    button_map[GamepadData::button_y] = S_PRO_BUTTON1_Y;
    button_map[GamepadData::button_start] = S_PRO_BUTTON2_PLUS + 8;
    button_map[GamepadData::button_select] = S_PRO_BUTTON2_MINUS + 8;
    button_map[GamepadData::button_dpad_u] = S_PRO_BUTTON3_DPAD_U + 16;
    button_map[GamepadData::button_dpad_d] = S_PRO_BUTTON3_DPAD_D + 16;
    button_map[GamepadData::button_dpad_l] = S_PRO_BUTTON3_DPAD_L + 16;
    button_map[GamepadData::button_dpad_r] = S_PRO_BUTTON3_DPAD_R + 16;
    button_map[GamepadData::button_l] = S_PRO_BUTTON3_L + 16;
    button_map[GamepadData::button_r] = S_PRO_BUTTON1_R;
    button_map[GamepadData::button_lz] = S_PRO_BUTTON3_LZ + 16;
    button_map[GamepadData::button_rz] = S_PRO_BUTTON1_RZ;
    button_map[GamepadData::button_ls] = S_PRO_BUTTON2_LS + 8;
    button_map[GamepadData::button_rs] = S_PRO_BUTTON2_RS + 8;
    button_map[GamepadData::button_home] = S_PRO_BUTTON2_HOME + 8;
    button_map[GamepadData::button_media] = S_PRO_BUTTON2_CAPTURE + 8;

    global_packet_num = 0;

    subcommand_semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(subcommand_semaphore);

    // Send command to set lights to P1
    uint8_t player_lights[] = { 0x01 };  // solid P1
    sendSubcommand(rsubcommand_set_player_lights, player_lights, 1);

    /*player_lights[0] = 2;
    sendSubcommand(rsubcommand_set_player_lights, player_lights, 1);
    player_lights[0] = 4;
    sendSubcommand(rsubcommand_set_player_lights, player_lights, 1);
    player_lights[0] = 8;
    sendSubcommand(rsubcommand_set_player_lights, player_lights, 1);
    player_lights[0] = 1;
    sendSubcommand(rsubcommand_set_player_lights, player_lights, 1);*/

    // Send command to set lights to P2
    //player_lights[0] = 0x02;  // solid P1
    //sendSubcommand(rsubcommand_set_player_lights, player_lights, 1);
}

void SwitchProController::onInputEvent(uint8_t report_id, uint8_t *data, uint16_t length) {
    static uint8_t count = 0;
    count++;
    if(count % 60 == 0) {
        ESP_LOGI("PROCON", "B1: %x B2: %x B3: %x", *input.buttons1, *input.buttons2, *input.buttons3);
        ESP_LOGI("PROCON", "RX: %d RY: %d", input.axis[GamepadData::axis_r_stick_x], input.axis[GamepadData::axis_r_stick_y]);
        ESP_LOGI("PROCON", "LX: %d LY: %d", input.axis[GamepadData::axis_l_stick_x], input.axis[GamepadData::axis_l_stick_y]);
    }
    //ESP_LOGI("PROCON", "Recieved input data with id %d and id %d and length %d", report_id, data[0], length);
    //if(length >= 11)
    //readStandardInput(data);
    //ESP_LOGI("PROCON", "B1: %x B2: %x RX: %d RY: %d", *output.buttons1, *output.buttons2, output.axis[GAMEPAD_AXIS_R_STICK_X], output.axis[GAMEPAD_AXIS_R_STICK_Y]);
    //ESP_LOGI("PROCON", "B1: %x", *output.buttons1);

    static int waitiguess = 0;
    if(waitiguess == 30) {
        // set to full reports
        uint8_t input_mode[] = { 0x30 };
        sendSubcommand(rsubcommand_set_input_report_mode, input_mode, 1);
        waitiguess++;
    } else if(waitiguess < 30) {
        waitiguess++;
    }


    switch(report_id) {
    case rinput_standard_subcommand:
        xSemaphoreGive(subcommand_semaphore);
        if(length < 15) {
            ESP_LOGI("PROCON", "subcommand resp too short!");
        }
        ESP_LOGI("PROCON", "subcommand ack: 0x%x to command 0x%x", data[13], data[14]);
        break;
    case rinput_standard_imu:
    case rinput_standard_nfc_ir:
    case rinput_standard_unknown_32:
    case rinput_standard_unknown_33:
        readStandardInput(data, length);
        break;
    case rinput_simple:
        readSimpleInput(data, length);
        break;
    case rinput_nfc_ir_fw:
    default:
        ESP_LOGW("PROCON", "Unhandled input report. ID: %d (0x%x)", report_id, report_id);
        break;
    }

    if(input.getButton(GamepadData::button_select) && input.getButton(GamepadData::button_home)) {
        // Turn off controller
        uint8_t hci_state[] = { 0x00 };
        sendSubcommand(rsubcommand_set_hci_state, hci_state, 1);
    }
}

void SwitchProController::onFeatureEvent(uint8_t report_id, uint8_t *data, uint16_t length) {
    ESP_LOGI("PROCON", "Recieved feature data");

    switch(report_id) {
        case rfeature_last_subcommand:
        case rfeature_enable_ota:
        case rfeature_setup_mem_read:
        case rfeature_mem_read:
        case rfeature_mem_sector_erase:
        case rfeature_mem_write:
        case rfeature_launch_reboot:
        case rfeature_unknown_cc:
        case rfeature_unknown_fe:
        default:
            ESP_LOGW("PROCON", "Unhandled feature report. ID: %d (0x%x)", report_id, report_id);
    }
}

void SwitchProController::readStandardInput(uint8_t *data, uint16_t length) {
    //data[0] Input report id
    //data[1] Timer
    //data[2] Battery level & connection info

    //buttons
    *input.buttons1 = data[3];
    *input.buttons2 = data[4];
    *input.buttons3 = data[5];

    //decode left stick (12 bits)
    input.axis[GamepadData::axis_l_stick_x] = data[6] | ((data[7] & 0xF) << 8);
    input.axis[GamepadData::axis_l_stick_y] = (data[7] >> 4) | (data[8] << 4);

    //decode right stick (12 bits)
    input.axis[GamepadData::axis_r_stick_x] = data[9] | ((data[10] & 0xF) << 8);
    input.axis[GamepadData::axis_r_stick_y] = (data[10] >> 4) | (data[11] << 4);
}

void SwitchProController::readSimpleInput(uint8_t *data, uint16_t length) {
    // Not supported
    //data[0] Input report id

    input.buttons = 0;

    input.setButton(GamepadData::button_b, data[1] & 0x01);
    input.setButton(GamepadData::button_a, data[1] & 0x02);
    input.setButton(GamepadData::button_x, data[1] & 0x04);
    input.setButton(GamepadData::button_y, data[1] & 0x08);
    input.setButton(GamepadData::button_l, data[1] & 0x10);
    input.setButton(GamepadData::button_r, data[1] & 0x20);
    input.setButton(GamepadData::button_lz, data[1] & 0x40);
    input.setButton(GamepadData::button_rz, data[1] & 0x80);

    input.setButton(GamepadData::button_select, data[2] & 0x01);
    input.setButton(GamepadData::button_start, data[2] & 0x02);
    input.setButton(GamepadData::button_ls, data[2] & 0x04);
    input.setButton(GamepadData::button_rs, data[2] & 0x08);
    input.setButton(GamepadData::button_home, data[2] & 0x10);
    input.setButton(GamepadData::button_media, data[2] & 0x20);
    // ??
    // ??
    
    input.setButton(GamepadData::button_dpad_u, data[3] <= 0x01 || data[3] == 0x07);
    input.setButton(GamepadData::button_dpad_l, data[3] <= 0x03 && data[3] >= 0x01);
    input.setButton(GamepadData::button_dpad_d, data[3] <= 0x05 && data[3] >= 0x03);
    input.setButton(GamepadData::button_dpad_r, data[3] <= 0x07 && data[3] >= 0x05);

    //decode left stick (as 16 bit int)
    input.axis[GamepadData::axis_l_stick_x] = (data[4] >> 4) | (data[5] << 4);
    input.axis[GamepadData::axis_l_stick_y] = (data[6] >> 4) | (data[7] << 4);

    //decode right stick (as 16 bit int)
    input.axis[GamepadData::axis_r_stick_x] = (data[8] >> 4) | (data[9] << 4);
    input.axis[GamepadData::axis_r_stick_y] = 0x0FFF - ((data[10] >> 4) | (data[11] << 4));
}

void SwitchProController::readSubcommandData(uint8_t *data, uint16_t length) {
    
}

void SwitchProController::readNfcData(uint8_t *data, uint16_t length) {

}

void SwitchProController::readImuData(uint8_t *data, uint16_t length) {

}

void SwitchProController::sendSubcommand(uint8_t command_id, uint8_t *data, uint16_t length) {
    if(xSemaphoreTake(subcommand_semaphore, pdMS_TO_TICKS(200)) != pdTRUE) {
        ESP_LOGE("PROCON", "Last subcommand never sent ACK");
    }

    if(length > 0x35) {
        ESP_LOGE("PROCON", "Subcommand data exceeds max length");
        return;
    }
    uint8_t buf[0x40] = {0};
    buf[0] = routput_subcommand;
    buf[1] = global_packet_num;
    // buf[2-9] Rumble data goes here 
    /*buf[2] = 0x04;
    buf[3] = 0;
    buf[4] = 0x01;
    buf[5] = 0x40;
    buf[6] = 0x04;
    buf[7] = 0;
    buf[8] = 0x01;
    buf[9] = 0x40;*/


    buf[10] = command_id;
    memcpy(buf + 11, data, length);

    // Increment by 1 for each packet sent. It loops in 0x0 - 0xF range.
    global_packet_num++;
    if(global_packet_num > 0x0F)
        global_packet_num = 0;

    sendOutputReport(routput_subcommand, buf, 11 + length);

    ESP_LOGI("PROCON", "Sending subcom with id 0x%x and data 0x%x", buf[10], buf[11]);
}

void SwitchProController::SendBtData(uint8_t *data) {
    data[0] = 0;
    data[1] = 0; // timer?
    data[2] = 0; // Battery
    //buttons
    data[3] = *output.buttons1;
    data[4] = *output.buttons2;
    data[5] = *output.buttons3;
    //encode left stick
    data[6] = (output.axis[GamepadData::axis_l_stick_x] << 4) & 0xF0;
    data[7] = (output.axis[GamepadData::axis_l_stick_x] & 0xF0) >> 4;
    data[8] = output.axis[GamepadData::axis_l_stick_y];
    //encode right stick
    data[9] = (output.axis[GamepadData::axis_r_stick_x] << 4) & 0xF0;
    data[10] = (output.axis[GamepadData::axis_r_stick_x] & 0xF0) >> 4;
    data[11] = output.axis[GamepadData::axis_l_stick_y];
}