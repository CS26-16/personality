#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct Platform {
	virtual ~Platform() = default;

	// timing
	virtual void delay_ms(uint32_t ms) = 0;

	// gpio (demo LED)
	virtual void led_toggle() = 0;

	// logging
	virtual void log(const std::string& s) = 0;

	// i2c (simple register protocol for demo)
	virtual bool i2c_read(uint8_t addr, uint8_t reg, uint8_t* buf, size_t len) = 0;
	virtual bool i2c_write(uint8_t addr, uint8_t reg, const uint8_t* buf, size_t len) = 0;
};
