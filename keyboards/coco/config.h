// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Default COMBO_TERM (50ms) is too tight for a 3-key Ctrl+Alt+digit chord
// on real hardware; give it more room to register all three keys together.
#define COMBO_TERM 500

// Combo key matching normally checks the keycode each matrix position
// resolves to on the currently active layer, but the "0" key resolves to
// a different keycode (plain KC_0 vs. the custom CC_0) depending on which
// keymap layer is active. Pin combo matching to layer 0 (KEYMAP_COCO) so
// it's evaluated consistently regardless of which layer is active.
#define COMBO_ONLY_FROM_LAYER 0
