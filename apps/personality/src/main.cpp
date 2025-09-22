#include "app_core.hpp"

#if defined(PLATFORM_AMBIQ)
  #include "ambiq/ambiq_platform.hpp"
  using PlatformT = AmbiqPlatform;
#else
#include "host/host_platform.hpp"
using PlatformT = HostPlatform;
#endif

int main() {
	PlatformT plat;
	run_app(plat);   // template; inlines on MCU
}
