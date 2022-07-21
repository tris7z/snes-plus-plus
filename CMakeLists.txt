cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

if(APPLE)
    set(CMAKE_C_COMPILER "/opt/homebrew/Cellar/gcc/11.3.0_2/bin/gcc-11")
    set(CMAKE_CXX_COMPILER "/opt/homebrew/Cellar/gcc/11.3.0_2/bin/g++-11")
elseif(WIN32)
    message("This is a Windows System")
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

project(SNES++ VERSION 0.1)

add_library(core STATIC core/core.cpp)
add_library(cpu STATIC core/cpu.cpp)
add_library(ram STATIC core/ram.cpp)
add_executable(snes++ main.cpp)
target_link_libraries(snes++ core cpu ram)