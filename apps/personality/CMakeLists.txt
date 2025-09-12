cmake_minimum_required(VERSION 3.20.0)

# Tell CMake/Zephyr we’ll use C and C++
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(personality_app LANGUAGES C CXX)

# C++ standard & flags (tweak as you like)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Typical embedded knobs; enable if you want exceptions/RTTI later
add_compile_options(
		$<$<COMPILE_LANGUAGE:CXX>:
		-fno-exceptions
		-fno-rtti
		-fno-threadsafe-statics
		>
)

# Your sources (C++ main + modules)
target_sources(app PRIVATE
		src/main.cpp
		src/sched.cpp
		src/cli_shell.cpp
		# add more .cpp files as created
)

