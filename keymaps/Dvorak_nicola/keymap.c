#include QMK_KEYBOARD_H
#include "../../nicola.c"
#include "keymap_jp.h"
NGKEYS nicola_keys;
enum keymap_layers {
  _DVORAK,
  _NICOLA,
  _SMODI,
  _QWERTY,
  _MODI,
  _FUNC
};
//  [_DVORAK] = LAYOUT(
// 		KC_ESC,            KC_F1,   KC_F2,   KC_F3,  KC_F4,  KC_F5,   KC_F6,   KC_F7,  KC_F8,   KC_F9,     KC_F10,   KC_F11,  KC_F12,   KC_DEL,  KC_PSCR,
// 		KC_GRV,            KC_1,    KC_2,    KC_3,   KC_4,   KC_5,             KC_6,   KC_7,    KC_8,      KC_9,     KC_0,    KC_LBRC,  KC_RBRC, KC_BSPC,
// 		KC_TAB,            KC_QUOT, KC_COMM, KC_DOT, KC_P,   KC_Y,    KC_F,    KC_G,   KC_C,    KC_R,      KC_L,     KC_SLSH, KC_EQUAL, KC_BSLS, KC_PGUP,
// 		KC_CAPS,           KC_A,    KC_O,    KC_E,   KC_U,   KC_I,    KC_BSPC, KC_D,   KC_H,    KC_T,      KC_N,     KC_S,    KC_MINS  ,KC_ENT,  KC_PGDN,
// 		LM(_MODI,MOD_LSFT),KC_SCLN, KC_Q,    KC_J,   KC_K,   KC_X,    KC_ENT,  KC_B,   KC_M,    KC_W,      KC_V,     KC_Z,    KC_RSFT,  KC_UP, 
// 		LM(_MODI,MOD_LCTL),KC_LGUI, KC_LALT,         KC_SPC, KC_RCTL,          KC_RSFT, KC_SPC, KC_RALT,   MO(_FUNC),KC_RCTL, KC_LEFT,  KC_DOWN, KC_RGHT),
void jis2us ( uint16_t keycode );
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 [_DVORAK] = LAYOUT(
		KC_ESC,            KC_F1,   KC_F2,   KC_F3,  KC_F4,  KC_F5,   KC_F6,   KC_F7,  KC_F8,   KC_F9,     KC_F10,   KC_F11,  KC_F12,   KC_DEL,  KC_PSCR,
		JP_GRV ,           KC_1,    KC_2,    KC_3,   KC_4,   KC_5,             KC_6,   KC_7,    KC_8,      KC_9,     KC_0,    JP_LBRC,  JP_RBRC, KC_BSPC,
		KC_TAB,            JP_QUOT, JP_COMM ,JP_DOT ,KC_P,   KC_Y,    KC_F,    KC_G,   KC_C,    KC_R,      KC_L,     JP_SLSH, JP_EQL  , JP_YEN , KC_PGUP,
		JP_HENK ,          KC_A,    KC_O,    KC_E,   KC_U,   KC_I,    KC_BSPC, KC_D,   KC_H,    KC_T,      KC_N,     KC_S,    JP_MINS  ,KC_ENT,  KC_PGDN,
		MO(_SMODI),        JP_SCLN ,KC_Q,    KC_J,   KC_K,   KC_X,    KC_ENT,  KC_B,   KC_M,    KC_W,      KC_V,     KC_Z,    KC_RSFT,  KC_UP, 
		LM(_MODI,MOD_LCTL),KC_LGUI, KC_LALT,         KC_SPC, KC_RCTL,          MO(_SMODI), KC_SPC, KC_RALT,MO(_FUNC),KC_RCTL, KC_LEFT,  KC_DOWN, KC_RGHT),
 [_NICOLA] = LAYOUT(
		KC_ESC,  KC_F1,   KC_F2, KC_F3,  KC_F4,  KC_F5,   KC_F6,   KC_F7, KC_F8,  KC_F9,   KC_F10, KC_F11,  KC_F12,  KC_DEL,  KC_PSCR,
		KC_NO,   NG_1,    NG_2,  NG_3,   NG_4,   NG_5,             NG_6,   NG_7,  NG_8,    NG_9,   NG_0,    KC_MINS, KC_EQL,  KC_BSPC,
		KC_TAB,  NG_Q,    NG_W,  NG_E,   NG_R,   NG_T,    NG_Y,    NG_U,  NG_I,   NG_O,    NG_P,   KC_COMM, KC_RBRC, KC_BSLS, KC_PGUP,
		JP_MHEN ,NG_A,    NG_S,  NG_D,   NG_F,   NG_G,    KC_BSPC, NG_H,  NG_J,   NG_K,    NG_L,   NG_SCLN, KC_BSPC, KC_ENT,  KC_PGDN,
		KC_TRNS, NG_Z,    NG_X,  NG_C,   NG_V,   NG_B,    KC_ENT,  NG_N,  NG_M,   NG_COMM, NG_DOT, NG_SLSH, KC_RSFT, KC_UP,
		KC_TRNS, KC_NO  , KC_LALT,       KC_SPC, NG_SHFTL,        NG_SHFTR,KC_SPC,KC_RALT, MO(_FUNC),KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),
 [_SMODI] = LAYOUT(
		KC_ESC,  KC_F1,   KC_F2,  KC_F3,  KC_F4,  KC_F5,   KC_F6,   KC_F7,  KC_F8,  KC_F9,   KC_F10,   KC_F11,  KC_F12,  KC_DEL,  KC_PSCR,
		JP_TILD ,JP_EXLM, JP_AT,  JP_HASH,JP_DLR ,JP_PERC,          JP_CIRC,JP_AMPR,JP_ASTR, JP_LPRN,  JP_RPRN ,JP_LCBR ,JP_RCBR, KC_BSPC,  
		KC_TAB,  JP_DQUO, JP_LABK,JP_RABK,S(KC_P),S(KC_Y), S(KC_F), S(KC_G),S(KC_C),S(KC_R), S(KC_L),  JP_QUES, JP_PLUS, JP_PIPE ,KC_PGUP,
		KC_CAPS, S(KC_A), S(KC_O),S(KC_E),S(KC_U),S(KC_I), KC_BSPC, S(KC_D),S(KC_H),S(KC_T), S(KC_N),  S(KC_S), JP_UNDS ,KC_ENT,  KC_PGDN,
		KC_TRNS, JP_COLN ,S(KC_Q),S(KC_J),S(KC_K),S(KC_X), KC_ENT,  S(KC_B),S(KC_M),S(KC_W), S(KC_V),  S(KC_Z), KC_RSFT, KC_UP,
		KC_TRNS, KC_NO  , KC_LALT,        KC_SPC, KC_TRNS ,         KC_TRNS,KC_SPC, KC_RALT, MO(_FUNC),KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),
  [_QWERTY] = LAYOUT(
		KC_ESC,  KC_F1,   KC_F2, KC_F3,  KC_F4,  KC_F5,   KC_F6,   KC_F7, KC_F8,  KC_F9,   KC_F10, KC_F11,  KC_F12,  KC_DEL,  KC_PSCR,
		KC_NO,   KC_1,    KC_2,  KC_3,   KC_4,   KC_5,             KC_6,   KC_7,  KC_8,    KC_9,   KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
		KC_TAB,  KC_Q,    KC_W,  KC_E,   KC_R,   KC_T,    KC_Y,    KC_U,  KC_I,   KC_O,    KC_P,   KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
		KC_NO,   KC_A,    KC_S,  KC_D,   KC_F,   KC_G,    KC_BSPC, KC_H,  KC_J,   KC_K,    KC_L,   KC_SCLN, KC_QUOT, KC_ENT,  KC_PGDN,
		KC_LSFT, KC_Z,    KC_X,  KC_C,   KC_V,   KC_B,    KC_ENT,  KC_N,  KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP,
		KC_LCTL, KC_NO ,  KC_LALT,       KC_SPC, KC_RCTL, KC_RSFT, KC_SPC,        KC_RALT, MO(_FUNC),  KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),
  [_MODI] = LAYOUT(
		KC_ESC,  KC_F1,   KC_F2, KC_F3,  KC_F4,  KC_F5,   KC_F6,   KC_F7, KC_F8,  KC_F9,   KC_F10, KC_F11,  KC_F12,  KC_DEL,  KC_PSCR,
		KC_GRV,  KC_1,    KC_2,  KC_3,   KC_4,   KC_5,             KC_6,   KC_7,  KC_8,    KC_9,   KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
		KC_TAB,  KC_Q,    KC_W,  KC_E,   KC_R,   KC_T,    KC_Y,    KC_U,  KC_I,   KC_O,    KC_P,   KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
		KC_CAPS, KC_A,    KC_S,  KC_D,   KC_F,   KC_G,    KC_BSPC, KC_H,  KC_J,   KC_K,    KC_L,   KC_SCLN, KC_QUOT, KC_ENT,  KC_PGDN,
		KC_LSFT, KC_Z,    KC_X,  KC_C,   KC_V,   KC_B,    KC_ENT,  KC_N,  KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP,
		KC_LCTL, KC_LGUI  , KC_LALT,     KC_SPC, KC_RCTL, KC_RSFT, KC_SPC,        KC_RALT, MO(_FUNC),  KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),
  [_FUNC] = LAYOUT(
    RESET,     KC_TRNS,  FUNC_01,    KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_CALC,   KC_MYCM,  KC_MSEL,   KC_MAIL,   NK_TOGG,   EEP_RST,
    KC_TRNS,   KC_TRNS,  KC_TRNS,    KC_TRNS,  KC_TRNS,  KC_TRNS,             KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_NLCK,
    RGB_TOG,   RGB_MOD,  RGB_VAI,    RGB_HUI,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_HOME,
    KC_TRNS,   RGB_SPD,  RGB_VAD,    RGB_SPI,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_END,
    KC_TRNS,   KC_TRNS,  KC_TRNS,    KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_MUTE,   KC_VOLU,
    KC_TRNS,   KC_TRNS,  KC_TRNS,              KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,            KC_TRNS,   KC_TRNS,  KC_MPLY,   KC_MPRV,   KC_VOLD,   KC_MNXT)
};
static bool is_gaming ;
void matrix_init_user(void) {
  // NICOLA親指シフト
  set_nicola(_NICOLA);
  is_gaming = false;
  // NICOLA親指シフト
}
void trg_nicola(void);
void trg_nicola(void){
  if( nicola_state() ){
    nicola_off();
  }else {
    nicola_on();
  }
}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if( is_gaming && (keycode != FUNC_01 )) return true;

  if( record->event.pressed ){
    switch(keycode){
      case KC_HENK : 
        nicola_on();
        return false;
      case KC_MHEN :
        nicola_off();
        return false;
      case FUNC_01 :
        if( is_gaming ){
          is_gaming = false;
          layer_off(_QWERTY);
        }else{
          is_gaming = true;
          layer_on(_QWERTY);
        }
        return false;
    }
  }
  // if( ( JIS2US_TOP <  keycode ) && ( keycode < JIS2US_BOTTOM ) && ( record->event.pressed )) {
  //   jis2us(keycode);
  //   return false;
  // }
  // NICOLA親指シフトaoeaoeu
  bool a = true;
  if (nicola_state()) {
    // nicola_mode(keycode, record);
    a = process_nicola(keycode, record);
  }
  if (a == false) return false;
  // NICOLA親指シフト
  return true;
}
// void jis2us ( uint16_t keycode ) {
//   if( get_mods() & ( MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT))){
//     switch(keycode) {
//       case JIS2US_GRV:   send_string("+"); break;
//       case JIS2US_LBRC:  send_string("}" ); break;
//       case JIS2US_RBRC:  send_string("|" ); break;
//       case JIS2US_QUOT:  send_string("@" ); break;
//       case JIS2US_SLSH:  send_string("?" ); break;
//       case JIS2US_EQUAL: send_string(":" ); break;
//       case JIS2US_BSLS:  send_string("" ); break;
//       case JIS2US_MINS:  send_string("" ); break;
//       case JIS2US_1:     send_string("!" ); break;
//       case JIS2US_2:     send_string("[" ); break;
//       case JIS2US_3:     send_string("#" ); break;
//       case JIS2US_4:     send_string("$" ); break;
//       case JIS2US_5:     send_string("%" ); break;
//       case JIS2US_6:     send_string("=" ); break;
//       case JIS2US_7:     send_string("^" ); break;
//       case JIS2US_8:     send_string("\"" ); break;
//       case JIS2US_9:     send_string("*" ); break;
//       case JIS2US_0:     send_string("(" ); break;
//       case JIS2US_SCLN:  send_string("'" ); break;
//     }   
//   }else{
//     switch(keycode) {
//       case JIS2US_GRV:   send_string("{"); break;
//       case JIS2US_LBRC:  send_string("]" ); break;
//       case JIS2US_RBRC:  send_string("\\" ); break;
//       case JIS2US_QUOT:  send_string("&" ); break;
//       case JIS2US_SLSH:  send_string("/" ); break;
//       case JIS2US_EQUAL: send_string("_" ); break;
//       case JIS2US_BSLS:  send_string("" ); break;
//       case JIS2US_MINS:  send_string("-" ); break;
//       case JIS2US_1:     send_string("1" ); break;
//       case JIS2US_2:     send_string("2" ); break;
//       case JIS2US_3:     send_string("3" ); break;
//       case JIS2US_4:     send_string("4" ); break;
//       case JIS2US_5:     send_string("5" ); break;
//       case JIS2US_6:     send_string("6" ); break;
//       case JIS2US_7:     send_string("7" ); break;
//       case JIS2US_8:     send_string("8" ); break;
//       case JIS2US_9:     send_string("9" ); break;
//       case JIS2US_0:     send_string("0" ); break;
//       case JIS2US_SCLN:  send_string(";" ); break;
//     }
//   }
// }




  