#ifndef TWITCH_CHAT_READER
#define TWITCH_CHAT_READER

#include <string>

#include <asio.hpp>

#include "twitch_message.hpp"

class client {
public:
  client(asio::io_service& service, const std::string& user, const std::string& password, const std::string& channel);

  void login(const std::string& user, const std::string& password, const std::string& channel);

  void do_read();

  void handle_message(const irc::message& twm);

  void ping_response(const std::string& server);

  void close();

  asio::io_service* io_service;
  asio::ip::tcp::socket socket;

  asio::streambuf receive_buffer;
};

#endif
