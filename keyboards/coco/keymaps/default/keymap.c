// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * Row/col order matches the stock CoCo keyboard PIA scan matrix
     * (columns strobed via $FF02, rows read via $FF00).
     *
     *       col0    col1    col2    col3    col4    col5    col6    col7
     * row0    @       A       B       C       D       E       F       G
     * row1    H       I       J       K       L       M       N       O
     * row2    P       Q       R       S       T       U       V       W
     * row3    X       Y       Z      Up      Down    Left    Right   Space
     * row4    0       1       2       3       4       5       6       7
     * row5    8       9       :       ;       ,       -       .       /
     * row6  Enter    Clr    Esc/Brk  Alt     Ctrl     F1      F2     Shift
     */
    [0] = LAYOUT(
        LSFT(KC_2), KC_A,    KC_B,     KC_C,    KC_D,    KC_E,    KC_F,    KC_G,
        KC_H,       KC_I,    KC_J,     KC_K,    KC_L,    KC_M,    KC_N,    KC_O,
        KC_P,       KC_Q,    KC_R,     KC_S,    KC_T,    KC_U,    KC_V,    KC_W,
        KC_X,       KC_Y,    KC_Z,     KC_UP,   KC_DOWN, KC_LEFT, KC_RGHT, KC_SPC,
        KC_0,       KC_1,    KC_2,     KC_3,    KC_4,    KC_5,    KC_6,    KC_7,
        KC_8,       KC_9,    KC_SCLN,  KC_SCLN, KC_COMM, KC_MINS, KC_DOT,  KC_SLSH,
        KC_ENT,     KC_HOME, QK_GESC,  KC_LALT, KC_LCTL, KC_F1,   KC_F2,   KC_LSFT
    )
};

// Atari-style digital joystick port (J2, DE9). Each direction/fire switch
// pulls its line to GND, wired straight to the Pico with no matrix diodes:
// J2 pin1=Up->GP27, pin2=Down->GP20, pin3=Left->GP21, pin4=Right->GP22,
// pin6=Fire->GP26, pin8=GND. Pins 5/7/9 (pot X, +5V, pot Y) are unwired.
enum joystick_input {
    JOY_UP = 0,
    JOY_DOWN,
    JOY_LEFT,
    JOY_RIGHT,
    JOY_FIRE,
    JOY_COUNT
};

static const pin_t    joystick_pins[JOY_COUNT]     = {GP27, GP20, GP21, GP22, GP26};
static const uint16_t joystick_keycodes[JOY_COUNT] = {KC_UP, KC_DOWN, KC_LEFT, KC_RGHT, KC_SPC};
static bool            joystick_pressed[JOY_COUNT]  = {false, false, false, false, false};

void matrix_init_user(void) {
    for (uint8_t i = 0; i < JOY_COUNT; i++) {
        gpio_set_pin_input_high(joystick_pins[i]);
    }
}

void matrix_scan_user(void) {
    for (uint8_t i = 0; i < JOY_COUNT; i++) {
        bool pressed = !gpio_read_pin(joystick_pins[i]);
        if (pressed != joystick_pressed[i]) {
            joystick_pressed[i] = pressed;
            if (pressed) {
                register_code16(joystick_keycodes[i]);
            } else {
                unregister_code16(joystick_keycodes[i]);
            }
        }
    }
}
