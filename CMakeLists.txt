cmake_minimum_required(VERSION 3.4...3.18)
project(chip_8)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
    SDL
    URL https://github.com/libsdl-org/SDL/archive/d629eb3bf093bc71c3d88e1f680b6bbacb33e0a0.zip
)

FetchContent_MakeAvailable(SDL) 

add_executable(chip_8 src/main.cpp) 

target_sources(chip_8 PRIVATE 
src/chip_8.cpp 
src/SDL3_platform.cpp
)

target_compile_options(chip_8 PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(chip_8 PRIVATE SDL3::SDL3)


