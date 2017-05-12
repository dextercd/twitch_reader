cmake_minimum_required(VERSION 3.5)

set (CMAKE_CXX_STANDARD 17)

project(irc)

set(sources
  "message_parser.cpp"
  "message.cpp"
  "../../include/irc/command_type_list.hpp"
  "../../include/irc/message.hpp"
  "../../include/irc/message_parser.hpp"
)

add_library(irc_lib ${sources} ${headers})
target_include_directories(irc_lib PRIVATE
  "../../include/irc"
  "../../include/basic"
)
