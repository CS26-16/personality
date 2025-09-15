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
set(AMBIQSUITE_DIR ${CMAKE_SOURCE_DIR}/third-party/AmbiqSuiteSDK)

# Includes
include_directories(
		${AMBIQSUITE_DIR}
		${AMBIQSUITE_DIR}/CMSIS/ARM/Include
		${AMBIQSUITE_DIR}/CMSIS/AmbiqMicro/Include
		${AMBIQSUITE_DIR}/CMSIS/AmbiqMicro/Source
		${AMBIQSUITE_DIR}/mcu/apollo3
		${AMBIQSUITE_DIR}/mcu/apollo3/hal
		${AMBIQSUITE_DIR}/utils
		${AMBIQSUITE_DIR}/boards/apollo3_evb
)

# ---- Linker script ----
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/toolchain/apollo3_evb_sbl.ld)

message(STATUS "Using linker script: ${LINKER_SCRIPT}")

# ---- Startup/system sources (names vary across SDKs; adjust if needed) ----
set(STARTUP_SOURCES
		${AMBIQSUITE_DIR}/CMSIS/AmbiqMicro/Source/system_apollo3.c
		${AMBIQSUITE_DIR}/CMSIS/AmbiqMicro/Source/startup_apollo3.s
)

# ---- App ----
add_executable(personality.elf
		apps/personality/src/main.cpp
		${STARTUP_SOURCES}
)

# ---- Link ----
# Link the prebuilt SDK libs (HAL + BSP)
target_link_libraries(personality.elf PRIVATE
		${AMBIQSUITE_DIR}/mcu/apollo3/gcc/bin/libam_hal.a
		${AMBIQSUITE_DIR}/boards/apollo3_evb/gcc/bin/libam_bsp.a
		
		# Standard libs (grouping helps circular refs)
		-Wl,--start-group
		m c gcc nosys
		-Wl,--end-group
)

target_link_options(personality.elf PRIVATE
		-T${LINKER_SCRIPT}
		-Wl,--gc-sections -Wl,-Map=${CMAKE_BINARY_DIR}/personality.map
)

# ---- Binary artifact ----
find_program(OBJCOPY arm-none-eabi-objcopy)
add_custom_command(TARGET personality.elf POST_BUILD
		COMMAND ${OBJCOPY} -O binary personality.elf personality.bin
		COMMENT "Generating personality.bin"
)
