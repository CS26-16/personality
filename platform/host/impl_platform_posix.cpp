#include "platform.hpp"
#include <thread>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <memory>

namespace {
	struct FakeSensor {
		std::unordered_map<uint8_t,uint8_t> regs{{0x00, 0xA5}};
		std::mt19937 rng{std::random_device{}()};
		std::uniform_int_distribution<int> dist{0,255};
		bool read(uint8_t reg, uint8_t* buf, size_t len) {
			for (size_t i=0;i<len;i++) buf[i] = (reg+i==0x00)? regs[0x00] : static_cast<uint8_t>(dist(rng));
			return true;
		}
		bool write(uint8_t reg, const uint8_t* buf, size_t len) {
			for (size_t i=0;i<len;i++) regs[reg+i]=buf[i];
			return true;
		}
	} fake68;
}

struct HostPlatform final : Platform {
	void delay_ms(uint32_t ms) override { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }
	void led_toggle() override { static bool on{}; on=!on; std::cout << "[LED] " << (on?"ON":"OFF") << "\n"; }
	void log(const std::string& s) override { std::cout << s << "\n"; }
	bool i2c_read(uint8_t addr, uint8_t reg, uint8_t* buf, size_t len) override {
		if (addr==0x68) return fake68.read(reg, buf, len);
		std::fill(buf, buf+len, 0); return true;
	}
	bool i2c_write(uint8_t addr, uint8_t reg, const uint8_t* buf, size_t len) override {
		if (addr==0x68) return fake68.write(reg, buf, len);
		return true;
	}
};

std::unique_ptr<Platform> make_platform() {
	return std::make_unique<HostPlatform>();
}
