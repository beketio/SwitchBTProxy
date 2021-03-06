//
// Created by timur on 3/13/2021.
//

#pragma once

#include <cstdint>

/*
#define GAMEPAD_BUTTON_A        0
#define GAMEPAD_BUTTON_B        1
#define GAMEPAD_BUTTON_X        2
#define GAMEPAD_BUTTON_Y        3
#define GAMEPAD_BUTTON_START    4
#define GAMEPAD_BUTTON_SELECT   5
#define GAMEPAD_BUTTON_DPAD_U   6
#define GAMEPAD_BUTTON_DPAD_D   7
#define GAMEPAD_BUTTON_DPAD_L   8
#define GAMEPAD_BUTTON_DPAD_R   9
#define GAMEPAD_BUTTON_L        10
#define GAMEPAD_BUTTON_R        11
#define GAMEPAD_BUTTON_LS       12
#define GAMEPAD_BUTTON_RS       13
#define GAMEPAD_BUTTON_HOME     14
#define GAMEPAD_BUTTON_MEDIA    15

#define GAMEPAD_AXIS_L_STICK_X  0
#define GAMEPAD_AXIS_L_STICK_Y  1
#define GAMEPAD_AXIS_R_STICK_X  2
#define GAMEPAD_AXIS_R_STICK_Y  3
#define GAMEPAD_AXIS_LZ         4
#define GAMEPAD_AXIS_RZ         5
#define GAMEPAD_AXIS_GYRO_X     6
#define GAMEPAD_AXIS_GYRO_Y     7
#define GAMEPAD_AXIS_GYRO_Z     8
#define GAMEPAD_AXIS_ACCEL_X    9
#define GAMEPAD_AXIS_ACCEL_Y    10
#define GAMEPAD_AXIS_ACCEL_Z    11
*/

class GamepadData {
public:

    GamepadData(uint8_t *map);

    enum ButtonIndex : uint8_t {
        button_a =        0,
        button_b =        1,
        button_x =        2,
        button_y =        3,
        button_start =    4,
        button_select =   5,
        button_dpad_u =   6,
        button_dpad_d =   7,
        button_dpad_l =   8,
        button_dpad_r =   9,
        button_l =        10,
        button_r =        11,
        button_lz =       12,
        button_rz =       13,
        button_ls =       14,
        button_rs =       15,
        button_home =     16,
        button_media =    17
    };

    enum AxisIndex : uint8_t {
        axis_l_stick_x =  0,
        axis_l_stick_y =  1,
        axis_r_stick_x =  2,
        axis_r_stick_y =  3,
        axis_lz =         4,
        axis_rz =         5,
        axis_gyro_x =     6,
        axis_gyro_y =     7,
        axis_gyro_z =     8,
        axis_accel_x =    9,
        axis_accel_y =    10,
        axis_accel_z =    11
    };

    uint8_t getButton(ButtonIndex index);
    uint8_t getAxis(AxisIndex index);

    void setButton(ButtonIndex index, uint8_t value);
    void setAxis(AxisIndex index, uint8_t value);

    uint16_t axis[12];
    uint32_t buttons;

    uint8_t *buttons1 = ((uint8_t*) &buttons);
    uint8_t *buttons2 = ((uint8_t*) &buttons) + 1;
    uint8_t *buttons3 = ((uint8_t*) &buttons) + 2;
    uint8_t *buttons4 = ((uint8_t*) &buttons) + 3;

    uint8_t battery;

protected:
    uint8_t *button_map;

};

class Gamepad {

public:
    Gamepad();

protected:
    uint8_t button_map[32];

    GamepadData input = GamepadData(button_map);
    GamepadData output = GamepadData(button_map);

private:

};



