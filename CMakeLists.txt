cmake_minimum_required(VERSION 3.10)
project(
    ex16
    VERSION 1.0.0
    LANGUAGES C
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include(${PROJECT_SOURCE_DIR}/res/FindSDL2TTF.cmake)

add_subdirectory(src)
