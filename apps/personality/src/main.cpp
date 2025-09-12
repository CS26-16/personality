// main.cpp

extern "C" {
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
}
LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

class Blinker {
public:
	void loop() const {
		while (true) {
			LOG_INF("Tick (C++)");
			k_sleep(K_SECONDS(1));
		}
	}
};

int main() {
	LOG_INF("Hello from Personality app (C++17)!");
	Blinker b;
	b.loop();
	return 0;
}
