cmake_minimum_required(VERSION 4.3.4)
project(stars LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()
option(SDL3_STATIC "Link SDL3 statically" OFF) 
if(SDL3_STATIC)
    find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3-static)
else()
    find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3-shared)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wsign-conversion")
set(CMAKE_CXX_FLAGS_DEBUG "-O2 -g -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

#main stuff
add_executable(main src/main.cpp src/init.cpp src/iterate.cpp src/quit.cpp src/events.cpp)
target_compile_features(main PRIVATE)
target_link_libraries(main PRIVATE SDL3::SDL3)

#lto stuff
include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error LANGUAGES CXX)
if( supported )
    message(STATUS "IPO / LTO enabled")
    set_property(TARGET main PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO / LTO not supported: <${error}>")
endif()


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
message(STATUS "build type: ${CMAKE_BUILD_TYPE}")
