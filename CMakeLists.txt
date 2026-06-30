cmake_minimum_required(VERSION 3.31)

project(gpu_stable_power CXX)

if(MSVC)
    add_compile_options(/Zi /MP)
    add_link_options(/DEBUG)
endif()

add_library(gpu_stable_power)
target_sources(gpu_stable_power	PRIVATE src/gpu_stable_power/gpu_stable_power.cpp)
target_include_directories(gpu_stable_power PUBLIC src)
