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

#define x___x KC_TRNS

enum keymap_layers
{
    _DVORAK,
    _NICOLA,
    _VI,
    _QWERTY,
    _FUNC
};
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Keymap VANILLA: (Base Layer) Default Layer
     *
     * |---------------------------------------------------------------------------------------------------------------------------------|
     * | Esc |  F1  |  F2  |  F3  |  F4  |      F5  |  F6  |  F7  |  F8  |      F9  |  F10 |  F11 |  F12 |   Delete   |    Prtsc    |
     * |---------------------------------------------------------------------------------------------------------------------------------|
     * |  ~  |     1   |   2   |   3   |   4   |    5      |       6    |    7    |    8   |   9  |   0  |   -  |  =  |  Backspace  |
     * |---------------------------------------------------------------------------------------------------------------------------------|
     * | Tab |   Q    |    W   |   E  |   R  |   T  |            |    Y   |    U   |    I  |   O  |   P  |   [  |  ]  |   \  | PgUp |
     * |---------------------------------------------------------------------------------------------------------------------------------|
     * | Ctl |   A   |   S   |   D  |   F  |   G  |      Bksp      |    H  |    J   |   K  |   L  |   ;  |  '"  |    Enter   | PgDn |
     * |---------------------------------------------------------------------------------------------------------------------------------|
     * |Shift|   Z  |   X  |   C  |   V  |   B  |       Enter       |    N  |    M   |  ,  |   .  |  /?  | Shift|      |  Up |
     * |---------------------------------------------------------------------------------------------------------------------------------|
     * |Ctrl | GUI |     Alter   |    Space   |   Ctrl   |   Shift   |     Space     |    Alter   |  FN  | Ctrl | Lft  |  Dn |  Rig |
     * |---------------------------------------------------------------------------------------------------------------------------------|
     */
    [_DVORAK] = LAYOUT(
        KC_ESC,    KC_F1,   KC_F2,   KC_F3,  KC_F4, KC_F5,         KC_F6, KC_F7, KC_F8, KC_F9, KC_F10,  KC_F11,  KC_F12,  KC_DEL, KC_PSCR,
        KC_GRV,    KC_1,    KC_2,    KC_3,   KC_4,  KC_5,          KC_6,  KC_7,  KC_8,  KC_9,  KC_0,    KC_LBRC, KC_RBRC, KC_BSPC,
        KC_TAB,    KC_QUOT, KC_COMM, KC_DOT, KC_P,  KC_Y,          KC_F,  KC_G,  KC_C,  KC_R,  KC_L,    KC_SLSH, KC_EQL,  KC_BSLS, KC_PGUP,
        CH_EN,     KC_A,    KC_O,    KC_E,   KC_U,  KC_I, KC_BSPC, KC_D,  KC_H,  KC_T,  KC_N,  KC_S,    KC_MINS, KC_ENT, KC_PGDN,
        KC_LSFT,   KC_SCLN, KC_Q,    KC_J,   KC_K , KC_X, KC_ENT,  KC_B,  KC_M,  KC_W,  KC_V,  KC_Z,    KC_RSFT, KC_UP,
        MO(_FUNC), KC_LGUI, KC_LALT, SFT_T(KC_SPC), KC_LCTL,    KC_RSFT, SFT_T(KC_ENT), CH_JP, KC_RCTL, MO(_FUNC), KC_LEFT, KC_DOWN, KC_RGHT),
    [_NICOLA] = LAYOUT(
        x___x, x___x, x___x, x___x, x___x, x___x,       x___x, x___x, x___x,   x___x,  x___x,   x___x,   x___x,   x___x, x___x,
        x___x, NG_1,   NG_2, NG_3,  NG_4,  NG_5,        NG_6,  NG_7,  NG_8,    NG_9,   NG_0,    KC_LBRC, KC_RBRC, x___x,
        x___x, NG_Q,   NG_W, NG_E,  NG_R,  NG_T,        NG_Y,  NG_U,  NG_I,    NG_O,   NG_P,    NG_SLSH, x___x,   x___x, x___x,
        x___x, NG_A,   NG_S, NG_D,  NG_F,  NG_G, x___x, NG_H,  NG_J,  NG_K,    NG_L,   NG_SCLN, NG_QUOT, x___x,   x___x,
        x___x, NG_Z,   NG_X, NG_C,  NG_V,  NG_B, x___x, NG_N, NG_M,   NG_COMM, NG_DOT, NG_SLSH, x___x,   x___x,
        x___x, x___x, x___x, NG_SHFTL, x___x,           x___x, NG_SHFTR, x___x, x___x, x___x, x___x, x___x, x___x),
    // [_VI] = LAYOUT(
    //     x___x, x___x, x___x, x___x, x___x, x___x,        x___x,   x___x,   x___x,   x___x, x___x,   x___x, x___x, x___x, x___x,
    //     x___x, x___x, x___x, x___x, x___x, x___x,        x___x,   x___x,   x___x,   x___x, x___x,   x___x, x___x, x___x,
    //     x___x, x___x, x___x, x___x, x___x, x___x,        KC_HOME, x___x,   x___x,   x___x, x___x,   x___x, x___x, x___x, x___x,
    //     x___x, x___x, x___x, x___x, x___x, x___x, x___x, KC_END, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT,   x___x, x___x, x___x,
    //     x___x, x___x, x___x, x___x, x___x, x___x, x___x, x___x,  x___x,   x___x,   x___x, x___x,   x___x, x___x,
    //     x___x, x___x, x___x, x___x, x___x, x___x, x___x, x___x,  x___x,   x___x,   x___x, x___x,   x___x),
    [_QWERTY] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2, KC_F3,  KC_F4,  KC_F5,            KC_F6,   KC_F7,  KC_F8,  KC_F9,   KC_F10, KC_F11,  KC_F12,  KC_DEL,  KC_PSCR,
        KC_GRV,  KC_1,    KC_2,  KC_3,   KC_4,   KC_5,             KC_6,   KC_7,   KC_8,    KC_9,   KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,  KC_E,   KC_R,   KC_T,             KC_Y,    KC_U,   KC_I,   KC_O,    KC_P,   KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
        CH_EN, KC_A,    KC_S,  KC_D,   KC_F,   KC_G,      KC_BSPC, KC_H,   KC_J,   KC_K,    KC_L,   KC_SCLN, KC_QUOT, KC_ENT,  KC_PGDN,
        KC_LSFT, KC_Z,    KC_X,  KC_C,   KC_V,   KC_B,    KC_ENT,  KC_N,   KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP,
        KC_LCTL, KC_RALT, KC_LALT, KC_SPC, KC_RCTL,                KC_RSFT, KC_SPC, KC_RALT, KC_RCTL,  MO(_FUNC), KC_LEFT, KC_DOWN, KC_RGHT),
    [_FUNC] = LAYOUT(
        QK_BOOT, x___x,   CH_QWERTY,   x___x,   x___x,   x___x,        x___x, x___x, x___x, KC_CALC, KC_MYCM, KC_MSEL, KC_MAIL, NK_TOGG, EE_CLR,
        x___x,   x___x,   x___x,   x___x,   x___x,   x___x,        x___x, x___x, x___x, x___x,   x___x,   x___x,   x___x,   KC_NUM,
        RM_TOGG, RM_NEXT, RM_VALU, RM_HUEU, x___x,   x___x,        x___x, x___x, x___x, x___x,   x___x,   x___x,   x___x,   x___x,  KC_HOME,
        x___x,   C(KC_A), C(KC_S), x___x,   C(KC_F), x___x, x___x, x___x, x___x, x___x, x___x,   x___x,   x___x,   x___x,  KC_END,
        KC_LSFT, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), x___x, x___x, x___x, x___x, x___x, x___x,   x___x,   KC_MUTE, KC_UP,
        x___x,  S(KC_F10),KC_MEH,  x___x, x___x, x___x, x___x, x___x, x___x, KC_MPLY, KC_LEFT, KC_DOWN, KC_RGHT)
};

void keyboard_post_init_user(void)
{
    rgblight_enable(); // RGBを有効に
    rgblight_mode(1);  // 単色固定モード
    rgblight_sethsv(85 /*Hue*/, 255 /*Sat*/, 100 /*Val*/);
}
void matrix_init_user(void)
{
    // NICOLA親指シフト
    set_nicola(_NICOLA);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    if (record->event.pressed)
    {
        if (keycode > CH_LAYER && keycode < CH_LAYER_END)
        {
            // layer_off(_VI);
            layer_off(_QWERTY);
            nicola_off();
            switch (keycode)
            {
            case CH_JP:
                nicola_on();
                 rgblight_sethsv(170 /*Hue*/, 255 /*Sat*/, 100 /*Val*/);
                return false;
            case CH_EN:
                 rgblight_sethsv(85 /*Hue*/, 255 /*Sat*/, 100 /*Val*/);
                return false;
            // case CH_VI:
            //     layer_on(_VI);
            //      rgblight_sethsv(20 /*Hue*/, 255 /*Sat*/, 100 /*Val*/);
            //     return false;
            case CH_QWERTY:
                layer_on(_QWERTY);
                 rgblight_sethsv(20 /*Hue*/, 255 /*Sat*/, 100 /*Val*/);
                return false;
            }
        }
    }

    // NICOLA親指シフト
    bool a = true;
    if (nicola_state())
    {
        // nicola_mode(keycode, record);
        a = process_nicola(keycode, record);
    }
    if (a == false)
        return false;
    // NICOLA親指シフト
    return true;
}
