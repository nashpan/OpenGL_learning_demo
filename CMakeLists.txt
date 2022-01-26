cmake_minimum_required(VERSION 3.10)
project(compute_shader)

set(CMAKE_CXX_STANDARD 14)

set(app_name compute_shader)

set(GLFW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glfw)
set(GLFW_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glfw/lib/vc-2019/glfw3.lib)
set(STB_IMAGE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/stbi)

set(GL3W_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/gl3w/include)

add_library(gl3w STATIC ${CMAKE_CURRENT_SOURCE_DIR}/3rd/gl3w/src/gl3w.c)

file(GLOB headers *.h *.hpp)
file(GLOB src *.c *.cpp)

add_executable(compute_shader_demo ${headers} ${src})

include_directories(${GLFW_INCLUDE_DIR}
                    ${GL3W_INCLUDE_DIR}
                    ${STB_IMAGE_INCLUDE_DIR})

target_link_libraries(compute_shader_demo 
                     ${GLFW_LIBRARY}
                     gl3w)

