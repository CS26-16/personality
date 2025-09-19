#include "platform.hpp"
#include <memory>
#include <array>
#include <string>

// Implemented by each backend (host / ambiq)
std::unique_ptr<Platform> make_platform();

[[noreturn]] int main() {
	auto plat = make_platform();
	plat->log("Personality app start");

	// demo logic — this is your single source of truth
	plat->led_toggle();
	plat->delay_ms(200);

	std::array<uint8_t, 6> buf{};
	plat->i2c_read(0x68, 0x10, buf.data(), buf.size());
	plat->log("Sensor bytes[0..2]: " + std::to_string(buf[0]) + "," +
			  std::to_string(buf[1]) + "," + std::to_string(buf[2]));

	while (true) {
		plat->led_toggle();
		plat->delay_ms(500);
	}
}
