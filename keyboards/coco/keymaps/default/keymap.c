// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum coco_keymaps {
    KEYMAP_COCO = 0,
    KEYMAP_PC   = 1,
};

// CoCo shift-symbol overrides (e.g. shift+2 = ", not @). "0" and "1" are
// handled separately below since they also double as the layer-switch chord.
enum custom_keycodes {
    CC_2 = SAFE_RANGE,
    CC_6,
    CC_7,
    CC_8,
    CC_9,
    CC_COLN,
    CC_MINS,
    CC_SCLN,
    CC_SAFE_RANGE_END,
    CC_0,
    CC_1,
};

typedef struct {
    uint16_t base;
    bool     shift;
} coco_sym_variant_t;

typedef struct {
    coco_sym_variant_t unshifted;
    coco_sym_variant_t shifted;
} coco_sym_t;

static const coco_sym_t coco_symbols[CC_SAFE_RANGE_END - SAFE_RANGE] = {
    [CC_2    - SAFE_RANGE] = {{KC_2,    false}, {KC_QUOT, true}},
    [CC_6    - SAFE_RANGE] = {{KC_6,    false}, {KC_7,    true}},
    [CC_7    - SAFE_RANGE] = {{KC_7,    false}, {KC_QUOT, false}},
    [CC_8    - SAFE_RANGE] = {{KC_8,    false}, {KC_9,    true}},
    [CC_9    - SAFE_RANGE] = {{KC_9,    false}, {KC_0,    true}},
    [CC_COLN - SAFE_RANGE] = {{KC_SCLN, true},  {KC_8,    true}},
    [CC_MINS - SAFE_RANGE] = {{KC_MINS, false}, {KC_EQL,  false}},
    [CC_SCLN - SAFE_RANGE] = {{KC_SCLN, false}, {KC_EQL,  true}},
};

static bool ctrl_alt_held(void) {
    uint8_t mods = get_mods();
    return (mods & MOD_BIT(KC_LCTL)) && (mods & MOD_BIT(KC_LALT));
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode >= SAFE_RANGE && keycode < CC_SAFE_RANGE_END) {
        if (record->event.pressed) {
            const coco_sym_t         *sym  = &coco_symbols[keycode - SAFE_RANGE];
            uint8_t                   mods = get_mods();
            const coco_sym_variant_t *v    = (mods & MOD_MASK_SHIFT) ? &sym->shifted : &sym->unshifted;
            if (v->shift) {
                register_mods(MOD_BIT(KC_LSFT));
            } else {
                del_mods(MOD_MASK_SHIFT);
            }
            tap_code(v->base);
            set_mods(mods);
        }
        return false;
    }

    switch (keycode) {
        case CC_1:
            if (record->event.pressed) {
                if (ctrl_alt_held()) {
                    set_single_persistent_default_layer(KEYMAP_PC);
                } else {
                    register_code(KC_1);
                }
            } else {
                unregister_code(KC_1);
            }
            return false;
        case CC_0:
            if (record->event.pressed) {
                if (ctrl_alt_held()) {
                    set_single_persistent_default_layer(KEYMAP_COCO);
                } else if (get_highest_layer(default_layer_state) == KEYMAP_COCO) {
                    uint8_t mods = get_mods();
                    del_mods(MOD_MASK_SHIFT);
                    register_code(KC_0);
                    set_mods(mods);
                } else {
                    register_code(KC_0);
                }
            } else {
                unregister_code(KC_0);
            }
            return false;
    }
    return true;
}

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
    [KEYMAP_COCO] = LAYOUT(
        LSFT(KC_2), KC_A,    KC_B,     KC_C,    KC_D,    KC_E,    KC_F,    KC_G,
        KC_H,       KC_I,    KC_J,     KC_K,    KC_L,    KC_M,    KC_N,    KC_O,
        KC_P,       KC_Q,    KC_R,     KC_S,    KC_T,    KC_U,    KC_V,    KC_W,
        KC_X,       KC_Y,    KC_Z,     KC_UP,   KC_DOWN, KC_LEFT, KC_RGHT, KC_SPC,
        CC_0,       CC_1,    CC_2,     KC_3,    KC_4,    KC_5,    CC_6,    CC_7,
        CC_8,       CC_9,    CC_COLN,  CC_SCLN, KC_COMM, CC_MINS, KC_DOT,  KC_SLSH,
        KC_ENT,     KC_HOME, QK_GESC,  KC_LALT, KC_LCTL, KC_F1,   KC_F2,   KC_LSFT
    ),
    [KEYMAP_PC] = LAYOUT(
        LSFT(KC_2), KC_A,    KC_B,     KC_C,    KC_D,    KC_E,    KC_F,    KC_G,
        KC_H,       KC_I,    KC_J,     KC_K,    KC_L,    KC_M,    KC_N,    KC_O,
        KC_P,       KC_Q,    KC_R,     KC_S,    KC_T,    KC_U,    KC_V,    KC_W,
        KC_X,       KC_Y,    KC_Z,     KC_UP,   KC_DOWN, KC_LEFT, KC_RGHT, KC_SPC,
        CC_0,       CC_1,    KC_2,     KC_3,    KC_4,    KC_5,    KC_6,    KC_7,
        KC_8,       KC_9,    KC_SCLN,  KC_SCLN, KC_COMM, KC_MINS, KC_DOT,  KC_SLSH,
        KC_ENT,     KC_HOME, QK_GESC,  KC_LALT, KC_LCTL, KC_F1,   KC_F2,   KC_LSFT
    ),
};

// J2 (DE9) Atari-style digital joystick: pin1=Up->GP27, pin2=Down->GP20,
// pin3=Left->GP21, pin4=Right->GP22, pin6=Fire->GP26, pin8=GND.
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
