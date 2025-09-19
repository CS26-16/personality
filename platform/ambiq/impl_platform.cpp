extern "C" {
#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"
}
#include "platform/platform.hpp"
#include <memory>

struct AmbiqPlatform final : Platform {
	void delay_ms(uint32_t ms) override { am_util_delay_ms(ms); }
	void led_toggle() override { am_hal_gpio_output_toggle(AM_BSP_GPIO_LED0); }
	void log(const std::string& s) override { am_util_stdio_printf("%s\n", s.c_str()); }
	bool i2c_read(uint8_t, uint8_t, uint8_t*, size_t) override { /* TODO: am_hal_iom_* */ return false; }
	bool i2c_write(uint8_t, uint8_t, const uint8_t*, size_t) override { /* TODO: am_hal_iom_* */ return false; }
};

std::unique_ptr<Platform> make_platform() {
	// do any board init here if needed
	am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);
	am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
	am_hal_cachectrl_enable();
	am_bsp_low_power_init();
	am_hal_gpio_pinconfig(AM_BSP_GPIO_LED0, g_AM_HAL_GPIO_OUTPUT);
	return std::make_unique<AmbiqPlatform>();
}
