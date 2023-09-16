#include QMK_KEYBOARD_H
#include "../../nicola.c"
#include "keymap_jp.h"

#define X__X KC_TRNS
NGKEYS nicola_keys;
enum keymap_layers {
  _DVORAK,
  _NICOLA,
	_VI,
  _ROGUE,
  _HJKL,
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
		JP_MHEN ,          KC_A,    KC_O,    KC_E,   KC_U,   KC_I,    KC_BSPC, KC_D,   KC_H,    KC_T,      KC_N,     KC_S,    JP_MINS  ,KC_ENT,  KC_PGDN,
		MO(_SMODI),        JP_SCLN ,KC_Q,    KC_J,   KC_K,   KC_X,    KC_ENT,  KC_B,   KC_M,    KC_W,      KC_V,     KC_Z,    KC_RSFT,  KC_UP, 
		LM(_MODI,MOD_LCTL),KC_LGUI, KC_LALT,         KC_SPC, KC_RCTL,          MO(_SMODI), VI_MODE, JP_HENK,MO(_FUNC),KC_RCTL, KC_LEFT,  KC_DOWN, KC_RGHT),
 [_NICOLA] = LAYOUT(
		KC_ESC,  KC_F1,   KC_F2, KC_F3,  KC_F4,  KC_F5,   KC_F6,   KC_F7,  KC_F8,   KC_F9,   KC_F10, KC_F11,  KC_F12,  KC_DEL,  KC_PSCR,
		KC_NO,   NG_1,    NG_2,  NG_3,   NG_4,   NG_5,             NG_6,   NG_7,    NG_8,    NG_9,   NG_0,    KC_MINS, KC_EQL,  KC_BSPC,
		KC_TAB,  NG_Q,    NG_W,  NG_E,   NG_R,   NG_T,    NG_Y,    NG_U,   NG_I,    NG_O,    NG_P,   KC_COMM, KC_RBRC, KC_BSLS, KC_PGUP,
		JP_MHEN ,NG_A,    NG_S,  NG_D,   NG_F,   NG_G,    KC_BSPC, NG_H,   NG_J,    NG_K,    NG_L,   NG_SCLN, KC_BSPC, KC_ENT,  KC_PGDN,
		X__X,    NG_Z,    NG_X,  NG_C,   NG_V,   NG_B,    KC_ENT,  NG_N,   NG_M,    NG_COMM, NG_DOT, NG_SLSH, KC_RSFT, KC_UP,
		X__X,    X__X  , KC_LALT,KC_SPC, NG_SHFTL,               NG_SHFTR,VI_MODE,JP_HENK, MO(_FUNC),KC_RCTL, KC_LEFT,KC_DOWN, KC_RGHT),
 [_VI] = LAYOUT(
		FUNC_DEFAULT,      KC_F1,   KC_F2,   KC_F3,  KC_F4,  KC_F5,   KC_F6,   KC_F7,  KC_F8,   KC_F9,     KC_F10,   KC_F11,  KC_F12,   KC_DEL,  KC_PSCR,
		KC_NO ,            KC_NO,    KC_NO,    KC_NO,   KC_NO,   KC_NO,           KC_NO,   KC_NO,   KC_NO,     KC_NO,    KC_NO,   KC_NO,    KC_NO,    KC_NO,
		KC_TAB,            KC_NO   , KC_NO    ,KC_NO   ,KC_NO,   KC_NO,           KC_NO,   KC_HOME, KC_NO,     KC_NO,    KC_PGUP, KC_NO ,   KC_NO ,   KC_NO,    KC_PGUP,
		JP_MHEN ,          KC_NO,    KC_NO,    KC_NO,   KC_NO,   KC_NO,  KC_BSPC, KC_NO,   KC_LEFT, KC_DOWN,   KC_UP,    KC_RGHT, KC_NO    ,KC_NO,    KC_PGDN,
		KC_LSFT,           C(KC_Z),  C(KC_X),  C(KC_C), C(KC_V), KC_NO,  KC_ENT,  KC_NO,   KC_END,  KC_NO,     KC_NO,    KC_PGDN, KC_RSFT,  KC_UP, 
		KC_LCTL,           KC_LGUI, KC_LALT,         KC_SPC, KC_RCTL,             KC_LSFT, VI_MODE,  JP_HENK,   MO(_FUNC),KC_RCTL, KC_LEFT,  KC_DOWN, KC_RGHT),
  [_ROGUE] = LAYOUT(
		KC_ESC,            KC_F1,   KC_F2,   KC_F3,  KC_F4,  KC_F5,   KC_F6,   KC_F7,  KC_F8,   KC_F9,     KC_F10,   KC_F11,  KC_F12,   KC_DEL,  KC_PSCR,
		JP_GRV ,           KC_1,    KC_2,    KC_3,   KC_4,   KC_5,             KC_6,   KC_7,    KC_8,      KC_9,     KC_0,    JP_LBRC,  JP_RBRC, KC_BSPC,
		KC_TAB,            JP_QUOT, JP_COMM ,JP_DOT ,KC_P,   KC_Y,    KC_F,    KC_7,   KC_9,    KC_R,      KC_L,     JP_SLSH, JP_EQL  , JP_YEN , KC_PGUP,
		KC_NO ,            KC_A,    KC_O,    KC_E,   KC_U,   KC_I,    KC_BSPC, KC_D,   KC_4,    KC_2,      KC_8,     KC_6,    KC_S     ,KC_ENT,  KC_PGDN,
		MO(_SMODI),        JP_SCLN ,KC_Q,    KC_J,   KC_K,   KC_X,    KC_ENT,  KC_B,   KC_1,    KC_3,      KC_V,     KC_Z,    KC_RSFT,  KC_UP, 
		KC_LCTL           ,KC_LGUI, KC_LALT,         KC_SPC, KC_RCTL,          MO(_SMODI), MO(_HJKL), KC_NO,MO(_FUNC),KC_RCTL, KC_LEFT,  KC_DOWN, KC_RGHT),
  [_HJKL] = LAYOUT(
		KC_ESC,            KC_F1,   KC_F2,   KC_F3,  KC_F4,  KC_F5,   KC_F6,   KC_F7,  KC_F8,   KC_F9,     KC_F10,   KC_F11,  KC_F12,   KC_DEL,  KC_PSCR,
		JP_GRV ,           KC_1,    KC_2,    KC_3,   KC_4,   KC_5,             KC_6,   KC_7,    KC_8,      KC_9,     KC_0,    JP_LBRC,  JP_RBRC, KC_BSPC,
		KC_TAB,            JP_QUOT, JP_COMM ,JP_DOT ,KC_P,   KC_Y,    KC_F,    KC_G,   KC_C,    KC_R,      KC_L,     JP_SLSH, JP_EQL  , JP_YEN , KC_PGUP,
		KC_NO ,          KC_A,    KC_O,    KC_E,   KC_U,   KC_I,    KC_BSPC,   KC_D,   KC_H,    KC_T,      KC_N,     KC_S,    JP_MINS  ,KC_ENT,  KC_PGDN,
		MO(_SMODI),        JP_SCLN ,KC_Q,    KC_J,   KC_K,   KC_X,    KC_ENT,  KC_B,   KC_M,    KC_W,      KC_V,     KC_Z,    KC_RSFT,  KC_UP, 
		KC_LCTL,          KC_LGUI, KC_LALT,         KC_SPC, KC_RCTL,        MO(_SMODI),X__X,   KC_NO,MO(_FUNC),KC_RCTL, KC_LEFT,  KC_DOWN, KC_RGHT),
 [_SMODI] = LAYOUT(
		S(KC_ESC),    S(KC_F1),     S(KC_F2),     S(KC_F3),    S(KC_F4),  S(KC_F5),   S(KC_F6),    S(KC_F7),    S(KC_F8),    S(KC_F9),     S(KC_F10),   S(KC_F11),    S(KC_F12),    S(KC_DEL), KC_PSCR,
		JP_TILD ,  JP_EXLM,   JP_AT,     JP_HASH,  JP_DLR ,JP_PERC,           JP_CIRC,  JP_AMPR,  JP_ASTR,   JP_LPRN,  JP_RPRN,   JP_LCBR ,  JP_RCBR,   S(KC_BSPC),  
		S(KC_TAB), JP_DQUO,   JP_LABK,   JP_RABK,  S(KC_P),S(KC_Y), S(KC_F),  S(KC_G),  S(KC_C),  S(KC_R),   S(KC_L),  JP_QUES,   JP_PLUS,   JP_PIPE ,  S(KC_HOME),
		S(KC_CAPS),S(KC_A),   S(KC_O),   S(KC_E),  S(KC_U),S(KC_I), KC_BSPC,  S(KC_D),  S(KC_H),  S(KC_T),   S(KC_N),  S(KC_S),   JP_UNDS,   S(KC_ENT), S(KC_END),
		X__X,   JP_COLN ,  S(KC_Q),   S(KC_J),  S(KC_K),S(KC_X), S(KC_ENT),S(KC_B),  S(KC_M),  S(KC_W),   S(KC_V),  S(KC_Z),   KC_RSFT,   S(KC_UP),
		S(KC_LCTL),S(KC_LGUI),          S(KC_LALT),S(KC_SPC),X__X,         X__X,  S(KC_SPC),VI_MODE,MO(_FUNC),S(KC_RCTL),S(KC_LEFT),S(KC_DOWN),S(KC_RGHT)),
  [_QWERTY] = LAYOUT(
		X__X,    X__X,    X__X,  X__X,   X__X,   X__X,    X__X,    X__X,  X__X,   X__X,    X__X,     X__X,    X__X,    X__X,    X__X,
		KC_NO,   KC_1,    KC_2,  KC_3,   KC_4,   KC_5,             KC_6,  KC_7,   KC_8,    KC_9,     KC_0,    KC_MINS, KC_EQL,  X__X,
		KC_TAB,  KC_Q,    KC_W,  KC_E,   KC_R,   KC_T,    KC_Y,    KC_U,  KC_I,   KC_O,    KC_P,     KC_LBRC, KC_RBRC, KC_BSLS, X__X,
		KC_NO,   KC_A,    KC_S,  KC_D,   KC_F,   KC_G,    KC_BSPC, KC_H,  KC_J,   KC_K,    KC_L,     KC_SCLN, KC_QUOT, KC_ENT,  X__X,
		KC_LSFT, KC_Z,    KC_X,  KC_C,   KC_V,   KC_B,    KC_ENT,  KC_N,  KC_M,   KC_COMM, KC_DOT,   KC_SLSH, X__X,    X__X,
		KC_LCTL, KC_NO ,  KC_LALT,       KC_SPC, KC_LCTL, KC_LSFT, KC_SPC,        KC_RALT, MO(_FUNC),X__X,    X__X,    X__X,    X__X),
  [_MODI] = LAYOUT(
		X__X,   X__X,   X__X, X__X,  X__X,  X__X,   X__X,   X__X, X__X,  X__X,   X__X, X__X,  X__X,  X__X,  X__X, 
		X__X,   X__X,    X__X,  X__X,   X__X,   X__X,             X__X,   X__X,  X__X,    X__X,   X__X,    X__X, X__X,  X__X,
		KC_TAB, KC_Q,    KC_W,  KC_E,   KC_R,   KC_T,    KC_Y,    KC_U,  KC_I,   KC_O,    KC_P,   X__X, X__X, X__X, X__X,
		KC_CAPS,KC_A,    KC_S,  KC_D,   KC_F,   KC_G,    KC_BSPC, KC_H,  KC_J,   KC_K,    KC_L,   X__X, X__X, X__X,  X__X,
		KC_LSFT,   KC_Z,    KC_X,  KC_C,   KC_V,   KC_B,    KC_ENT,  KC_N,  KC_M,   KC_COMM, KC_DOT, X__X, X__X, X__X,
		X__X,   KC_LGUI  , X__X,     X__X, X__X, X__X, KC_SPC,        KC_RALT, MO(_FUNC),  X__X, X__X, X__X, X__X),
  [_FUNC] = LAYOUT(
    RESET,  FUNC_DEFAULT,    FUNC_01, FUNC_ROGUE,X__X,  X__X,   X__X,  X__X,  X__X,  KC_CALC,KC_MYCM,KC_MSEL,KC_MAIL,NK_TOGG,EEP_RST,
    X__X,   X__X,    X__X,    X__X,    X__X,  X__X,          X__X,  X__X,  X__X,   X__X,   X__X,   X__X,   X__X,   KC_NLCK,
    RGB_TOG,RGB_MOD, RGB_VAI, RGB_HUI, X__X,  X__X,   X__X,  X__X,  X__X,  X__X,   X__X,   X__X,   X__X,   X__X,   KC_HOME,
    X__X,   RGB_SPD, RGB_VAD, RGB_SPI, X__X,  X__X,   X__X,  X__X,  X__X,  X__X,   X__X,   X__X,   X__X,   X__X,   KC_END,
    X__X,   X__X,    X__X,    X__X,    X__X,  X__X,   X__X,  X__X,  X__X,  X__X,   X__X,   X__X,   KC_MUTE,KC_VOLU,
    X__X,   X__X,    X__X,             X__X,  X__X,   X__X,  X__X,         X__X,   X__X,   KC_MPLY,KC_MPRV,KC_VOLD,KC_MNXT)
};

void matrix_init_user(void) {
  // NICOLA親指シフト
  set_nicola(_NICOLA);
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
  if( (layer_state_is(_QWERTY) || layer_state_is(_ROGUE)) && 
      ( keycode != FUNC_01 && keycode != FUNC_ROGUE && keycode != FUNC_DEFAULT)
    )  return true;

  if( record->event.pressed ){
    switch(keycode){
      case KC_HENK : 
				layer_off(_VI);
        nicola_on();
        return false;
      case KC_MHEN :
				layer_off(_VI);
        nicola_off();
        return false;
      case FUNC_01 :
          tap_code(KC_MHEN);
          layer_move(_QWERTY);
        return false;
      case FUNC_ROGUE:
          tap_code(KC_MHEN);
          layer_move(_ROGUE);
        return false;
			case VI_MODE :
        layer_invert(_VI);
  			tap_code(KC_MHEN);
        return false;
      case FUNC_DEFAULT:
        tap_code(KC_MHEN);
        layer_move(_DVORAK);
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




  