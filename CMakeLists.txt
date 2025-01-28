cmake_minimum_required(VERSION 3.20)  # Minimum CMake version
project(HelloWorld)                  # Define project name

include_directories(lib/glm)

# Define an executable target
add_executable(main main.cpp)
