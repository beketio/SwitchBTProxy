//
// Created by timur on 3/13/2021.
//

#include "gamepad.h"

GamepadData::GamepadData(uint8_t *map) {
    button_map = map;
}

uint8_t GamepadData::GetButton(uint8_t index) {
    return buttons & (1 << button_map[index]);
}

uint8_t GamepadData::GetAxis(uint8_t index) {
    return axis[index];
}

void GamepadData::SetButton(uint8_t index, uint8_t value) {
    uint8_t b_index = button_map[index];
    uint8_t current = buttons & (1 << b_index);
    if(!current && value)
        buttons |= (1 << b_index);
    else if(current && !value)
        buttons &= ~(1 << b_index);
}

void GamepadData::SetAxis(uint8_t index, uint8_t value) {
    axis[index] = value;
}

Gamepad::Gamepad() {

}