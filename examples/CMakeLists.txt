cmake_minimum_required(VERSION 3.0)

project(examples C)

if(NOT raylib_DIR)
    message(FATAL_ERROR "'raylib_DIR' is not specified! Please provide a path to the 'raylib' installation.")
endif()

if(NOT raygui_DIR)
    message(FATAL_ERROR "'raygui_DIR' is not specified! Please provide a path to the 'raygui' installation.")
endif()

# For now, raylib will be declared using raylib_DIR set through the cmake command-line.
add_subdirectory(${raylib_DIR} ${CMAKE_CURRENT_BINARY_DIR}/raylib)

add_executable(
    showcase
    showcase/main.c
)

target_link_libraries(
    showcase
    raylib
)

target_include_directories(
    showcase
    PRIVATE ${raylib_DIR}/src
    PRIVATE ${raygui_DIR}/src
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src
)
