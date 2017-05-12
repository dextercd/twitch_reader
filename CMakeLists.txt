cmake_minimum_required(VERSION 3.5)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake_modules)

set (CMAKE_CXX_STANDARD 17)

enable_testing()

project(chat_reader)

add_executable(chat_reader
  "src/main.cpp"
  "src/twitch_chat_reader.cpp"
  "include/twitch_chat_reader.hpp"
)

add_subdirectory(libs/twitch)
target_link_libraries(chat_reader
  twitch_lib
)

target_compile_definitions(chat_reader PRIVATE ASIO_STANDALONE)

target_include_directories(chat_reader PRIVATE
  "include"
  "include/basic"
)

find_package(Threads)
target_link_libraries(chat_reader ${CMAKE_THREAD_LIBS_INIT})

find_package(Asio)
target_include_directories(chat_reader PRIVATE ${ASIO_INCLUDE_DIR})
