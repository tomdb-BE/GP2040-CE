/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2024 OpenStickCommunity (gp2040-ce.info)
 */

#ifndef MK2CABA_BOARD_CONFIG_H_
#define MK2CABA_BOARD_CONFIG_H_

#include "enums.pb.h"
#include "class/hid/hid.h"

#define BOARD_CONFIG_LABEL "MK2CabA"

// Main pin mapping Configuration
//                                                  // GP2040 | Xinput | Switch  | PS3/4/5  | Dinput | Arcade |
#define GPIO_PIN_00 GpioAction::BUTTON_PRESS_LEFT   // LEFT   | LEFT   | LEFT    | LEFT     | LEFT   | LEFT   |
#define GPIO_PIN_01 GpioAction::BUTTON_PRESS_RIGHT  // RIGHT  | RIGHT  | RIGHT   | RIGHT    | RIGHT  | RIGHT  |	
#define GPIO_PIN_02 GpioAction::BUTTON_PRESS_DOWN   // DOWN   | DOWN   | DOWN    | DOWN     | DOWN   | DOWN   |
#define GPIO_PIN_03 GpioAction::BUTTON_PRESS_UP     // UP     | UP     | UP      | UP       | UP     | UP     |
#define GPIO_PIN_06 GpioAction::BUTTON_PRESS_L3     // L3     | LS     | LS      | L3       | 11     | LS     |
#define GPIO_PIN_07 GpioAction::BUTTON_PRESS_R1     // R1     | RB     | R       | R1       | 6      | P3     |
#define GPIO_PIN_08 GpioAction::BUTTON_PRESS_B1     // B1     | A      | B       | Cross    | 2      | K1     |
#define GPIO_PIN_09 GpioAction::BUTTON_PRESS_B2     // B2     | B      | A       | Circle   | 3      | K2     |
#define GPIO_PIN_10 GpioAction::BUTTON_PRESS_L1     // L1     | LB     | L       | L1       | 5      | P4     |
#define GPIO_PIN_11 GpioAction::BUTTON_PRESS_R3     // R3     | RS     | RS      | R3       | 12     | RS     |
#define GPIO_PIN_12 GpioAction::BUTTON_PRESS_B3     // B3     | X      | Y       | Square   | 1      | P1     |
#define GPIO_PIN_13 GpioAction::BUTTON_PRESS_B4     // B4     | Y      | X       | Triangle | 4      | P2     |
#define GPIO_PIN_16 GpioAction::BUTTON_PRESS_S2     // S2     | Start  | Plus    | Start    | 10     | Start  |
#define GPIO_PIN_19 GpioAction::BUTTON_PRESS_A2     // A2     | ~      | Capture | ~        | 14     | ~      |
#define GPIO_PIN_20 GpioAction::BUTTON_PRESS_S1     // S1     | Back   | Minus   | Select   | 9      | Coin   |
#define GPIO_PIN_26 GpioAction::BUTTON_PRESS_A1     // A1     | Guide  | Home    | PS       | 13     | ~      |
#define GPIO_PIN_27 GpioAction::BUTTON_PRESS_A4     // A4     | ~      | ~       | ~        | 16     | ~      |

// Setting GPIO pins to assigned by add-on
//
#define GPIO_PIN_14 GpioAction::ASSIGNED_TO_ADDON
#define GPIO_PIN_15 GpioAction::ASSIGNED_TO_ADDON
#define GPIO_PIN_17 GpioAction::ASSIGNED_TO_ADDON
#define GPIO_PIN_18 GpioAction::ASSIGNED_TO_ADDON
#define GPIO_PIN_21 GpioAction::ASSIGNED_TO_ADDON
#define GPIO_PIN_22 GpioAction::ASSIGNED_TO_ADDON
#define GPIO_PIN_28 GpioAction::ASSIGNED_TO_ADDON

// Keyboard Mapping Configuration
//                                            // GP2040 | Xinput | Switch  | PS3/4/5  | Dinput | Arcade |
#define KEY_DPAD_UP     HID_KEY_ARROW_UP      // UP     | UP     | UP      | UP       | UP     | UP     |
#define KEY_DPAD_DOWN   HID_KEY_ARROW_DOWN    // DOWN   | DOWN   | DOWN    | DOWN     | DOWN   | DOWN   |
#define KEY_DPAD_RIGHT  HID_KEY_ARROW_RIGHT   // RIGHT  | RIGHT  | RIGHT   | RIGHT    | RIGHT  | RIGHT  |
#define KEY_DPAD_LEFT   HID_KEY_ARROW_LEFT    // LEFT   | LEFT   | LEFT    | LEFT     | LEFT   | LEFT   |
#define KEY_BUTTON_B1   HID_KEY_SHIFT_LEFT    // B1     | A      | B       | Cross    | 2      | K1     |
#define KEY_BUTTON_B2   HID_KEY_Z             // B2     | B      | A       | Circle   | 3      | K2     |
#define KEY_BUTTON_R2   HID_KEY_X             // R2     | RT     | ZR      | R2       | 8      | K3     |
#define KEY_BUTTON_L2   HID_KEY_V             // L2     | LT     | ZL      | L2       | 7      | K4     |
#define KEY_BUTTON_B3   HID_KEY_CONTROL_LEFT  // B3     | X      | Y       | Square   | 1      | P1     |
#define KEY_BUTTON_B4   HID_KEY_ALT_LEFT      // B4     | Y      | X       | Triangle | 4      | P2     |
#define KEY_BUTTON_R1   HID_KEY_SPACE         // R1     | RB     | R       | R1       | 6      | P3     |
#define KEY_BUTTON_L1   HID_KEY_C             // L1     | LB     | L       | L1       | 5      | P4     |
#define KEY_BUTTON_S1   HID_KEY_5             // S1     | Back   | Minus   | Select   | 9      | Coin   |
#define KEY_BUTTON_S2   HID_KEY_1             // S2     | Start  | Plus    | Start    | 10     | Start  |
#define KEY_BUTTON_L3   HID_KEY_EQUAL         // L3     | LS     | LS      | L3       | 11     | LS     |
#define KEY_BUTTON_R3   HID_KEY_MINUS         // R3     | RS     | RS      | R3       | 12     | RS     |
#define KEY_BUTTON_A1   HID_KEY_9             // A1     | Guide  | Home    | PS       | 13     | ~      |
#define KEY_BUTTON_A2   HID_KEY_F2            // A2     | ~      | Capture | ~        | 14     | ~      |
#define KEY_BUTTON_FN   -1                    // Hotkey Function                                        |

#define TURBO_ENABLED 0
#define TURBO_LED_PIN -1

#define BOOTSEL_BUTTON_ENABLED 1
#define BOOTSEL_BUTTON_MASK GAMEPAD_MASK_A4

#define BOARD_LEDS_PIN -1
#define LED_BRIGHTNESS_MAXIMUM 100
#define LED_BRIGHTNESS_STEPS 5
#define LED_FORMAT LED_FORMAT_GRB
#define LEDS_PER_PIXEL 1

#define LEDS_DPAD_LEFT   0
#define LEDS_DPAD_DOWN   1
#define LEDS_DPAD_RIGHT  2
#define LEDS_DPAD_UP     3
#define LEDS_BUTTON_B3   4
#define LEDS_BUTTON_B4   5
#define LEDS_BUTTON_R1   6
#define LEDS_BUTTON_L1   7
#define LEDS_BUTTON_B1   8
#define LEDS_BUTTON_B2   9
#define LEDS_BUTTON_R2   10
#define LEDS_BUTTON_L2   11
#define LEDS_BUTTON_A1   12
#define LEDS_BUTTON_L3   13
#define LEDS_BUTTON_R3   14
#define LEDS_BUTTON_A2   15

#define HAS_I2C_DISPLAY 0
#define I2C0_ENABLED 0
#define I2C0_PIN_SDA -1
#define I2C0_PIN_SCL -1
#define I2C1_ENABLED 0
#define I2C1_PIN_SDA -1
#define I2C1_PIN_SCL -1

#define COINLEDS_ENABLED 1
#define COINLEDS_START_PIN_1 17
#define COINLEDS_START_PIN_2 18
#define COINLEDS_START_PIN_3 -1
#define COINLEDS_START_PIN_4 -1
#define COINLEDS_COIN_PIN_1 21
#define COINLEDS_COIN_PIN_2 22
#define COINLEDS_COIN_PIN_3 -1
#define COINLEDS_COIN_PIN_4 -1
#define COINLEDS_MARQUEE_PIN 28
#define COINLEDS_EXT_START_PIN 14
#define COINLEDS_EXT_COIN_PIN 15
#define COINLEDS_START_MASK_1 GAMEPAD_MASK_S2
#define COINLEDS_START_MASK_2 GAMEPAD_MASK_A2
#define COINLEDS_START_MASK_3 0
#define COINLEDS_START_MASK_4 0
#define COINLEDS_COIN_MASK_1 GAMEPAD_MASK_S1
#define COINLEDS_COIN_MASK_2 GAMEPAD_MASK_A1
#define COINLEDS_COIN_MASK_3 0
#define COINLEDS_COIN_MASK_4 0
#define COINLEDS_EXT_START_MASK GAMEPAD_MASK_A2
#define COINLEDS_EXT_COIN_MASK GAMEPAD_MASK_A1
#define COINLEDS_START_BRIGHTNESS 100
#define COINLEDS_COIN_BRIGHTNESS 100
#define COINLEDS_MARQUEE_BRIGHTNESS 100

#define PCCONTROL_ENABLED 0
#define PCCONTROL_POWER_PIN -1
#define PCCONTROL_SWITCH_MASK 0
#define PCCONTROL_BUTTON_MASK_1 0
#define PCCONTROL_BUTTON_MASK_2 0

#define Z680_ENABLED 0
#define Z680_POWER_PIN -1
#define Z680_VOLUME_UP_PIN -1
#define Z680_VOLUME_DOWN_PIN -1
#define Z680_MUTE_PIN -1
#define Z680_POWER_STATE_PIN -1
#define Z680_BUTTON_MASK 0

#define BUTTON_LAYOUT BUTTON_LAYOUT_STICKLESS
#define BUTTON_LAYOUT_RIGHT BUTTON_LAYOUT_STICKLESSB

#endif