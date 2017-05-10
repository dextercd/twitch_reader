cmake_minimum_required(VERSION 3.5)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake_modules)
project(chat_reader)

set (CMAKE_CXX_STANDARD 17)

set(sources
  "src/main.cpp"
  "src/message.cpp"
  "src/message_parser.cpp"
  "src/twitch_chat_reader.cpp"
  "src/twitch_message.cpp"
)

set(headers
  "include/binary_find.hpp"
  "include/color.hpp"
  "include/command_type_list.hpp"
  "include/convert.hpp"
  "include/message.hpp"
  "include/message_parser.hpp"
  "include/twitch_chat_reader.hpp"
  "include/twitch_message.hpp"
)

add_executable(chat_reader ${sources} ${headers})

target_compile_definitions(chat_reader PRIVATE ASIO_STANDALONE)

target_include_directories(chat_reader PRIVATE include)

find_package(Threads)
target_link_libraries(chat_reader ${CMAKE_THREAD_LIBS_INIT})

find_package(Asio)
target_include_directories(chat_reader PRIVATE ${ASIO_INCLUDE_DIR})
