cmake_minimum_required(VERSION 3.28)
project(stars LANGUAGES CXX)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


option(SDL3_STATIC "Link SDL3 statically" OFF)

if(SDL3_STATIC)
    find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3-static)
else()
    find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3-shared)
endif()



add_executable(main src/main.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SDL3::SDL3)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
