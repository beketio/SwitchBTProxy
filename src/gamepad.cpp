//
// Created by timur on 3/13/2021.
//

#include "gamepad.h"

GamepadData::GamepadData(uint8_t *map) {
    button_map = map;
}

uint8_t GamepadData::getButton(ButtonIndex index) {
    return (buttons >> button_map[index]) & 0x01;
}

uint8_t GamepadData::getAxis(AxisIndex index) {
    return axis[index];
}

void GamepadData::setButton(ButtonIndex index, uint8_t value) {
    uint8_t b_index = button_map[index];
    uint8_t current = (buttons >> b_index) & 0x01;
    if(!current && value)
        buttons |= (1ul << b_index);
    else if(current && !value)
        buttons &= ~(1ul << b_index);
}

void GamepadData::setAxis(AxisIndex index, uint8_t value) {
    axis[index] = value;
}

Gamepad::Gamepad() {

}