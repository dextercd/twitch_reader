cmake_minimum_required(VERSION 3.5)

set (CMAKE_CXX_STANDARD 17)

project(twitch)

set(sources
  "src/twitch_message.cpp"
)

set(headers
  "include/twitch/twitch_message.hpp"
)

add_library(twitch_lib STATIC
  ${sources}
  ${headers}
)

add_subdirectory(libs/irc)
add_subdirectory(libs/color)

target_link_libraries(twitch_lib
  irc_lib
  color_lib
)

target_include_directories(twitch_lib PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/twitch>
  $<INSTALL_INTERFACE:include/twitch>
)

target_include_directories(twitch_lib INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
