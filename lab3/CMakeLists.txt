cmake_minimum_required(VERSION 3.5.0)
project(lab3 VERSION 0.1.0 LANGUAGES C)

add_custom_command(
    OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/www.tab.c
    COMMAND bison ${CMAKE_CURRENT_SOURCE_DIR}/www.y
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/www.y
)


add_custom_command(
    OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c
    COMMAND flex ${CMAKE_CURRENT_SOURCE_DIR}/www.l
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/www.l
)

add_executable(
    lab3
    ${CMAKE_CURRENT_BINARY_DIR}/www.tab.c
    ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c
)
target_link_libraries(lab3 fl)
