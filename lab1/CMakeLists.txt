cmake_minimum_required(VERSION 3.5.0)
project(lab1 VERSION 0.1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(lab1 main.c)

target_link_libraries(lab1 m)
target_link_libraries(lab1 pthread)
target_compile_definitions(lab1 PUBLIC _POSIX_C_SOURCE)