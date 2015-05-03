cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
SET(CMAKE_BUILD_TYPE "Debug")
project (TSP)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_NAME "TSPRunner")

include_directories("${PROJECT_BINARY_DIR}/bin/generated")

add_executable(TSPRunner src/graph.cpp src/tour.cpp src/display.cpp src/main.cpp)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)

file(GLOB files "${PROJECT_BINARY_DIR}/include/*.h")
foreach(file ${files})
  get_filename_component(name ${file} NAME)
  configure_file(${file} "${PROJECT_BINARY_DIR}/bin/generated/${name}")
endforeach()

if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_range_for)