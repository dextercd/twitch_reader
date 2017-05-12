cmake_minimum_required(VERSION 3.5)

project(chat_reader)

set (CMAKE_CXX_STANDARD 17)


add_executable(irc_test
  "irc_test.cpp"
)

target_link_libraries(irc_test irc_lib)

find_package(Threads)

find_package(Boost COMPONENTS
  system
  filesystem
  unit_test_framework

  REQUIRED
)

target_link_libraries(irc_test
  ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(irc_test
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_test(irc irc_test)
