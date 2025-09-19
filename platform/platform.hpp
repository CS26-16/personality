#pragma once
#include <string>

struct Sample;

// Each backend implements this shape (keeps virtualization power down on MCU)
struct Platform {
	void init_low_power();
	void log(const std::string&);
	bool sensor_ready();
	Sample read_sample();
	void process(const Sample&);
	void sleep_until_event();
};
