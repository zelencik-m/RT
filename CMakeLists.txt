cmake_minimum_required(VERSION 3.20)  # Minimum CMake version
project(HelloWorld)                  # Define project name

include_directories(lib/glm)
include_directories(${CMAKE_SOURCE_DIR})

add_executable(main main.cpp)
