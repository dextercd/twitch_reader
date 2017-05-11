cmake_minimum_required(VERSION 3.5)

project(chat_reader)

set (CMAKE_CXX_STANDARD 17)


add_executable(irc_test
  "irc_test.cpp"
  "../src/message.cpp"
  "../src/message_parser.cpp"
  "../include/message.hpp"
  "../include/message_parser.hpp"
)

target_include_directories(irc_test PRIVATE "../include")

add_test(irc irc_test)

find_package(Threads)
find_package(Asio)
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
target_link_libraries(irc_test ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(irc_test PRIVATE ${ASIO_INCLUDE_DIR})
target_link_libraries(irc_test
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
