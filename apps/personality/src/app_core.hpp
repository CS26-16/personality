#pragma once
#include <cstdint>

// simple sensor sample struct #TODO: Fully implement with proper sensor setup
struct Sample {
	uint8_t data[8];
};

// Template entrypoint, inlined for zero overhead on MCU
template <class Platform>
[[noreturn]] void run_app(Platform& plat) {
	plat.init_low_power();
	plat.log("Personality app starting...");

	while (true) {
		if (plat.sensor_ready()) {
			Sample s = plat.read_sample();
			plat.process(s);
		}
		plat.sleep_until_event(); // DEEPSLEEP + interrupt
	}
}
