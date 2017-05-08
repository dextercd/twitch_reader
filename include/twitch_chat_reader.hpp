#ifndef TWITCH_CHAT_READER
#define TWITCH_CHAT_READER

#include <string>

#include <asio.hpp>

#include "twitch_message.hpp"

class client {
public:
  client(asio::io_service& service, std::string user, std::string pass, std::string channel);

  void login();

  void do_read();

  void handle_message(const irc::message& twm);

  void ping_response(const std::string& server);

  void close();

  asio::io_service* io_service;
  asio::ip::tcp::socket socket;

  asio::streambuf receive_buffer;

  std::string username;
  std::string password;
  std::string current_channel;
};

#endif
