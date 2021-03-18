//
// Created by timur on 3/13/2021.
//

#include "switch_pro_controller.h"

SwitchProController::SwitchProController(esp_hidh_dev_t *hid_device_id) : BtHidInputDevice(hid_device_id) {
    button_map[GAMEPAD_BUTTON_A] = S_PRO_BUTTON1_A;
    button_map[GAMEPAD_BUTTON_B] = S_PRO_BUTTON1_B;
    button_map[GAMEPAD_BUTTON_X] = S_PRO_BUTTON1_X;
    button_map[GAMEPAD_BUTTON_Y] = S_PRO_BUTTON1_Y;
    button_map[GAMEPAD_BUTTON_START] = S_PRO_BUTTON2_PLUS + 8;
    button_map[GAMEPAD_BUTTON_SELECT] = S_PRO_BUTTON2_MINUS + 8;
    button_map[GAMEPAD_BUTTON_DPAD_U] = S_PRO_BUTTON3_DPAD_U + 16;
    button_map[GAMEPAD_BUTTON_DPAD_D] = S_PRO_BUTTON3_DPAD_D + 16;
    button_map[GAMEPAD_BUTTON_DPAD_L] = S_PRO_BUTTON3_DPAD_L + 16;
    button_map[GAMEPAD_BUTTON_DPAD_R] = S_PRO_BUTTON3_DPAD_R + 16;
    button_map[GAMEPAD_BUTTON_L] = S_PRO_BUTTON3_L + 16;
    button_map[GAMEPAD_BUTTON_R] = S_PRO_BUTTON1_R;
    button_map[GAMEPAD_BUTTON_LS] = S_PRO_BUTTON2_LS + 8;
    button_map[GAMEPAD_BUTTON_RS] = S_PRO_BUTTON2_RS + 8;
    button_map[GAMEPAD_BUTTON_HOME] = S_PRO_BUTTON2_HOME + 8;
    button_map[GAMEPAD_BUTTON_MEDIA] = S_PRO_BUTTON2_CAPTURE + 8;
}

void SwitchProController::onInputEvent(uint8_t *data, uint16_t length) {
    ESP_LOGI("PROCON", "Recieved input data");
}

void SwitchProController::onFeatureEvent(uint8_t *data, uint16_t length) {
    ESP_LOGI("PROCON", "Recieved feature data");
}

void SwitchProController::ReceiveBtData(uint8_t *data) {
    data[0] = 0;
    data[1] = 0; // timer?
    data[2] = 0; // Battery
    //buttons
    *output.buttons1 = data[3];
    *output.buttons2 = data[4];
    *output.buttons3 = data[5];
    //decode left stick
    output.axis[GAMEPAD_AXIS_L_STICK_X] = ((data[6] & 0xF0) >> 4) | ((data[7] << 4) & 0xF0);
    output.axis[GAMEPAD_AXIS_L_STICK_Y] = data[8];
    //decode right stick
    output.axis[GAMEPAD_AXIS_R_STICK_X] = ((data[9] & 0xF0) >> 4) | ((data[10] << 4) & 0xF0);
    output.axis[GAMEPAD_AXIS_L_STICK_Y] = data[11];
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
    data[6] = (output.axis[GAMEPAD_AXIS_L_STICK_X] << 4) & 0xF0;
    data[7] = (output.axis[GAMEPAD_AXIS_L_STICK_X] & 0xF0) >> 4;
    data[8] = output.axis[GAMEPAD_AXIS_L_STICK_Y];
    //encode right stick
    data[9] = (output.axis[GAMEPAD_AXIS_R_STICK_X] << 4) & 0xF0;
    data[10] = (output.axis[GAMEPAD_AXIS_R_STICK_X] & 0xF0) >> 4;
    data[11] = output.axis[GAMEPAD_AXIS_L_STICK_Y];
}