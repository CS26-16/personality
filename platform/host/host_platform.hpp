#pragma once
#include <string>
#include "app_core.hpp"

struct HostPlatform {
	void init_low_power();
	void log(const std::string& s);
	bool sensor_ready();
	Sample read_sample();
	void process(const Sample& s);
	void sleep_until_event();
};
