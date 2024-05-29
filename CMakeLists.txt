cmake_minimum_required(VERSION 3.5)
project(space)

cmake_policy(SET CMP0072 NEW)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)

include_directories(libs)
include_directories(include)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE LIBS    ${CMAKE_CURRENT_SOURCE_DIR}/libs/*.c ${CMAKE_CURRENT_SOURCE_DIR}/libs/*.cpp)

add_executable(space ${LIBS} ${SOURCES} main.cpp)
target_link_libraries(space glfw OpenGL::GL)
