cmake_minimum_required (VERSION 2.6)
project (TSP)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories("${PROJECT_BINARY_DIR}/include")

add_executable(TSPRunner src/main.cpp src/graph.cpp)