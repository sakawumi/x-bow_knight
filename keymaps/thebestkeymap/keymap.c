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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_NICOLA] = LAYOUT_split_3x5_3(
        NG_Q,   NG_W, NG_E,  NG_R,  NG_T,             NG_Y,  NG_U,  NG_I,    NG_O,   NG_P,
        NG_A,   NG_S, NG_D,  NG_F,  NG_G,             NG_H,  NG_J,  NG_K,    NG_L,   NG_SCLN,
        NG_Z,   NG_X, NG_C,  NG_V,  NG_B,             NG_N, NG_M, NG_COMM  , NG_DOT, NG_SLSH,
        NG_SHFTL , NG_SHFTL2, MS_BTN1,           KC_DEL, NG_SHFTR2, NG_SHFTR),
    [_MOVE] = LAYOUT_split_3x5_3(
        _______, _______,  _______,  _______,  _______,            _______,  KC_HOME,  KC_PGDN,  KC_PGUP,  KC_END,
        _______, _______,  _______,  _______,  _______,            _______,  KC_LEFT, KC_DOWN,  KC_UP,  KC_RIGHT,
        _______, _______,  _______,  _______,  _______,            _______, _______,  _______,  _______,  _______,
        _______, _______, _______,                             _______, _______,  _______),
    [_NUMBER] = LAYOUT_split_3x5_3(
        QK_BOOT, _______,  _______,  _______,  KC_PSCR,            _______,  KC_1,  KC_2,  KC_3,  _______,
        _______, _______,  _______,  _______,  _______,            _______,  KC_4,  KC_5,  KC_6,  _______,
        _______, _______,  _______,  _______,  _______,            _______,  KC_7,  KC_8,  KC_9,  KC_0,
        _______, _______, _______,                             _______, _______,  _______),
    [_GAME] = LAYOUT_split_3x5_3(
        KC_Q, KC_W, KC_E, KC_R, KC_T,            KC_Y, KC_U, KC_I, KC_O, KC_P,
        KC_A, KC_S, KC_D, KC_F, KC_G,            KC_H, KC_J, KC_K, KC_L, KC_SCLN,
        KC_Z, KC_X, KC_C, KC_V, KC_B,            KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH,
        KC_LALT, KC_LCTL, KC_LSFT,      KC_RCTL, KC_RSFT, KC_RALT),
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
