/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

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

#include "quantum.h"

// aliases

#define JAP KC_LANGUAGE_1
#define ENG KC_LANGUAGE_2

#define CLICK KC_MS_BTN1
#define RCLICK KC_MS_BTN2

#define SCREENSHOT_PART LCMD(LSFT(KC_4))
#define SCREENSHOT_WHOLE LCMD(LSFT(KC_3))

// macro

enum custom_keycodes {
  KC_ESC_AND_ENG = SAFE_RANGE,
  CMD_CLICK
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t scroll_timer;

  switch(keycode) {
    case KC_ESC_AND_ENG:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESC) SS_TAP(X_LANGUAGE_2));
      }
      break;

    // click while holding cmd
    // for some reason `LCMD(CLICK)` did not work, so use a macro instead
    case CMD_CLICK:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LCMD) SS_DELAY(20) SS_TAP(X_BTN1) SS_DELAY(20) SS_UP(X_LCMD));
      }
      break;

    // enable scroll mode when `:` is held down
    case KC_COLON:
      if (record->event.pressed) {
        scroll_timer = timer_read();
        keyball_set_scroll_mode(true); // enable scroll mode
      } else {
        keyball_set_scroll_mode(false); // disable scroll mode
        if (timer_elapsed(scroll_timer) < TAPPING_TERM) {
          // key was used to input ":"
          SEND_STRING(":");
        }
      }
      // keypress was handled
      return false;
  }

  return true;
}

// ref: https://github.com/qmk/qmk_firmware/blob/master/docs/tap_hold.md#tapping-term
// permissive hold alone wasn't enough to prevent missed layer key presses
// (i.e. layer keys weren't triggering when I wanted them to trigger), so make the
// tapping term for the layer keys shorter.
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(1, CLICK):
            return 80;
        case LT(2, CLICK):
            return 80;
        default:
            return TAPPING_TERM;
    }
}

// ref: https://docs.qmk.fm/#/feature_pointing_device?id=how-to-enable
void keyboard_post_init_user(void) {
    set_auto_mouse_enable(true);
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default
  [0] = LAYOUT_universal(
    XXXXXXX   , KC_Q      , KC_W      , KC_E      , KC_R      , KC_T      ,                          KC_Y      , KC_U      , KC_I      , KC_O      , KC_P      , XXXXXXX   ,
    XXXXXXX   , KC_A      ,LCTL_T(KC_S),LALT_T(KC_D),LCMD_T(KC_F), KC_G   ,                          KC_H     ,LCMD_T(KC_J),LALT_T(KC_K),RCTL_T(KC_L),KC_COLON , XXXXXXX   ,
    XXXXXXX   , KC_Z      , KC_X      , KC_C      , KC_V      , KC_B      ,                          KC_N      , KC_M      , KC_COMM   , KC_DOT    , KC_SLASH  , XXXXXXX   ,
    XXXXXXX   , XXXXXXX   ,             CLICK     , MO(1)     , LSFT_T(KC_SPACE),                    RSFT_T(KC_ENT), MO(2) ,             _______   , _______   , _______
  ),

  [1] = LAYOUT_universal(
    XXXXXXX   , _______   , _______, LCMD(KC_LCBR), LCMD(KC_RCBR), _______,                          KC_0     , KC_1     , KC_2     , KC_3     , _______  , XXXXXXX  ,
    XXXXXXX   , _______   , KC_LCBR   , KC_DEL    , KC_BSPC   , KC_RCBR   ,                          KC_MINUS , KC_4     , KC_5     , KC_6     , _______  , XXXXXXX  ,
    XXXXXXX   , _______   , _______   , _______   , _______   , _______   ,                          KC_EQUAL , KC_7     , KC_8     , KC_9     , _______  , XXXXXXX  ,
    _______   , _______   ,             _______   , _______   , _______   ,                    KC_ESC_AND_ENG , LT(3,RCLICK) ,        _______  , _______  , _______
  ),

  [2] = LAYOUT_universal(
    XXXXXXX   , KC_EXCLAIM, KC_DQT    , KC_HASH  , KC_DOLLAR , KC_PERCENT ,                           KC_AMPR   , KC_ASTR   , KC_LPRN   , KC_RPRN   , KC_CIRC   , XXXXXXX   ,
    XXXXXXX   , KC_TAB    , KC_QUOTE  , ENG      , JAP       , KC_AT      ,                           KC_LEFT   , KC_DOWN   , KC_UP     , KC_RIGHT  , KC_SCLN   , XXXXXXX   ,
    XXXXXXX   , _______   , KC_GRAVE  , KC_TILDE , KC_PIPE   , _______    ,                           KC_UNDERSCORE, KC_PLUS, KC_LBRC   , KC_RBRC   , KC_BACKSLASH,XXXXXXX  ,
    _______   , _______   ,             _______  ,LT(3,RCLICK), _______   ,                           _______   , _______   ,             _______   , _______   , _______
  ),

  [3] = LAYOUT_universal (
    XXXXXXX   , _______   , _______   , _______   , _______   , AML_TO    ,                           _______   , _______   , _______   , _______   , _______   , XXXXXXX   ,
    XXXXXXX   , _______   , _______   , _______   , _______   , SCREENSHOT_PART,                   KC_MS_WH_LEFT, _______   , _______,KC_MS_WH_RIGHT, _______   , XXXXXXX   ,
    XXXXXXX   , _______   , _______   , _______   , _______   , SCREENSHOT_WHOLE,                     _______   , _______   , _______   , _______   , _______   , XXXXXXX   ,
    XXXXXXX   , _______   , _______   ,             _______   , _______   ,                           _______   , _______   ,             _______   , _______   , XXXXXXX
  ),

  // auto mouse layer
  [4] = LAYOUT_universal (
    XXXXXXX   , _______   , _______   , _______   , _______   , _______   ,                           CMD_CLICK , _______   , _______   , _______   , _______   , XXXXXXX   ,
    XXXXXXX   , _______   , _______   , _______   , _______   , _______   ,                           CLICK     , _______   , _______   , _______   , _______   , XXXXXXX   ,
    XXXXXXX   , _______   , _______   , _______   , _______   , _______   ,                           RCLICK    , _______   , _______   , _______   , _______   , XXXXXXX   ,
    XXXXXXX   , _______   , _______   ,             _______   , _______   ,                           _______   , _______   ,             _______   , _______   , XXXXXXX
  ),
};
// clang-format on

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
