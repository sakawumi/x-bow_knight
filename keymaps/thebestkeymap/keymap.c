/* Copyright 2021 Shulin Huang <mumu@x-bows.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "../../nicola.h"
#include "keymap_japanese.h"
#define x__x KC_NO

enum keymap_layers
{
    _NICOLA,
    _FUNC,
    _FUNC2,

};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_NICOLA] = LAYOUT_split_3x5_3(
        NG_Q,   NG_W, NG_E,  NG_R,  NG_T,             NG_Y,  NG_U,  NG_I,    NG_O,   NG_P,
        NG_A,   NG_S, NG_D,  NG_F,  NG_G,             NG_H,  NG_J,  NG_K,    NG_L,   NG_SCLN,
        NG_Z,   NG_X, NG_C,  NG_V,  NG_B,             NG_N, NG_M, NG_COMM  , NG_DOT, NG_SLSH,
        NG_SHFTL , CTL_T(KC_TAB), LT(_FUNC, KC_ESC),    LT(_FUNC2, KC_DEL), SFT_T(KC_BSPC), NG_SHFTR),
    [_FUNC] = LAYOUT_split_3x5_3(
        QK_BOOT, x__x,    x__x,    x__x,    KC_PSCR,        x__x, x__x,  x__x,  x__x,  x__x,
        x__x,    C(KC_A), C(KC_S), KC_DEL,  LCTL(JP_SLSH),  x__x, KC_LEFT, KC_DOWN,  KC_UP,  KC_RIGHT,
        KC_LALT, C(KC_Z), C(KC_X), C(KC_C), C(KC_V),        x__x, x__x,    x__x,  x__x,  x__x,
        KC_LGUI, KC_LCTL, x__x,                             x__x, KC_RSFT,  KC_RALT),
    [_FUNC2] = LAYOUT_split_3x5_3(
        x__x, x__x,  x__x,  x__x,  x__x,            x__x,  x__x,  x__x,  x__x,  x__x,
        KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,      KC_F6,  x__x,  x__x,  x__x,  x__x,
        KC_F7,  KC_F8,  KC_F9,  KC_F10,  KC_F11,    KC_F12,  x__x,  x__x,  x__x,  x__x,
        KC_LGUI,  KC_LCTL,  x__x,                         x__x,  KC_RSFT,  KC_RALT)
};
void matrix_init_user(void)
{
    // NICOLA親指シフト
    set_nicola(_NICOLA);
}
bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    // NICOLA親指シフト
    bool a = true;
    if(keycode >= NG_TOP && keycode <= NG_BOTTOM){
        a = process_nicola(keycode, record);
    }
    if (a == false)
        return false;
    // NICOLA親指シフト
    return true;
}
