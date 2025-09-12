cmake_minimum_required(VERSION 3.20.0)

# Forward the build to the Zephyr app directory.
# The app CMakeLists already does `find_package(Zephyr ...)`.
add_subdirectory(apps/personality)
