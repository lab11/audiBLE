#include "audible.h"
#include "nrf_gpio.h"


// Resets the integrator
void audible_integrator_reset() {
	//TODO: Test if delay is needed
    nrf_gpio_pin_set(INT_DRAIN_PIN);
    nrf_gpio_pin_clear(INT_DRAIN_PIN);
}

// Turns on AudiBLE Hardware
void audible_power_up() {
	nrf_gpio_pin_set(AUDIBLE_LDO_EN_PIN);
	nrf_gpio_pin_clear(LTC1966_EN_PIN);
}

// Turns off AudiBLE Hardware
void audible_power_down() {
	nrf_gpio_pin_clear(AUDIBLE_LDO_EN_PIN);
	nrf_gpio_pin_set(LTC1966_EN_PIN);
}

// Inits AudiBLE
void audible_init() {
	nrf_gpio_cfg_output(INT_DRAIN_PIN);
	nrf_gpio_cfg_output(AUDIBLE_LDO_EN_PIN);
	nrf_gpio_cfg_output(LTC1966_EN_PIN);

	audible_power_up();
}