// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Layer 0 ("coco"): default layer, matches the real CoCo shift-symbol
// matrix (Lomont's docs write each entry as shifted/unshifted, e.g. "\"/2"
// means shift+2 sends ", not @ like a US host layout would normally send).
// Only the number-row/punctuation keys whose CoCo shift symbol differs from
// standard US QWERTY are overridden below via custom keycodes; keys 1,3,4,5
// and the comma/dot/slash keys already match US QWERTY natively.
//
// Layer 1 ("pc"): the original tested layout (US QWERTY shift symbols
// throughout), kept exactly as first flashed.
enum coco_keymaps {
    KEYMAP_COCO = 0,
    KEYMAP_PC   = 1,
};

// Custom keycodes for the number-row/punctuation keys whose CoCo shifted
// character doesn't match what the underlying US-layout keycode would
// normally send when shifted.
enum custom_keycodes {
    CC_2 = SAFE_RANGE, // 2 / "
    CC_6,              // 6 / &
    CC_7,              // 7 / '
    CC_8,              // 8 / (
    CC_9,              // 9 / )
    CC_0,              // 0 / 0 (shift has no effect)
    CC_COLN,           // : / *
    CC_MINS,           // - / =
    CC_SCLN,           // ; / +
    CC_SAFE_RANGE_END,
};

typedef struct {
    uint16_t base;  // base US-layout keycode to tap
    bool     shift; // whether to force shift on for that tap
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
    [CC_0    - SAFE_RANGE] = {{KC_0,    false}, {KC_0,    false}},
    [CC_COLN - SAFE_RANGE] = {{KC_SCLN, true},  {KC_8,    true}},
    [CC_MINS - SAFE_RANGE] = {{KC_MINS, false}, {KC_EQL,  false}},
    [CC_SCLN - SAFE_RANGE] = {{KC_SCLN, false}, {KC_EQL,  true}},
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode >= SAFE_RANGE && keycode < CC_SAFE_RANGE_END) {
        if (record->event.pressed) {
            const coco_sym_t          *sym  = &coco_symbols[keycode - SAFE_RANGE];
            uint8_t                    mods = get_mods();
            const coco_sym_variant_t  *v    = (mods & MOD_MASK_SHIFT) ? &sym->shifted : &sym->unshifted;
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
    return true;
}

// Ctrl+Alt+1 switches to the PC layout, Ctrl+Alt+0 switches back to the
// CoCo layout. Both chords work from either layer, since neither "0" nor
// "1" is overridden on layer 0 except "0" itself (CC_0), which is only
// ever needed to switch back while already on layer 1 (where the "0" key
// is still plain KC_0), so the combo definitions below match correctly.
enum combos {
    COMBO_TO_PC,
    COMBO_TO_COCO,
};

const uint16_t PROGMEM combo_to_pc[]   = {KC_LCTL, KC_LALT, KC_1, COMBO_END};
const uint16_t PROGMEM combo_to_coco[] = {KC_LCTL, KC_LALT, CC_0, COMBO_END};

combo_t key_combos[] = {
    [COMBO_TO_PC]   = COMBO(combo_to_pc, DF(KEYMAP_PC)),
    [COMBO_TO_COCO] = COMBO(combo_to_coco, DF(KEYMAP_COCO)),
};

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
        CC_0,       KC_1,    CC_2,     KC_3,    KC_4,    KC_5,    CC_6,    CC_7,
        CC_8,       CC_9,    CC_COLN,  CC_SCLN, KC_COMM, CC_MINS, KC_DOT,  KC_SLSH,
        KC_ENT,     KC_HOME, QK_GESC,  KC_LALT, KC_LCTL, KC_F1,   KC_F2,   KC_LSFT
    ),
    [KEYMAP_PC] = LAYOUT(
        LSFT(KC_2), KC_A,    KC_B,     KC_C,    KC_D,    KC_E,    KC_F,    KC_G,
        KC_H,       KC_I,    KC_J,     KC_K,    KC_L,    KC_M,    KC_N,    KC_O,
        KC_P,       KC_Q,    KC_R,     KC_S,    KC_T,    KC_U,    KC_V,    KC_W,
        KC_X,       KC_Y,    KC_Z,     KC_UP,   KC_DOWN, KC_LEFT, KC_RGHT, KC_SPC,
        KC_0,       KC_1,    KC_2,     KC_3,    KC_4,    KC_5,    KC_6,    KC_7,
        KC_8,       KC_9,    KC_SCLN,  KC_SCLN, KC_COMM, KC_MINS, KC_DOT,  KC_SLSH,
        KC_ENT,     KC_HOME, QK_GESC,  KC_LALT, KC_LCTL, KC_F1,   KC_F2,   KC_LSFT
    ),
};

// Atari-style digital joystick port (J2, DE9). Each direction/fire switch
// pulls its line to GND, wired straight to the Pico with no matrix diodes:
// J2 pin1=Up->GP27, pin2=Down->GP20, pin3=Left->GP21, pin4=Right->GP22,
// pin6=Fire->GP26, pin8=GND. Pins 5/7/9 (pot X, +5V, pot Y) are unwired.
// Works the same regardless of which keymap layer is active.
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
