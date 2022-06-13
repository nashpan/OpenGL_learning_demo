cmake_minimum_required(VERSION 3.10)
project(compute_shader)

set(CMAKE_CXX_STANDARD 14)

set(app_name compute_shader)

file(GLOB headers *.h *.hpp)
file(GLOB src *.c *.cpp)

add_executable(compute_shader_demo ${headers} ${src})
target_link_libraries(compute_shader_demo 
                     ${GLFW_LIBRARY}
                     gl3w)

