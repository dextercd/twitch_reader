cmake_minimum_required(VERSION 3.5)

set (CMAKE_CXX_STANDARD 17)

project(binary_find)

add_library(binary_find_lib INTERFACE)

target_include_directories(binary_find_lib INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
