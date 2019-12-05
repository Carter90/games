cmake_minimum_required(VERSION 3.13)  # CMake version check
project(Pokemon)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES Pokemon.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(Pokemon ${SOURCE_FILES})

configure_file(newpokemon.csv ${CMAKE_CURRENT_BINARY_DIR}/newpokemon.csv COPYONLY)
