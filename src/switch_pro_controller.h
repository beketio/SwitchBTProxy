//
// Created by timur on 3/13/2021.
//

#pragma once

#include "gamepad.h"

#define S_PRO_BUTTON1_Y         0
#define S_PRO_BUTTON1_X         1
#define S_PRO_BUTTON1_B         2
#define S_PRO_BUTTON1_A         3
// Joycon specific              4
// Joycon specific              5
#define S_PRO_BUTTON1_R         6
#define S_PRO_BUTTON1_ZR        7

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
#define S_PRO_BUTTON3_ZL        7

class SwitchProController: public Gamepad, public BtGamepad {

public:
    SwitchProController();
    void ReceiveBtData(uint8_t *data);
    void SendBtData(uint8_t *data);

};
