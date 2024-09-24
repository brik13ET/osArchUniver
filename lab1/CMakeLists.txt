cmake_minimum_required(VERSION 3.27)
project(osArchUniver C)

set(CMAKE_C_STANDARD 11)

add_executable(osArchUniver main.c)
target_link_libraries(osArchUniver m)
