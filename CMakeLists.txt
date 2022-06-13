cmake_minimum_required(VERSION 3.10)
project(opengl_learning)

set(CMAKE_CXX_STANDARD 14)

set(app_name opengl_learning)

set(GLFW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glfw)
set(GLFW_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glfw/lib/vc-2019/glfw3.lib)
set(STB_IMAGE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/stbi)

set(GL3W_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/gl3w/include)

add_library(gl3w STATIC ${CMAKE_CURRENT_SOURCE_DIR}/3rd/gl3w/src/gl3w.c)

include_directories(${GLFW_INCLUDE_DIR}
                    ${GL3W_INCLUDE_DIR}
                    ${STB_IMAGE_INCLUDE_DIR})

                   

add_subdirectory(compute_shader_demo)
add_subdirectory(light_demo)