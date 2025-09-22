extern "C" {
#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"
}
#include "ambiq_platform.hpp"

void AmbiqPlatform::init_low_power() {
	am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);
	am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
	am_hal_cachectrl_enable();
	am_bsp_low_power_init();
	am_hal_gpio_pinconfig(AM_BSP_GPIO_LED0, g_AM_HAL_GPIO_OUTPUT);
	am_util_stdio_printf("Apollo3 init_low_power\n");
}

void AmbiqPlatform::log(const std::string& s) { am_util_stdio_printf("%s\n", s.c_str()); }

bool AmbiqPlatform::sensor_ready() { return true; /* TODO: event/IRQ */ }

Sample AmbiqPlatform::read_sample() {
	Sample s{}; s.data[0] = 0x55; /* TODO: IOM read */
	return s;
}

void AmbiqPlatform::process(const Sample& s) {
	am_util_stdio_printf("Processing sample, first byte=%d\n", s.data[0]);
	am_hal_gpio_output_toggle(AM_BSP_GPIO_LED0);
}

void AmbiqPlatform::sleep_until_event() {
	am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
}
