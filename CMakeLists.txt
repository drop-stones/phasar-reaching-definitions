# Version specification
cmake_minimum_required(VERSION 3.16)

# Project name
project(reaching_definitions)

# Compile setting
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# executable
add_executable(reaching_definitions
  main.cpp
  ReachingDefinitions.cpp
)

# PhASAR
find_package(phasar COMPONENTS controlflow REQUIRED)
find_package(phasar COMPONENTS mono REQUIRED)
target_include_directories(reaching_definitions PUBLIC ${PHASAR_INCLUDE_DIR})
target_link_directories(reaching_definitions PUBLIC ${PHASAR_LIBRARY_DIR})

phasar_config(reaching_definitions)
