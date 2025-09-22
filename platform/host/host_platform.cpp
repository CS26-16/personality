#include "host_platform.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <thread>

// ---- OS-specific knobs ----
#if defined(_WIN32)
// Windows-specific includes go here
static constexpr const char* kHostOS = "Windows";
#else
// POSIX / macOS / Linux
static constexpr const char* kHostOS = "POSIX";
#endif

void HostPlatform::init_low_power() {
	log(std::string("[Host/") + kHostOS + "] init_low_power (noop on host)");
}

void HostPlatform::log(const std::string& s) {
	std::cout << s << std::endl;
}

bool HostPlatform::sensor_ready() {
	// Always ready in the fake; wire real event/condition if you simulate IRQs.
	return true;
}

Sample HostPlatform::read_sample() {
	static std::mt19937 rng{std::random_device{}()};
	Sample s{};
	std::generate(std::begin(s.data), std::end(s.data),
				  [&]{ return static_cast<uint8_t>(rng() & 0xFF); });
	return s;
}

void HostPlatform::process(const Sample& s) {
	log("[Host] Processing sample, first byte=" + std::to_string(s.data[0]));
}

void HostPlatform::sleep_until_event() {
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(500ms); // portable sleep
}
