cmake_minimum_required(VERSION 3.5)

set (CMAKE_CXX_STANDARD 17)

project(color)

add_library(color_lib INTERFACE)

target_include_directories(color_lib INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
