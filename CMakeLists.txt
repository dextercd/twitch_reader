cmake_minimum_required(VERSION 3.5)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
project("chat_reader")

set (CMAKE_CXX_STANDARD 17)

add_executable("program" "src/color.cpp" "src/main.cpp" "src/message.cpp" "src/message_parser.cpp" "src/twitch_chat_reader.cpp" "src/twitch_message.cpp")

target_include_directories("program" PRIVATE "include")

find_package(Threads)
target_link_libraries("program" ${CMAKE_THREAD_LIBS_INIT})

find_package(Asio)
target_include_directories("program" PRIVATE ${ASIO_INCLUDE_DIR})
