/* Copyright 2018-2019 eswai <@eswai>
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
/*
 * introduced NICOLA state machine
 * (C) 2020 Sadao Ikebe @bonyarou
 */

#include QMK_KEYBOARD_H
#include "nicola.h"
#include "deferred_exec.h"
#include <timer.h>
#include "sendstring_japanese.h"
#include "matrix.h"
#include "quantum.h"

static bool is_nicola = false; // 親指シフトがオンかオフか
static uint8_t nicola_layer = 0; // レイヤー番号
// static uint8_t n_modifier = 0; // 押しているmodifierキーの数

#define TIMEOUT_THRESHOLD (150)
#define OVERLAP_THRESHOLD (20)

#ifndef NICOLA_SHFTL_TAP
#define NICOLA_SHFTL_TAP KC_SPC
#endif
#ifndef NICOLA_SHFTR_TAP
#define NICOLA_SHFTR_TAP KC_ENT
#endif
#ifndef NICOLA_SHFTL2_TAP
#define NICOLA_SHFTL2_TAP KC_TAB
#endif
#ifndef NICOLA_SHFTR2_TAP
#define NICOLA_SHFTR2_TAP KC_BSPC
#endif
#ifndef NICOLA_SHFTL_HOLD
#define NICOLA_SHFTL_HOLD KC_LALT
#endif
#ifndef NICOLA_SHFTR_HOLD
#define NICOLA_SHFTR_HOLD KC_LALT
#endif
#ifndef NICOLA_SHFTL2_HOLD
#define NICOLA_SHFTL2_HOLD KC_LCTL
#endif
#ifndef NICOLA_SHFTR2_HOLD
#define NICOLA_SHFTR2_HOLD KC_RSFT
#endif

typedef enum {
  NICOLA_STATE_S1_INIT,
  NICOLA_STATE_S2_M,
  NICOLA_STATE_S3_O,
  NICOLA_STATE_S4_MO,
  NICOLA_STATE_S5_OM
} nicola_state_t;

static nicola_state_t nicola_int_state = NICOLA_STATE_S1_INIT;
static int nicola_m_key;
static int nicola_o_key;
static uint16_t nicola_m_time;
static uint16_t nicola_o_time;
static bool nicola_hold_registered = false;
static bool nicola_hold_keys_registered[4] = {false, false, false, false};
static uint16_t nicola_hold_keycodes[4] = {KC_NO, KC_NO, KC_NO, KC_NO};

static int key_process_guard = 0;
void keypress_timer_expired(void);
static deferred_token timeout_token = INVALID_DEFERRED_TOKEN;

static uint32_t nicola_timeout(uint32_t trigger_time, void *cb_arg) {
  if(key_process_guard) {
    return 1;
  }

  timeout_token = INVALID_DEFERRED_TOKEN;
  keypress_timer_expired();
  return 0;
}

static void keypress_timer_start(void) {
  if(timeout_token == INVALID_DEFERRED_TOKEN) {
    timeout_token = defer_exec(TIMEOUT_THRESHOLD, nicola_timeout, NULL);
  } else {
    extend_deferred_exec(timeout_token, TIMEOUT_THRESHOLD);
  }
}

static bool nicola_is_o_key(uint16_t keycode) {
  return keycode == NG_SHFTL || keycode == NG_SHFTR ||
         keycode == NG_SHFTL2 || keycode == NG_SHFTR2;
}

static bool nicola_is_left_o_key(uint16_t keycode) {
  return keycode == NG_SHFTL || keycode == NG_SHFTL2;
}

static bool nicola_is_mode_toggle_pair(uint16_t first_key, uint16_t second_key) {
  return (first_key == NG_SHFTL && second_key == NG_SHFTR) ||
         (first_key == NG_SHFTR && second_key == NG_SHFTL) ||
         (first_key == NG_SHFTL2 && second_key == NG_SHFTR2) ||
         (first_key == NG_SHFTR2 && second_key == NG_SHFTL2);
}

static int8_t nicola_o_key_index(uint16_t keycode) {
  switch(keycode) {
    case NG_SHFTL: return 0;
    case NG_SHFTR: return 1;
    case NG_SHFTL2: return 2;
    case NG_SHFTR2: return 3;
  }
  return -1;
}

static bool nicola_o_key_held(uint16_t keycode) {
  int8_t hold_key_index = nicola_o_key_index(keycode);
  return hold_key_index >= 0 && nicola_hold_keys_registered[hold_key_index];
}

static uint16_t nicola_o_tap_keycode(uint16_t keycode) {
  switch(keycode) {
    case NG_SHFTL: return NICOLA_SHFTL_TAP;
    case NG_SHFTR: return NICOLA_SHFTR_TAP;
    case NG_SHFTL2: return NICOLA_SHFTL2_TAP;
    case NG_SHFTR2: return NICOLA_SHFTR2_TAP;
  }
  return KC_NO;
}

static uint16_t nicola_o_hold_keycode(uint16_t keycode) {
  switch(keycode) {
    case NG_SHFTL: return NICOLA_SHFTL_HOLD;
    case NG_SHFTR: return NICOLA_SHFTR_HOLD;
    case NG_SHFTL2: return NICOLA_SHFTL2_HOLD;
    case NG_SHFTR2: return NICOLA_SHFTR2_HOLD;
  }
  return KC_NO;
}

static void tap_code16_if_set(uint16_t keycode) {
  if(keycode != KC_NO) {
    tap_code16(keycode);
  }
}

// if we have independent timeout routine, no need to check timeout on key press
#ifdef TIMEOUT_INTERRUPT
#define IF_TIMEOUT(x) if(0)
#else
#define IF_TIMEOUT(x) if(x)
#endif

// 親指シフトのレイヤー、シフトキーを設定
void set_nicola(uint8_t layer) {
  nicola_layer = layer;
#ifdef TIMEOUT_INTERRUPT
  keypress_timer_init(keypress_timer_expired);
#endif
}

// 親指シフトをオンオフ
void nicola_on(void) {
  is_nicola = true;
  tap_code16(KC_INT4);
  nicola_clear();
}

void nicola_off(void) {
  is_nicola = false;
  tap_code16(KC_INT5);
  nicola_clear();
}

void layer_change(int layer_num) {
  is_nicola = false;
  tap_code16(KC_INT5);
  layer_on(layer_num);
} 

// 親指シフトの状態
bool nicola_state(void) {
  return is_nicola;
}


// バッファをクリアする
void nicola_clear(void) {
  layer_move(_NICOLA);
  for(uint8_t i = 0; i < 4; i++) {
    if(nicola_hold_keys_registered[i]) {
      unregister_code16(nicola_hold_keycodes[i]);
      nicola_hold_keys_registered[i] = false;
      nicola_hold_keycodes[i] = KC_NO;
    }
  }
  nicola_hold_registered = false;
  if(timeout_token != INVALID_DEFERRED_TOKEN) {
    cancel_deferred_exec(timeout_token);
    timeout_token = INVALID_DEFERRED_TOKEN;
  }
  nicola_int_state = NICOLA_STATE_S1_INIT;
  key_process_guard = 0;
}

void nicola_m_type(void) {
  if(is_nicola) {
    switch(nicola_m_key) {
        case NG_Q   : send_string("." ); break;
        case NG_W   : send_string("ka"); break;
        case NG_E   : send_string("ta"); break;
        case NG_R   : send_string("ko"); break;
        case NG_T   : send_string("sa"); break;
        case NG_Y   : send_string("ra"); break;
        case NG_U   : send_string("ti"); break;
        case NG_I   : send_string("ku"); break;
        case NG_O   : send_string("tu"); break;
        case NG_P   : send_string("," ); break;
        case NG_LBRC: send_string("," ); break;
        case NG_RBRC: send_string(";" ); break;

        case NG_A   : send_string("u" ); break;
        case NG_S   : send_string("si"); break;
        case NG_D   : send_string("te"); break;
        case NG_F   : send_string("ke"); break;
        case NG_G   : send_string("se"); break;
        case NG_H   : send_string("ha"); break;
        case NG_J   : send_string("to"); break;
        case NG_K   : send_string("ki"); break;
        case NG_L   : send_string("i" ); break;
        case NG_SCLN: send_string("nn"); break;

        case NG_Z   : tap_code(KC_LGUI); break;
        case NG_X   : send_string("hi"); break;
        case NG_C   : send_string("su"); break;
        case NG_V   : send_string("hu"); break;
        case NG_B   : send_string("he"); break;
        case NG_N   : send_string("me"); break;
        case NG_M   : send_string("so"); break;
        case NG_COMM: send_string("ne"); break;
        case NG_DOT : send_string("ho"); break;
        case NG_SLSH: send_string("/" ); break;
    }
  } else {
    switch(nicola_m_key) {
        case NG_Q   : send_string("'" ); break;
        case NG_W   : send_string(","); break;
        case NG_E   : send_string("."); break;
        case NG_R   : send_string("p"); break;
        case NG_T   : send_string("y"); break;
        case NG_Y   : send_string("f"); break;
        case NG_U   : send_string("g"); break;
        case NG_I   : send_string("c"); break;
        case NG_O   : send_string("r"); break;
        case NG_P   : send_string("l" ); break;
        case NG_LBRC: send_string("[" ); break;
        case NG_RBRC: send_string("]" ); break;

        case NG_A   : send_string("a" ); break;
        case NG_S   : send_string("o"); break;
        case NG_D   : send_string("e"); break;
        case NG_F   : send_string("u"); break;
        case NG_G   : send_string("i"); break;
        case NG_H   : send_string("d"); break;
        case NG_J   : send_string("h"); break;
        case NG_K   : send_string("t"); break;
        case NG_L   : send_string("n" ); break;
        case NG_SCLN: send_string("s"); break;
        //case NG_QUOT: send_string(SS_TAP(X_BSPACE)); brzefuu"oae'eak;

        case NG_Z   : tap_code(KC_LGUI); break;
        case NG_X   : send_string("q"); break;
        case NG_C   : send_string("j"); break;
        case NG_V   : send_string("k"); break;
        case NG_B   : send_string("x"); break;
        case NG_N   : send_string("b"); break;
        case NG_M   : send_string("m"); break;
        case NG_COMM: send_string("w"); break;
        case NG_DOT : send_string("v"); break;
        case NG_SLSH: send_string("z" ); break;
    }
  }
}

void nicola_o_type(void) {
  uint16_t tap_keycode = nicola_o_tap_keycode(nicola_o_key);
  tap_code16_if_set(tap_keycode);
}

void nicola_o_hold(void) {
  int8_t hold_key_index = nicola_o_key_index(nicola_o_key);
  if(hold_key_index >= 0 && !nicola_hold_keys_registered[hold_key_index]) {
    uint16_t hold_keycode = nicola_o_hold_keycode(nicola_o_key);
    if(hold_keycode != KC_NO) {
      register_code16(hold_keycode);
      nicola_hold_keycodes[hold_key_index] = hold_keycode;
      nicola_hold_keys_registered[hold_key_index] = true;
      nicola_hold_registered = true;
    }
  }
}

void nicola_om_type_l2(void) {
  switch(nicola_m_key) {
      case NG_Q   : break;
      case NG_W   : break;
      case NG_E   : break;
      case NG_R   : break;
      case NG_T   : break;
      case NG_Y   : break;
      case NG_U   : tap_code16(KC_HOME); break;
      case NG_I   : tap_code16(KC_PGDN); break;
      case NG_O   : tap_code16(KC_PGUP); break;
      case NG_P   : tap_code16(KC_END); break;
      case NG_LBRC: break;
      case NG_RBRC: break;

      case NG_A   : nicola_off(); break;
      case NG_S   : break;
      case NG_D   : tap_code16(RGUI(KC_E)); break;
      case NG_F   : break;
      case NG_G   : break;
      case NG_H   : break;
      case NG_J   : tap_code16(KC_LEFT); break;
      case NG_K   : tap_code16(KC_DOWN); break;
      case NG_L   : tap_code16(KC_UP); break;
      case NG_SCLN: tap_code16(KC_RIGHT); break;
      case NG_QUOT: break;

      case NG_Z   : tap_code16(KC_ESC); break;
      case NG_X   : tap_code16(RGUI(KC_LEFT)); break;
      case NG_C   : tap_code16(RGUI(KC_DOWN));break;
      case NG_V   : tap_code16(RGUI(KC_UP)); break;
      case NG_B   : tap_code16(RGUI(KC_RIGHT)); break;
      case NG_N   : break;
      case NG_M   : break;
      case NG_COMM: break;
      case NG_DOT : break;
      case NG_SLSH: break;

      default: break;
  }
}

void nicola_om_type_r2(void) {
  switch(nicola_m_key) {
      case NG_Q   : break;
      case NG_W   : break;
      case NG_E   : break;
      case NG_R   : break;
      case NG_T   : break;
      case NG_Y   : break;
      case NG_U   : layer_change(_NUMBER); break;
      case NG_I   : break;
      case NG_O   : break;
      case NG_P   : break;
      case NG_LBRC: break;
      case NG_RBRC: break;

      case NG_A   : break;
      case NG_S   : tap_code16(RSFT(KC_F2)); break;
      case NG_D   : break;
      case NG_F   : break;
      case NG_G   : break;
      case NG_H   : break;
      case NG_J   : layer_change(_MOVE); break;
      case NG_K   : break;
      case NG_L   : break;
      case NG_SCLN: nicola_on(); break;
      case NG_QUOT: break;

      case NG_Z   : tap_code16(KC_ESC); break;
      case NG_X   : break;
      case NG_C   : break;
      case NG_V   : break;
      case NG_B   : break;
      case NG_N   : tap_code16(RCTL(KC_F12)); break;
      case NG_M   : break;
      case NG_COMM: break;
      case NG_DOT : break;
      case NG_SLSH: break;

      default: break;
  }
}

void nicola_om_type(void) {
  if(is_nicola) {
    if(nicola_o_key == NG_SHFTL) {
        switch(nicola_m_key) {
            case NG_Q   : send_string("la"); break;
            case NG_W   : send_string("e" ); break;
            case NG_E   : send_string("ri"); break;
            case NG_R   : send_string("lya");break;
            case NG_T   : send_string("re"); break;
            case NG_Y   : send_string("pa"); break;
            case NG_U   : send_string("di"); break;
            case NG_I   : send_string("gu"); break;
            case NG_O   : send_string("du"); break;
            case NG_P   : send_string("pi"); break;
            case NG_LBRC: send_string("[" ); break;
            case NG_RBRC: send_string("]" ); break;

            case NG_A   : send_string("wo"); break;
            case NG_S   : send_string("a" ); break;
            case NG_D   : send_string("na"); break;
            case NG_F   : send_string("lyu");break;
            case NG_G   : send_string("mo"); break;
            case NG_H   : send_string("ba"); break;
            case NG_J   : send_string("do"); break;
            case NG_K   : send_string("gi"); break;
            case NG_L   : send_string("po"); break;
            case NG_SCLN: send_string("!" ); break;
            //case NG_QUOT: send_string(SS_TAP(X_BSPACE)); break;

            case NG_Z   : send_string("lu"); break;
            case NG_X   : send_string("-" ); break;
            case NG_C   : send_string("ro"); break;
            case NG_V   : send_string("ya"); break;
            case NG_B   : send_string("li"); break;
            case NG_N   : send_string("pu"); break;
            case NG_M   : send_string("zo"); break;
            case NG_COMM: send_string("pe"); break;
            case NG_DOT : send_string("bo"); break;
            case NG_SLSH: send_string("?" ); break;
        }
    } else if(nicola_o_key == NG_SHFTR) {
        switch(nicola_m_key) {
            case NG_Q   :                    break;
            case NG_W   : send_string("ga"); break;
            case NG_E   : send_string("da"); break;
            case NG_R   : send_string("go"); break;
            case NG_T   : send_string("za"); break;
            case NG_Y   : send_string("yo"); break;
            case NG_U   : send_string("ni"); break;
            case NG_I   : send_string("ru"); break;
            case NG_O   : send_string("ma"); break;
            case NG_P   : send_string("le"); break;
            case NG_LBRC: send_string("_" ); break;
            case NG_RBRC: send_string("*" ); break;

            case NG_A   : send_string("vu"); break;
            case NG_S   : send_string("zi"); break;
            case NG_D   : send_string("de"); break;
            case NG_F   : send_string("ge"); break;
            case NG_G   : send_string("ze"); break;
            case NG_H   : send_string("mi"); break;
            case NG_J   : send_string("o" ); break;
            case NG_K   : send_string("no"); break;
            case NG_L   : send_string("lyo");break;
            case NG_SCLN: send_string("xtu");break;
            case NG_QUOT: send_string("'" ); break;

            case NG_Z   :                    break;
            case NG_X   : send_string("bi"); break;
            case NG_C   : send_string("zu"); break;
            case NG_V   : send_string("bu"); break;
            case NG_B   : send_string("be"); break;
            case NG_N   : send_string("nu"); break;
            case NG_M   : send_string("yu"); break;
            case NG_COMM: send_string("mu"); break;
            case NG_DOT : send_string("wa"); break;
            case NG_SLSH: send_string("lo"); break;
        }
    } else if(nicola_o_key == NG_SHFTL2) {
        nicola_om_type_l2();
    } else if(nicola_o_key == NG_SHFTR2) {
        nicola_om_type_r2();
    }
  } else {
        if(nicola_o_key == NG_SHFTL) {
        switch(nicola_m_key) {
            case NG_Q   : send_string("1"); break;
            case NG_W   : send_string("2" ); break;
            case NG_E   : send_string("3"); break;
            case NG_R   : send_string("4");break;
            case NG_T   : send_string("5"); break;
            case NG_Y   : send_string("6"); break;
            case NG_U   : send_string("7"); break;
            case NG_I   : send_string("8"); break;
            case NG_O   : send_string("9"); break;
            case NG_P   : send_string("0"); break;
            // case NG_LBRC: send_string("{" ); break;
            // case NG_RBRC: send_string("}" ); break;

            case NG_A   : tap_code16(LCTL(KC_A)); break;
            case NG_S   : tap_code16(LCTL(KC_S)); break;
            case NG_D   : send_string("'" ); break;
            case NG_F   : send_string("\"" ); break;
            case NG_G   : tap_code16(LCTL(JP_SLSH)); break;

            case NG_H   :                 ; break;
            case NG_J   : send_string("="); break;
            case NG_K   : send_string("/"); break;
            case NG_L   : send_string("["); break;
            case NG_SCLN: send_string("]");  break;
            //case NG_QUOT: send_string(SS_TAP(X_BSPACE)); break;

            case NG_Z   : tap_code16(LCTL(KC_Z)); break;
            case NG_X   : tap_code16(LCTL(KC_X)); break;
            case NG_C   : tap_code16(LCTL(KC_C)); break;
            case NG_V   : tap_code16(LCTL(KC_V)); break;
            case NG_B   :                       ; break;
            case NG_N   :                       ; break;
            case NG_M   : send_string(";" ); break;
            case NG_COMM: send_string("-"); break;
            case NG_DOT : send_string("`"); break;
            case NG_SLSH: send_string("\\" ); break;
        }
    } else if(nicola_o_key == NG_SHFTR) {
        switch(nicola_m_key) {
            case NG_Q   : send_string("!"); break;
            case NG_W   : send_string("@"); break;
            case NG_E   : send_string("#"); break;
            case NG_R   : send_string("$"); break;
            case NG_T   : send_string("%"); break;
            case NG_Y   : send_string("^"); break;
            case NG_U   : send_string("&"); break;
            case NG_I   : send_string("*"); break;
            case NG_O   : send_string("("); break;
            case NG_P   : send_string(")"); break;

            case NG_A   : tap_code16(KC_F1); break;
            case NG_S   : tap_code16(KC_F2); break;
            case NG_D   : tap_code16(KC_F3); break;
            case NG_F   : tap_code16(KC_F4); break;
            case NG_G   : tap_code16(KC_F5); break;
            case NG_H   : tap_code16(KC_F6); break;

            case NG_J   : send_string("+" ); break;
            case NG_K   : send_string("?"); break;
            case NG_L   : send_string("{");break;
            case NG_SCLN: send_string("}");  break;

            case NG_Z   :                    break;
            case NG_X   : tap_code16(KC_F8); break;
            case NG_C   : tap_code16(KC_F9); break;
            case NG_V   : tap_code16(KC_F10); break;
            case NG_B   : tap_code16(KC_F11); break;
            case NG_N   : tap_code16(KC_F12); break;
            case NG_M   : send_string(":" ); break;
            case NG_COMM: send_string("_"); break;
            case NG_DOT : send_string("~"); break;
            case NG_SLSH: send_string("|"); break;
        }
    } else if(nicola_o_key == NG_SHFTL2) {
        nicola_om_type_l2();
    } else if(nicola_o_key == NG_SHFTR2) {
        nicola_om_type_r2();
    }
  }
}

// 親指シフトの入力処理
bool process_nicola(uint16_t keycode, keyrecord_t *record) {
  key_process_guard = 1; // timeout entrance guard
  bool cont_process = true;
  // if (!is_nicola || n_modifier > 0) return true;
  uint16_t curr_time = timer_read();

  if (record->event.pressed) {
    if(NG_M_TOP <= keycode && keycode <= NG_M_BOTTOM) {
        // M key
        switch(nicola_int_state) {
          case NICOLA_STATE_S1_INIT:
            // no timeout check
            nicola_int_state = NICOLA_STATE_S2_M;
            break;
          case NICOLA_STATE_S2_M:
            // same behavior for time out and sequential stroke
            nicola_m_type();
            break;
          case NICOLA_STATE_S3_O:
            // timeout check
            IF_TIMEOUT(curr_time - nicola_o_time > TIMEOUT_THRESHOLD) {
              // timeout => hold configured modifier => S2
              nicola_o_hold();
              nicola_int_state = NICOLA_STATE_S2_M;
            } else {
              // combo => S5
              nicola_int_state = NICOLA_STATE_S5_OM;
            }
            break;
          case NICOLA_STATE_S4_MO:
            // timeout check
            IF_TIMEOUT(curr_time - nicola_o_time > TIMEOUT_THRESHOLD) {
              // timeout => (output MO) => S2
              nicola_om_type();
              nicola_int_state = NICOLA_STATE_S2_M;
            } else {
              // combo => three key judge
              uint16_t t1 = nicola_o_time - nicola_m_time;
              uint16_t t2 = curr_time - nicola_o_time;
              if(t1 < t2) {
                // the O key in between is combo with the leading M key
                nicola_om_type();
                nicola_int_state = NICOLA_STATE_S2_M;
              } else {
                // the leading M key is single, the O key in between is combo with current key
                nicola_m_type();
                nicola_int_state = NICOLA_STATE_S5_OM;
              }
            }
            break;
          case NICOLA_STATE_S5_OM:
            // same behavior for time out and sequential stroke
            nicola_om_type();
            nicola_int_state = NICOLA_STATE_S2_M;
            break;
        }
        nicola_m_key = keycode;
        nicola_m_time = curr_time;
        keypress_timer_start();
        cont_process = false;
    } else if(nicola_is_o_key(keycode)) {
        // O key
        if (nicola_int_state == NICOLA_STATE_S3_O &&
            nicola_is_mode_toggle_pair(nicola_o_key, keycode)) {
          if (nicola_is_left_o_key(nicola_o_key)) {
            nicola_on();
          } else {
            nicola_off();
          }

          key_process_guard = 0;
          return false;
        }

        switch(nicola_int_state) {
          case NICOLA_STATE_S1_INIT:
            // no timeout check
            nicola_int_state = NICOLA_STATE_S3_O;
            break;
          case NICOLA_STATE_S2_M:
            // timeout check
            IF_TIMEOUT(curr_time - nicola_m_time > TIMEOUT_THRESHOLD) {
              // timeout => (output M) => S3
              nicola_m_type();
              nicola_int_state = NICOLA_STATE_S3_O;
            } else {
              // combo => S4
              nicola_int_state = NICOLA_STATE_S4_MO;
            }
            break;
          case NICOLA_STATE_S3_O:
            // same behavior for time out and sequential stroke
            nicola_o_type();
            break;
          case NICOLA_STATE_S4_MO:
            // same behavior for time out and sequential stroke
            nicola_om_type();
            nicola_int_state = NICOLA_STATE_S3_O;
            break;
          case NICOLA_STATE_S5_OM:
            // timeout check
            IF_TIMEOUT(curr_time - nicola_m_time > TIMEOUT_THRESHOLD) {
              // timeout => (output MO) => S3
              nicola_om_type();
              nicola_int_state = NICOLA_STATE_S3_O;
            } else {
              // combo => three key judge
              uint16_t t1 = nicola_m_time - nicola_o_time;
              uint16_t t2 = curr_time - nicola_m_time;
              if(t1 < t2) {
                // the M key in between is combo with the leading O key
                nicola_om_type();
                nicola_int_state = NICOLA_STATE_S3_O;
              } else {
                // the leading O key is single, the M key in between is combo with current key
                nicola_o_type();
                nicola_int_state = NICOLA_STATE_S4_MO;
              }
            }
            break;
        }
        nicola_o_key = keycode;
        nicola_o_time = curr_time;
        keypress_timer_start();
        cont_process = false;
    } else {
        // その他のキーが押された
        switch(nicola_int_state) {
          case NICOLA_STATE_S1_INIT:
            break;
          case NICOLA_STATE_S2_M:
            nicola_m_type();
            break;
          case NICOLA_STATE_S3_O:
            IF_TIMEOUT(curr_time - nicola_o_time > TIMEOUT_THRESHOLD) {
              nicola_o_hold();
            } else if(!nicola_o_key_held(nicola_o_key)) {
              nicola_o_type();
            }
            break;
          case NICOLA_STATE_S4_MO:
            nicola_om_type();
            break;
          case NICOLA_STATE_S5_OM:
            nicola_om_type();
            break;
        }
        nicola_int_state = NICOLA_STATE_S1_INIT;
        key_process_guard = 0;
        // continue processing current key, so this path returns true
    }
  } else { // key release
    int8_t hold_key_index = nicola_o_key_index(keycode);
    if(hold_key_index >= 0 && nicola_hold_keys_registered[hold_key_index]) {
        unregister_code16(nicola_hold_keycodes[hold_key_index]);
        nicola_hold_keys_registered[hold_key_index] = false;
        nicola_hold_keycodes[hold_key_index] = KC_NO;
        nicola_hold_registered = false;
        for(uint8_t i = 0; i < 4; i++) {
          if(nicola_hold_keys_registered[i]) {
            nicola_hold_registered = true;
            break;
          }
        }
        cont_process = false;
    } else if(NG_TOP <= keycode && keycode <= NG_BOTTOM) { // key off
        switch(nicola_int_state) {
          case NICOLA_STATE_S1_INIT:
            break;
          case NICOLA_STATE_S2_M:
            if(nicola_m_key == keycode) {
              nicola_m_type();
              nicola_int_state = NICOLA_STATE_S1_INIT;
            }
            break;
          case NICOLA_STATE_S3_O:
            if(nicola_o_key == keycode) {
              nicola_o_type();
              nicola_int_state = NICOLA_STATE_S1_INIT;
            }
            break;
          case NICOLA_STATE_S4_MO:
            if(nicola_m_key == keycode) {
              // M ON --> O ON --> M OFF
              uint16_t t1 = nicola_o_time - nicola_m_time;
              uint16_t t2 = curr_time - nicola_o_time;
              if(t1>=t2 && t2 < OVERLAP_THRESHOLD) {
                // M ON --> O ON --> M OFF (M is output, but O is still open to combo)
                nicola_m_type();
                nicola_int_state = NICOLA_STATE_S3_O;
              } else {
                // M ON --> O ON --> M OFF (both M and O are output)
                nicola_om_type();
                nicola_int_state = NICOLA_STATE_S1_INIT;
              }
            } else if(nicola_o_key == keycode) {
              // M ON --> O ON --> O OFF (both M and O are output)
              nicola_om_type();
              nicola_int_state = NICOLA_STATE_S1_INIT;
            }
            break;
          case NICOLA_STATE_S5_OM:
            if(nicola_o_key == keycode) {
              // O ON --> M ON --> O OFF
              uint16_t t1 = nicola_m_time - nicola_o_time;
              uint16_t t2 = curr_time - nicola_m_time;
              if(t1>=t2 && t2 < OVERLAP_THRESHOLD) {
                // O ON --> M ON --> O OFF (O is output, but M is still open to combo)
                nicola_o_type();
                nicola_int_state = NICOLA_STATE_S2_M;
              } else {
                // O ON --> M ON --> O OFF (both M and O are output)
                nicola_om_type();
                nicola_int_state = NICOLA_STATE_S1_INIT;
              }
            } else if(nicola_m_key == keycode) {
              // O ON --> M ON --> M OFF (both O and M are output)
              nicola_om_type();
              nicola_int_state = NICOLA_STATE_S1_INIT;
            }
            break;
        }
        cont_process = false;
    }
  }
  key_process_guard = 0;
  return cont_process;
}

void keypress_timer_expired(void) {
    if(!key_process_guard) {
        switch(nicola_int_state) {
            case NICOLA_STATE_S1_INIT:
                break;
            case NICOLA_STATE_S2_M:
                nicola_m_type();
                break;
            case NICOLA_STATE_S3_O:
                nicola_o_hold();
                break;
            case NICOLA_STATE_S4_MO:
                nicola_om_type();
                break;
            case NICOLA_STATE_S5_OM:
                nicola_om_type();
                break;
        }
        nicola_int_state = NICOLA_STATE_S1_INIT;
    }
}
