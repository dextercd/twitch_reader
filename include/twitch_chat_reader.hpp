#ifndef TWITCH_CHAT_READER
#define TWITCH_CHAT_READER

#include <mutex>
#include <string>
#include <vector>

#include <asio.hpp>

#include "twitch/twitch_message.hpp"

class client {
public:
  client(asio::io_service& service,
         const std::string& user,
         const std::string& password,
         const std::string& channel);

  void login(const std::string& user, const std::string& password, const std::string& channel);

  void do_read();
  void do_read_completer(const asio::error_code& error, std::size_t bytes_transferred);

  void send_responses();
  void send_responses_completer(const asio::error_code& error, std::size_t bytes_transferred);

  void handle_message(const irc::message& twm);

  void ping_response(const std::string& server);

  bool can_send();

  void close();

  asio::io_service* io_service;
  asio::ip::tcp::socket socket;

  asio::streambuf receive_buffer;

  std::mutex responses_mutex;
  std::vector<std::string> responses;
};

#endif
