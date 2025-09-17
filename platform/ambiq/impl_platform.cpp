extern "C" {
#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"
}
#include "platform.hpp"

struct AmbiqPlatform : Platform {
	void delay_ms(uint32_t ms) override { am_util_delay_ms(ms); }
	void led_toggle() override { am_hal_gpio_output_toggle(AM_BSP_GPIO_LED0); }
	void log(const std::string& s) override { am_util_stdio_printf("%s\n", s.c_str()); }
	bool i2c_read(uint8_t, uint8_t, uint8_t* buf, size_t len) override { (void)buf; (void)len; return false; /* TODO: am_hal_iom_* */ }
	bool i2c_write(uint8_t, uint8_t, const uint8_t*, size_t) override { return false; /* TODO: am_hal_iom_* */ }
};

// Factory for embedded
std::unique_ptr<Platform> make_ambiq_platform() {
	return std::make_unique<AmbiqPlatform>();
}
