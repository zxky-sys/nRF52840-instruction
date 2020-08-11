/*
Copyright 2018 Sekigon
Copyright 2019 hatano.h

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
#include "app_ble_func.h"
#include "nrf_gpio.h"
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif
#ifdef WPM_ENABLE
  #include "wpm.h"
#endif

const uint8_t is_master = IS_LEFT_HAND;

// int RGB_current_mode;
#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
void nrfmicro_power_enable(bool enable);
#endif

enum custom_keycodes {
    // BLE keys
    NORM = SAFE_RANGE,	  /* Normal mode */
    AD_WO_L,		  /* Start advertising without whitelist  */
    SEL_BLE,		  /* Select BLE HID Sending		  */
    SEL_USB,		  /* Select USB HID Sending		  */
    TOG_HID,		  /* Toggle BLE/USB HID Sending		  */
    DELBNDS,              /* Delete all bonding                   */
    ADV_ID0,              /* Start advertising to PeerID 0        */
    ADV_ID1,              /* Start advertising to PeerID 1        */
    ADV_ID2,              /* Start advertising to PeerID 2        */
    ADV_ID3,              /* Start advertising to PeerID 3        */
    ADV_ID4,              /* Start advertising to PeerID 4        */
    ADV_ID5,              /* Start advertising to PeerID 5        */
    BATT_LV,              /* Display battery level in milli volts */
    DEL_ID0,              /* Delete bonding of PeerID 0           */
    DEL_ID1,              /* Delete bonding of PeerID 1           */
    DEL_ID2,              /* Delete bonding of PeerID 2           */
    DEL_ID3,              /* Delete bonding of PeerID 3           */
    DEL_ID4,              /* Delete bonding of PeerID 4           */
    DEL_ID5,              /* Delete bonding of PeerID 5           */
    ENT_DFU,              /* Start bootloader                     */
    ENT_SLP,              /* Deep sleep mode                      */

    RGBRST,
    RGBTOG,
    CST_MVP
};

enum { TO_SETTINGS=0 };

extern keymap_config_t keymap_config;

enum layer_number {
    _BASE = 0,
    _FN1,
    _SETTINGS,
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

void dance_layer_TO_SETTINGS(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {  // TAP THREE TIMES TO ENTER SETTINGS
        layer_on(_SETTINGS);
        reset_tap_dance (state);
    }
    else
    {
    	register_code (KC_GRV);
    }
}
void dance_layer_TO_SETTINGS_RESET(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) {  // TAP THREE TIMES TO ENTER SETTINGS
    }
    else
    {
    	unregister_code (KC_GRV);
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {[TO_SETTINGS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,dance_layer_TO_SETTINGS,dance_layer_TO_SETTINGS_RESET)};

// define keymaps
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	  [_BASE] = LAYOUT(
		KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_MUTE, KC_VOLD, KC_VOLU, 
		KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_POWER, 	
		KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, LCTL(LGUI(KC_N)), 
		KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_PGUP, 
		KC_LSPO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSPC, KC_UP, KC_PGDN, 
		KC_LCTL, KC_LALT, KC_LGUI, KC_SPC, KC_RGUI, MO(_FN1), KC_RALT, KC_LEFT, KC_DOWN, KC_RGHT),
	  [_FN1] = LAYOUT(
		TD(TO_SETTINGS), ADV_ID0, ADV_ID1, ADV_ID2, ADV_ID3, TOG_HID, _______, _______, _______, _______, _______, _______, _______, _______, _______, DELBNDS, 
		_______, DEL_ID0, DEL_ID1, DEL_ID2, DEL_ID3, _______, _______, _______, _______, _______, _______, _______, _______, _______, AD_WO_L, 
		_______, RGB_TOG, RGB_MOD, RGB_VAI, RGB_VAD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, _______, _______, _______, _______, _______, _______, 
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_VAI, RGBRST,
		_______, _______, _______, _______, _______, _______, _______, RGBTOG, RGB_VAD, RGB_MOD),
    [_SETTINGS]=LAYOUT(
		TO(_BASE), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RESET, 
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, AD_WO_L, 	
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
		XXXXXXX, XXXXXXX, ENT_SLP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_VAI, RGBRST, 
		XXXXXXX, XXXXXXX, XXXXXXX, ENT_DFU, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX),
};

#ifndef TAPPING_TERM_PER_KEY
#define get_tapping_term(kc)	(TAPPING_TERM)
#endif

const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

static bool process_record_user_special(uint16_t keycode, bool pressed) {
  #ifdef SSD1306OLED
  iota_gfx_flush(); // wake up screen
#endif
  switch (keycode) {
  case NORM:
    if (pressed) {
      set_single_persistent_default_layer(_BASE);
    }
    break;
  case DELBNDS:
    if (pressed)
      delete_bonds();
    break;
  case AD_WO_L:
    if (pressed)
      restart_advertising_wo_whitelist();
    break;
  case SEL_BLE:
    if (pressed) {
      set_ble_enabled(true);
      set_usb_enabled(false);
    }
    break;
  case SEL_USB:
    if (pressed) {
      set_ble_enabled(false);
      set_usb_enabled(true);
    }
    break;
  case TOG_HID:
#ifndef NRF_SEPARATE_KEYBOARD_SLAVE
    // workaround:
    // get_ble_enabled() macro(in app_ble_func.h) is incorrect.
    if (pressed) {
      bool ble = get_ble_enabled(); 
      set_ble_enabled(!ble);
      set_usb_enabled(ble);
    }
#endif
    break;
  case ADV_ID0:
  case ADV_ID1:
  case ADV_ID2:
  case ADV_ID3:
  case ADV_ID4:
  case ADV_ID5:
    if (pressed)
      restart_advertising_id(keycode-ADV_ID0);
    break;
  case DEL_ID0:
  case DEL_ID1:
  case DEL_ID2:
  case DEL_ID3:
  case DEL_ID4:
  case DEL_ID5:
    if (pressed)
      delete_bond_id(keycode-DEL_ID0);
    break;
  case BATT_LV:
    if (pressed) {
      char str[16];
      sprintf(str, "%4dmV", get_vcc());
      send_string(str);
    }
    break;
  case ENT_DFU:
    if (pressed)
      bootloader_jump();
    break;
  case ENT_SLP:
    if (!pressed)
    // #ifdef SSD1306OLED
    //   iota_gfx_off();
    // #endif
      sleep_mode_enter();
    break;

  default:
    // other unspecial keys
    return true;
  }
  return false;
}

char bat_state_str[24];
char bat_percentage_str[24];
void set_bat_state(void) {
  uint8_t value = nrf_gpio_pin_read(SWITCH_PIN);

  if(value)
  {
    snprintf(bat_state_str, sizeof(bat_state_str), "VOLT: %4d MV",
           get_vcc());
    snprintf(bat_percentage_str, sizeof(bat_percentage_str), "&': %d %%",
           (get_vcc()-2400)/18);
  } else {
    snprintf(bat_state_str, sizeof(bat_state_str), "VOLT: CHECK SWITCH");
    snprintf(bat_percentage_str, sizeof(bat_percentage_str), "&': CHECK SWITCH");
  }

}

const char *read_bat_state(void) {
  return bat_state_str;
}

const char *read_bat_percentage(void) {
  return bat_percentage_str;
}

char hid_state_str[24];
const char *read_hid_state(void) {
#if defined IS_LEFT_HAND  &&  IS_LEFT_HAND == true
  snprintf(hid_state_str, sizeof(hid_state_str), "(): %s %s",
           (get_usb_enabled()) ? "!\"" : " ",
           (get_ble_enabled()) ? "#$" : " ");
#endif
  return hid_state_str;
}

#ifdef WPM_ENABLE
char wpm_str[24];
void set_wpm(void) {
    snprintf(wpm_str, sizeof(wpm_str), "*+: %d", get_current_wpm());
}
const char *read_wpm(void) {
  return wpm_str;
}
#endif

#ifdef RGBLIGHT_ENABLE
char rgb_state_str[24];
const char *read_rgb_state(void)
{
    snprintf(rgb_state_str, sizeof(rgb_state_str), "LIGHT %s MODE: %d",
    rgblight_config.enable ? "ON " : "OFF", rgblight_config.mode);
    return rgb_state_str;
}
#endif

char mod_shift_win_str[24];
const char *read_shift_win_state(void)
{
      snprintf(mod_shift_win_str, sizeof(mod_shift_win_str), "%s %s",
        get_mods() & MOD_BIT(KC_LSHIFT)? "]^" : "  ",
        get_mods() & MOD_BIT(KC_LGUI)? "\\" : " " );
    return mod_shift_win_str; 
}

char mod_ctrl_alt_str[24];
const char *read_ctrl_alt_state(void)
{
      snprintf(mod_ctrl_alt_str, sizeof(mod_ctrl_alt_str), "%s %s",
        get_mods() & MOD_BIT(KC_LCTL)? "_" : " ",
        get_mods() & MOD_BIT(KC_LALT)? "{" : " " );
    return mod_ctrl_alt_str; 
}

char version_str[24];
const char *read_version(void)
{
      snprintf(version_str, sizeof(version_str), "VER 0510 NOEE");
    return version_str; 
}


/*
char caps_lock_state[24];
const char *read_caps_lock_state(void)
{
  uint8_t leds = host_keyboard_leds();
  snprintf(caps_lock_state, sizeof(caps_lock_state), "CL:%s",
           (leds & (1 << USB_LED_CAPS_LOCK)) ? "1" : "0");
  return caps_lock_state;
}
*/


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch(keycode)
  {
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          // nrfmicro_power_enable(true);
          eeconfig_update_rgblight_default();
          rgblight_enable();
          // RGB_current_mode = rgblight_config.mode;
          // NRF_LOG_INFO("RGBRST, RGB_current_mode: %d\n", RGB_current_mode);
        }
      #endif
      break;
    case RGBTOG:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          if(!rgblight_config.enable) {
            eeconfig_update_rgblight_default();
            rgblight_enable();
          }
          else{
            rgblight_disable();
          }
        }
      #endif
      break;
  }
  if (record->event.pressed) {
    set_bat_state();
    // set_keylog(keycode, record);
    #ifdef WPM_ENABLE
      update_wpm(keycode);
      set_wpm();
    #endif
  }
  switch (keycode) {
  default:
    // unset_layer(record);
    return process_record_user_special(keycode, record->event.pressed);
  }
  return false;
}

#ifdef SSD1306OLED

void matrix_update(struct CharacterMatrix *dest,
                          const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

void matrix_render_user(struct CharacterMatrix *matrix) {
    switch(biton32(layer_state))
    {
      case _BASE:
        // matrix_write_ln(matrix, read_layer_state());
        matrix_write_ln(matrix, read_hid_state());
        matrix_write_ln(matrix, read_bat_percentage());
        #ifdef WPM_ENABLE
          matrix_write_ln(matrix, read_wpm());
        #endif
        matrix_write_ln(matrix,read_shift_win_state());
        matrix_write(matrix,read_ctrl_alt_state());
        //others:
        // matrix_write(matrix,read_caps_lock_state());   
        // matrix_write_ln(matrix, read_keylog());
        // matrix_write_ln(matrix, read_keylogs());
        //matrix_write_ln(matrix, read_mode_icon(keymap_config.swap_lalt_lgui));
        //matrix_write_ln(matrix, read_host_led_state());
        //matrix_write_ln(matrix, read_timelog());
      break;
      case _EXTRA:
        matrix_write_ln(matrix, "./");
        matrix_write_ln(matrix, read_bat_state());
        matrix_write_ln(matrix,read_shift_win_state());
        matrix_write(matrix,read_ctrl_alt_state());
      break;
      case _SETTINGS:
        matrix_write_ln(matrix, ",-             [ESC");
        matrix_write_ln(matrix, "S: SYSTEM");
        matrix_write_ln(matrix, "B: BLUETOOTH");
        matrix_write(matrix, "R: RGB LIGHT");
        break;
      case _SYSTEM_SETTINGS:
        matrix_write_ln(matrix, ",- =>          [ESC");
        // matrix_write_ln(matrix, "P: SLEEP");
        matrix_write_ln(matrix, "B: ENTER BOOT");
        matrix_write_ln(matrix, read_version());
      break;
      case _BLE_SETTINGS:
        matrix_write_ln(matrix, ",- #$          [ESC");
        matrix_write_ln(matrix, "T: TOGGLE HID");
        matrix_write_ln(matrix, "DEL: DEL BONDS");
      break;
      case _RGB_SETTINGS:
      #ifdef RGBLIGHT_ENABLE
        matrix_write_ln(matrix, ",- ;<          [ESC");
        matrix_write_ln(matrix, read_rgb_state());
        matrix_write_ln(matrix, "T: TOGGLE E: RESET ");
        matrix_write(matrix, "M: MODE HUE:ENCODER");
      #else
        matrix_write_ln(matrix, ",- ;<          [ESC");
        matrix_write_ln(matrix, "RGB NOT SUPPORTED");
      #endif
      break;
      default:
      matrix_write_ln(matrix, "ERROR LAYER");
      break;
    }
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;
  matrix_clear(&matrix);
  matrix_render_user(&matrix);
  matrix_update(&display, &matrix);
}

#endif

//Media keys not working, don't know why
#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
  switch(biton32(layer_state))
  {
    case _BASE:
      if(clockwise){
        register_code(KC_SLCK);
        unregister_code(KC_SLCK);
      }
      else{
        register_code(KC_PAUS);
        unregister_code(KC_PAUS);
      }
      break;
    case _EXTRA:
      if(clockwise){
        register_code(KC_PGUP);
        unregister_code(KC_PGUP);
      }
      else{
        register_code(KC_PGDN);
        unregister_code(KC_PGDN);
      }
      break;
    case _SETTINGS:
    case _SYSTEM_SETTINGS:
    case _BLE_SETTINGS:
    break;
    case _RGB_SETTINGS:
      if (get_mods() & MOD_BIT(KC_LALT)) {
        if(clockwise){
          rgblight_decrease_val();
        } else {
          rgblight_increase_val();
        }
      } else if (get_mods() & MOD_BIT(KC_LCTL)){
        if(clockwise){
          rgblight_decrease_sat();
        } else {
          rgblight_increase_sat();
        }
      } else {
        if(clockwise){
          rgblight_decrease_hue();
        } else {
          rgblight_increase_hue();
        }
      }
      break;
    default:
    break;
  }
}
#endif
