cmake_minimum_required(VERSION 3.5)

set (CMAKE_CXX_STANDARD 17)

project(irc)

set(sources
  "src/message_parser.cpp"
  "src/message.cpp"
)

set(headers
  "include/irc/command_type_list.hpp"
  "include/irc/message.hpp"
  "include/irc/message_parser.hpp"
)

add_library(irc_lib ${sources} ${headers})

add_subdirectory(libs/binary_find)
target_link_libraries(irc_lib binary_find_lib)

target_include_directories(irc_lib PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/irc>
  $<INSTALL_INTERFACE:include/irc>
)

target_include_directories(irc_lib INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

enable_testing()
add_subdirectory(test)
