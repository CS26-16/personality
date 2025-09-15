#include <cstdint>

extern "C" {
#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"
}

static void board_init() {
	am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);
	am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
	am_hal_cachectrl_enable();
	am_bsp_low_power_init();
}

int main(void) {
	board_init();

	// TODO: choose an EVB LED pin (from am_bsp_pins.h) and toggle here.
	// Example once you know LED pin:
	// am_hal_gpio_pinconfig(AM_BSP_GPIO_LED0, g_AM_HAL_GPIO_OUTPUT);
	// while (true) { am_hal_gpio_output_toggle(AM_BSP_GPIO_LED0); am_util_delay_ms(500); }

	while (true) {
		for (volatile uint32_t i = 0; i < 1000000; ++i) { /* delay */ }
	}
}
