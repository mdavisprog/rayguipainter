cmake_minimum_required(VERSION 3.0)

project(examples C)

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
)
