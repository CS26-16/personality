cmake_minimum_required(VERSION 3.22)
project(personality LANGUAGES C CXX)

# ---- Toolchain (GNU Arm Embedded) ----
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# ---- Language / flags ----
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
		-mcpu=cortex-m4 -mthumb
		-ffunction-sections -fdata-sections
		$<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti>
		-Wall -Wextra
)

# ---- AmbiqSuite SDK root ----
set(AMBIQSUITE_DIR ${CMAKE_SOURCE_DIR}/third_party/AmbiqSuiteSDK)

# Includes
include_directories(
		${AMBIQSUITE_DIR}
		${AMBIQSUITE_DIR}/CMSIS/ARM/Include
		${AMBIQSUITE_DIR}/CMSIS/AmbiqMicro/Include
		${AMBIQSUITE_DIR}/mcu/apollo3
		${AMBIQSUITE_DIR}/mcu/apollo3/hal
		${AMBIQSUITE_DIR}/utils
		${AMBIQSUITE_DIR}/boards/apollo3_evb
)

# ---- Pick a linker script from the SDK (first match wins) ----
file(GLOB_RECURSE LD_CANDIDATES
		"${AMBIQSUITE_DIR}/boards/apollo3_evb/**.ld"
		"${AMBIQSUITE_DIR}/mcu/apollo3/gcc/**.ld"
)
list(LENGTH LD_CANDIDATES _ldn)
if(_ldn EQUAL 0)
	message(FATAL_ERROR "No Apollo3 EVB .ld found in AmbiqSuiteSDK. Point LINKER_SCRIPT to the right file.")
endif()
list(GET LD_CANDIDATES 0 LINKER_SCRIPT)
message(STATUS "Using linker script: ${LINKER_SCRIPT}")

# ---- Startup/system sources (names vary across SDKs; adjust if needed) ----
set(STARTUP_SOURCES
		${AMBIQSUITE_DIR}/mcu/apollo3/hal/am_hal_sysctrl.c
		${AMBIQSUITE_DIR}/mcu/apollo3/hal/am_hal_interrupt.c
		${AMBIQSUITE_DIR}/mcu/apollo3/hal/am_hal_cachectrl.c
		${AMBIQSUITE_DIR}/mcu/apollo3/hal/am_hal_clkgen.c
		${AMBIQSUITE_DIR}/mcu/apollo3/hal/am_hal_flash.c
		${AMBIQSUITE_DIR}/mcu/apollo3/hal/am_hal_gpio.c
		${AMBIQSUITE_DIR}/mcu/apollo3/hal/am_hal_uart.c
		${AMBIQSUITE_DIR}/mcu/apollo3/device/system_apollo3.c
		# If this file doesn't exist in your SDK, search for "startup_" under AmbiqSuiteSDK and replace:
		${AMBIQSUITE_DIR}/mcu/apollo3/startup_gcc.c
)

# ---- App ----
add_executable(personality.elf
		apps/personality/src/main.cpp
		${STARTUP_SOURCES}
)

# ---- Link ----
target_link_options(personality.elf PRIVATE
		-T${LINKER_SCRIPT}
		-Wl,--gc-sections
		-Wl,-Map=${CMAKE_BINARY_DIR}/personality.map
)

# ---- Binary artifact ----
find_program(OBJCOPY arm-none-eabi-objcopy)
add_custom_command(TARGET personality.elf POST_BUILD
		COMMAND ${OBJCOPY} -O binary personality.elf personality.bin
		COMMENT "Generating personality.bin"
)
