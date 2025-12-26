cmake_minimum_required(VERSION 4.1)
project(desk_clock C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

add_executable(desk_clock main.c)

target_link_libraries(desk_clock PRIVATE PkgConfig::GTK4)
