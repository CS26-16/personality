extern "C" {
#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"
}

#include "app_core.hpp"

struct AmbiqPlatform {
	static void init_low_power() {
		am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, nullptr);
		am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
		am_hal_cachectrl_enable();
		am_bsp_low_power_init();
		am_hal_gpio_pinconfig(AM_BSP_GPIO_LED0, g_AM_HAL_GPIO_OUTPUT);
		am_util_stdio_printf("Apollo3 init_low_power\n");
	}

	static void log(const std::string& s) { am_util_stdio_printf("%s\n", s.c_str()); }

	static bool sensor_ready() {
		// TODO: check an interrupt flag or timer
		return true;
	}

	static Sample read_sample() {
		Sample s{};
		// TODO: real IOM I2C read into s.data
		s.data[0] = 0x55;
		return s;
	}

	static void process(const Sample& s) {
		am_util_stdio_printf("Processing sample, first byte=%d\n", s.data[0]);
		am_hal_gpio_output_toggle(AM_BSP_GPIO_LED0);
	}

	static void sleep_until_event() {
		// Deep sleep until RTC/CTIMER/IOM wakes
		am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
	}
};

int main() {
	AmbiqPlatform plat;
	return run_app(plat);
}
