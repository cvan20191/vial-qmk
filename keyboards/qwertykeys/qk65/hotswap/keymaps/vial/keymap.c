/*
Copyright 2022 qwertykeys

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include QMK_KEYBOARD_H
#include "features/achordion.h"


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT_65_ansi_blocker(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
        KC_LCTL, MT(MOD_LSFT,KC_A),    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    MT(MOD_RSFT,KC_SCLN), KC_QUOT,          KC_ENT,  KC_PGDN,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   MO(1),
        KC_LCTL, KC_LALT, KC_LGUI,                            KC_SPC,                    KC_RALT, KC_RCTL,          KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [1] = LAYOUT_65_ansi_blocker(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS, QK_BOOT,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_END,
        KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,                   KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS
    )

};

const key_override_t ctrl_m_enter_override = ko_make_basic(MOD_MASK_CTRL, KC_M, KC_ENT);

const key_override_t **my_key_overrides = (const key_override_t *[]){
    &ctrl_m_enter_override,
    NULL
};


const uint16_t PROGMEM kl_combos[] = {KC_K, KC_L, COMBO_END};
combo_t my_key_combos[] = {
    COMBO(kl_combos, KC_BSPC),
};

//achordion

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) { return false; }

  return true;
}


void matrix_scan_user(void) {
  achordion_task(); // Important: Call the Achordion task
}



bool is_override_key(uint16_t keycode) {
  return (keycode == KC_9 || keycode == KC_0 ||
          keycode == KC_MINS || keycode == KC_EQL|| keycode == KC_QUOT || 
          keycode == MT(MOD_RSFT,KC_SCLN) || keycode == KC_LBRC || keycode == KC_RBRC || 
          keycode == KC_BSLS || keycode == KC_COMM || keycode == KC_DOT || keycode == KC_SLSH);
}

// Achordion chord condition
bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t* tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t* other_record) {


  if (tap_hold_keycode == MT(MOD_LSFT,KC_A)) {
    if (is_override_key(other_keycode)) {
      return true; // Treat "A" as held (modifier active)
    } else {
      return false; // Treat "A" as tapped 
    }
  } 

  return achordion_opposite_hands(tap_hold_record, other_record); 
}

uint16_t achordion_streak_timeout(uint16_t tap_hold_keycode) {
  if (IS_QK_LAYER_TAP(tap_hold_keycode)) {
    return 0;  // Disable streak detection on layer-tap keys.
  }

  // Otherwise, tap_hold_keycode is a mod-tap key.
  uint8_t mod = mod_config(QK_MOD_TAP_GET_MODS(tap_hold_keycode));
  if ((mod & MOD_LSFT) != 0) {
    return 50;  // A shorter streak timeout for Shift mod-tap keys.
  } else {
    return 120;  // A longer timeout otherwise.
  }
}

