# todo: clean this up someday
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_FLAGS "-Wall -std=c++20")

if(APPLE)
    set(CMAKE_C_COMPILER "/opt/homebrew/Cellar/gcc/11.3.0_2/bin/gcc-11")
    set(CMAKE_CXX_COMPILER "/opt/homebrew/Cellar/gcc/11.3.0_2/bin/g++-11")
elseif(WIN32)
    message("This is a Windows System")
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)
SET(LIBFLTK_PATH "/usr/local/lib")

project(SNES++ VERSION 0.1)

include_directories(/usr/local/include)
find_library(FLTK NAMES fltk PATHS ${LIBFLTK_PATH})
find_library(FLTK_FORMS NAMES fltk_forms PATHS ${LIBFLTK_PATH})
find_library(FLTK_GL NAMES fltk_gl PATHS ${LIBFLTK_PATH})
find_library(FLTK_IMAGES NAMES fltk_images PATHS ${LIBFLTK_PATH})
find_library(FLTK_JPEG NAMES fltk_jpeg PATHS ${LIBFLTK_PATH})
find_library(FLTK_PNG NAMES fltk_png PATHS ${LIBFLTK_PATH})


add_library(core STATIC core/core.cpp)
add_library(cpu STATIC core/cpu.cpp)
add_library(ram STATIC core/ram.cpp)
add_library(MainMenu STATIC gui/MainMenu.cpp)
add_executable(snes++ main.cpp)

if(APPLE)
    find_library(COCOA_LIBRARY Cocoa ONLY)
    target_link_libraries(snes++ core cpu ram MainMenu
    ${COCOA_LIBRARY}
    ${FLTK}
    ${FLTK_FORMS}
    ${FLTK_GL}
    ${FLTK_IMAGES}
    ${FLTK_JPEG}
    ${FLTK_PNG})
else()
    target_link_libraries(snes++ core cpu ram MainMenu
        ${FLTK}
        ${FLTK_FORMS}
        ${FLTK_GL}
        ${FLTK_IMAGES}
        ${FLTK_JPEG}
        ${FLTK_PNG})
endif()