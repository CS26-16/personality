#include "platform.hpp"
#include <memory>
#include <array>
#include <cstdint>
#include <string>

std::unique_ptr<Platform> make_host_platform(); // from host impl

int main() {
	auto plat = make_host_platform();
	plat->log("Host app start");

	// demo: poll fake sensor 0x68, reg 0x10, 6 bytes, blink LED
	std::array<uint8_t, 6> buf{};
	for (int i = 0; i < 10; ++i) {
		plat->led_toggle();
		plat->delay_ms(200);
		plat->i2c_read(0x68, 0x10, buf.data(), buf.size());
		plat->log("Sensor bytes: " + std::to_string(buf[0]) + "," +
				  std::to_string(buf[1]) + "," + std::to_string(buf[2]) + "...");
	}

	plat->log("Done.");
	return 0;
}
