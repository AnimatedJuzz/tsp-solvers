cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
SET(CMAKE_BUILD_TYPE "Debug")
project (TSP)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories("${PROJECT_BINARY_DIR}/include")

add_executable(TSPRunner src/graph.cpp src/tour.cpp src/main.cpp)
target_compile_features(TSPRunner PRIVATE cxx_range_for)