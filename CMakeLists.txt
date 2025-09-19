cmake_minimum_required(VERSION 3.22)
project(personality LANGUAGES C CXX)

# Build only the host target when ON (no cross/ARM config touched)
option(HOST_ONLY "Build host targets only (no cross/ARM stuff)" OFF)

# --------------------------------------------------------------------
# Locate AmbiqSuiteSDK (support both third_party/ and third-party/)
# --------------------------------------------------------------------
set(AMBIQ_DIR_CANDIDATES
		${CMAKE_SOURCE_DIR}/third_party/AmbiqSuiteSDK
		${CMAKE_SOURCE_DIR}/third-party/AmbiqSuiteSDK
)
set(AMBIQSUITE_DIR "")
foreach(P ${AMBIQ_DIR_CANDIDATES})
	if(EXISTS "${P}/CMSIS" AND EXISTS "${P}/mcu/apollo3")
		set(AMBIQSUITE_DIR "${P}")
	endif()
endforeach()
message(STATUS "AmbiqSuiteSDK root: ${AMBIQSUITE_DIR}")

# ===========================
# Host target (native build)
# ===========================
set(HOST_SOURCES
		platform/host/impl_platform_posix.cpp
		apps/personality/src/app_core.hpp
)

if(WIN32)
	list(APPEND HOST_SOURCES platform/host/impl_platform_win.cpp)
else()
	list(APPEND HOST_SOURCES platform/host/impl_platform_posix.cpp)
endif()

add_executable(personality_host ${HOST_SOURCES})

# Add include path so #include "platform.hpp" works
target_include_directories(personality_host PRIVATE
		${CMAKE_SOURCE_DIR}/platform
		${CMAKE_SOURCE_DIR}/apps/personality/src
)

target_compile_features(personality_host PRIVATE cxx_std_17)
target_compile_options(personality_host PRIVATE -Wall -Wextra)

if(MSVC)
	target_compile_definitions(personality_host PRIVATE _CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
	target_link_libraries(personality_host PRIVATE c++)
endif()

# ===============================
# Embedded target (ARM toolchain)
# ===============================
if(NOT HOST_ONLY)
	# --- Cross toolchain (scoped to embedded build only) ---
	set(CMAKE_SYSTEM_NAME Generic)
	set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
	set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
	set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
	set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
	
	# --- Includes ---
	include_directories(
			${AMBIQSUITE_DIR}
			${AMBIQSUITE_DIR}/CMSIS/ARM/Include
			${AMBIQSUITE_DIR}/CMSIS/AmbiqMicro/Include
			${AMBIQSUITE_DIR}/mcu/apollo3
			${AMBIQSUITE_DIR}/mcu/apollo3/hal
			${AMBIQSUITE_DIR}/utils
			${AMBIQSUITE_DIR}/boards/apollo3_evb
	)
	
	# --- Linker script (your local one; swap to SDK .ld later if you prefer) ---
	set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/toolchain/apollo3_evb_sbl.ld)
	message(STATUS "Using linker script: ${LINKER_SCRIPT}")
	
	# --- Startup + system sources (from CMSIS in your SDK) ---
	set(SYSTEM_APOLLO3  ${AMBIQSUITE_DIR}/CMSIS/AmbiqMicro/Source/system_apollo3.c)
	set(STARTUP_APOLLO3 ${AMBIQSUITE_DIR}/CMSIS/AmbiqMicro/Source/startup_apollo3.s)
	
	add_executable(personality.elf
			platform/ambiq/impl_platform.cpp
			apps/personality/src/app_core.hpp
			${SYSTEM_APOLLO3}
			${STARTUP_APOLLO3}
	)
	
	target_include_directories(personality.elf PRIVATE
			${CMAKE_SOURCE_DIR}/platform
			${CMAKE_SOURCE_DIR}/apps/personality/src
	)
	
	# Cortex-M flags ONLY for the embedded target
	target_compile_options(personality.elf PRIVATE
			-mcpu=cortex-m4 -mthumb
			-ffunction-sections -fdata-sections
			-Wall -Wextra
			$<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions;-fno-rtti;-fno-threadsafe-statics>
	)
	
	target_link_options(personality.elf PRIVATE
			-T${LINKER_SCRIPT}
			-Wl,--gc-sections
			-Wl,-Map=${CMAKE_BINARY_DIR}/personality.map
	)
	
	# Link prebuilt HAL + BSP from the SDK
	target_link_libraries(personality.elf PRIVATE
			${AMBIQSUITE_DIR}/mcu/apollo3/gcc/bin/libam_hal.a
			${AMBIQSUITE_DIR}/boards/apollo3_evb/gcc/bin/libam_bsp.a
			-Wl,--start-group m c gcc nosys -Wl,--end-group
	)
	
	# Create .bin next to the .elf
	find_program(OBJCOPY arm-none-eabi-objcopy)
	add_custom_command(TARGET personality.elf POST_BUILD
			COMMAND ${OBJCOPY} -O binary personality.elf personality.bin
			COMMENT "Generating personality.bin"
	)
endif()
