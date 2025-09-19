#include "app_core.hpp"
#include <thread>
#include <chrono>
#include <iostream>
#include <random>
#include <algorithm>
#include <string>
using namespace std;

struct HostPlatform {
	static void init_low_power() { log("[Host] init_low_power"); }
	static void log(const string& s) { cout << s << endl; }
	static bool sensor_ready() { return true; } // always ready in fake
	static Sample read_sample() {
		Sample s{};
		static mt19937 rng{random_device{}()};
		generate(begin(s.data), end(s.data),
					  [&]{ return static_cast<uint8_t>(rng()%256); });
		return s;
	}
	static void process(const Sample& s) {
		log("[Host] Processing sample, first byte=" + to_string(s.data[0]));
	}
	static void sleep_until_event() {
		this_thread::sleep_for(chrono::milliseconds(500));
	}
};

int main() {
	HostPlatform plat;
	run_app(plat);
}
