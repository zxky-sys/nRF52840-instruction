#include "tianw84.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_power.h"
#include "nrfx_power.h"
#include "nrf.h"
#include "app_ble_func.h"

// adafruit bootloader, send "dfu" to debug serial port
#define DFU_MAGIC_UF2_RESET             0x57
void bootloader_jump(void) {
  sd_power_gpregret_set(0, DFU_MAGIC_UF2_RESET);
  NVIC_SystemReset();
}

static bool ble_flag = false;

void nrfmicro_power_enable(bool enable) {
	if (enable) {
		/*	
		nrf_gpio_cfg_output(POWER_PIN);	//disable power pin feature
		nrf_gpio_pin_set(POWER_PIN);
		*/
	} else {
		/*
		nrf_gpio_cfg_output(POWER_PIN); //disable power pin feature
		nrf_gpio_pin_clear(POWER_PIN); // works
		*/
		//input with pull-up consumes less than without it when pin is open (Hasu)
		//nrf_gpio_cfg_input(POWER_PIN, NRF_GPIO_PIN_PULLUP); // doesn't seem to work
		//nrf_gpio_cfg_input(POWER_PIN, NRF_GPIO_PIN_NOPULL); // neither this
		//nrf_gpio_cfg_input(POWER_PIN, NRF_GPIO_PIN_PULLDOWN); // or this
	}
}

void check_ble_switch(bool init) {
	uint8_t value = nrf_gpio_pin_read(SWITCH_PIN);

	if (init || ble_flag != value) {
		ble_flag = value;

		// mind that it doesn't disable BLE completely, it only disables send
		set_usb_enabled(!ble_flag);
		set_ble_enabled(ble_flag);

		nrf_gpio_pin_clear(LED_PIN);

		if (ble_flag) {
			// blink twice on ble enabled
			for (int i=0; i<2; i++) {
				nrf_gpio_pin_set(LED_PIN);
				nrf_delay_ms(100);
				nrf_gpio_pin_clear(LED_PIN);
				nrf_delay_ms(100);
			}
		}
	}
}

//switch to usb mode when usb cable connected
void check_usb_power()
{
	if (nrfx_power_usbstatus_get() == NRFX_POWER_USB_STATE_CONNECTED ||
    	nrfx_power_usbstatus_get() == NRFX_POWER_USB_STATE_READY) {
		if(!get_usb_enabled() && get_ble_enabled())
		{
    		set_usb_enabled(true);
			set_ble_enabled(false);
		}
  	} else {
  		if(get_usb_enabled() && !get_ble_enabled())
		{
    		set_usb_enabled(false);
			set_ble_enabled(true);
		}
  	}
}

void nrfmicro_init() {
  // configure pins
  // nrf_gpio_cfg_output(POWER_PIN);
  nrf_gpio_cfg_output(LED_PIN);
  nrf_gpio_cfg_input(SWITCH_PIN, NRF_GPIO_PIN_PULLDOWN);

  nrf_delay_ms(100);

  // nrfmicro_power_enable(false);
  check_ble_switch(true);

  // eeconfig_update_rgblight_default();
  // rgblight_enable();
}


void nrfmicro_update() {
  // check_ble_switch(false);
  check_usb_power();
}

