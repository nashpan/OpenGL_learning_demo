cmake_minimum_required(VERSION 3.10)
project(compute_shader)

set(CMAKE_CXX_STANDARD 14)

set(app_name compute_shader)

set(GLFW_INCLUDE_DIR "E:/resources/glfw-3.3.4.bin.WIN64/glfw-3.3.4.bin.WIN64/include")
set(GLFW_LIBRARY "E:/resources/glfw-3.3.4.bin.WIN64/glfw-3.3.4.bin.WIN64/lib-vc2019/glfw3.lib")

set(GL3W_INCLUDE_DIR "E:/projects/RHI/libxyrhi/3rd/gl3w/include")
set(GL3W_LIBRARY "E:/projects/RHI/libxyrhi/3rd/gl3w")

add_library(gl3w STATIC ${GL3W_LIBRARY}/src/gl3w.c)
message(${GL3W_LIBRARY}/src/gl3w.c)

file(GLOB headers *.h *.hpp)
file(GLOB src *.c *.cpp)

add_executable(demo ${headers} ${src})

include_directories(${GLFW_INCLUDE_DIR}
                    ${GL3W_INCLUDE_DIR})

target_link_libraries(demo 
                     ${GLFW_LIBRARY}
                     gl3w)

