cmake_minimum_required(VERSION 3.5.0)
project(lab2 VERSION 0.1.0 LANGUAGES C)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_C_STANDARD 11)


find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED) 


add_executable(lab2 main.c) 

target_link_libraries(lab2 PUBLIC m)
target_link_libraries(lab2 PUBLIC GLEW)
target_link_libraries(lab2 PUBLIC GLU)
target_link_libraries(lab2 PUBLIC GL)
# target_link_libraries(lab2 PUBLIC OpenGL)
# target_link_libraries(lab2 PUBLIC EGL)
target_link_libraries(lab2 PUBLIC glfw)

add_custom_command(
        TARGET lab2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/frag.glsl
				${CMAKE_CURRENT_BINARY_DIR}/frag.glsl)

add_custom_command(
	TARGET lab2 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_SOURCE_DIR}/vertex.glsl
			${CMAKE_CURRENT_BINARY_DIR}/vertex.glsl)

