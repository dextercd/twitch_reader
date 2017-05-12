#include <iostream>

#include <irc/message.hpp>
#include <irc/message_parser.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(general) {
  const char message[]{"@vendor/a-key=364;tag2=hello :nickname!user@host PRIVMSG space seperated wow :trailing part that can include spaces!*))\r\n"};
  irc::message m{irc::irc_message(std::begin(message), std::end(message))};
  
  std::cout << m.command << '\n';
  BOOST_TEST(m.command == irc::command_type::PRIVMSG);
  BOOST_TEST(m.tags["vendor/a-key"] == "364");
  BOOST_TEST(m.tags["tag2"] == "hello");
  BOOST_TEST(m.prefix.nick == "nickname");
  BOOST_TEST(m.prefix.user == "user");
  BOOST_TEST(m.prefix.host == "host");
  BOOST_TEST(m.parameters.size() == 4);
}
