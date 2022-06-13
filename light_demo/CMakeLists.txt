project(photo_model)

set(app_name photo_model)

file(GLOB headers *.h *.hpp)
file(GLOB src *.c *.cpp)

add_executable(photo_model ${headers} ${src})