#include "twitch_chat_reader.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <utility>

#include "message_parser.hpp"

using namespace std::placeholders;
using namespace std::literals;

using asio::ip::tcp;

client::client(asio::io_service& service, const std::string& user, const std::string& password, const std::string& channel)
  : io_service{&service}
  , socket{*io_service}
{
  tcp::resolver resolver(*io_service);
  tcp::resolver::query query("irc.chat.twitch.tv", "6667");

  auto con = asio::connect(socket, resolver.resolve(query));

  if(con != tcp::resolver::iterator{}) {
    std::cout << "Connected to " << con->host_name() << ':' << con->service_name() << ".\n";
    login(channel, user, password);
  }
}

// give login info
void client::login(const std::string& user, const std::string& password, const std::string& channel)
{
  asio::error_code error;
  asio::write(socket,
              asio::buffer("PASS "s + password + "\r\nNICK " + user + "\r\nJOIN #"
                           + channel
                           + "\r\nCAP REQ :twitch.tv/membership\r\nCAP REQ "
                             ":twitch.tv/commands\r\nCAP REQ "
                             ":twitch.tv/tags\r\n"),
              error);
  if(error) {
    std::cout << "Error when sending login info: " << error.message() << ".\n";
  } else {
    std::cout << "Successfully send login info.\n";
    do_read();
  }
}

void client::do_read()
{
  static auto do_read_impl = [this](const asio::error_code& ec, std::size_t byte_count) {
    if(ec) {
      return;
    }

    std::unique_ptr<char[]> message_data{new char[byte_count]};
    receive_buffer.sgetn(message_data.get(), byte_count);

    // we copied the data, so we can start the next read now.
    do_read();

    handle_message(irc::irc_message(message_data.get(), message_data.get() + byte_count));
  };

  asio::async_read_until(socket, receive_buffer, "\r\n", do_read_impl);
}

void client::handle_message(const irc::message& irc_message)
{
  std::cout << irc_message << "\n\n";

  /*
  switch(irc_message.command) {
    case irc::command_type::PING: {
      ping_response(irc_message.parameters[0]);
    } break;

    default: {
      // do nothing
    } break;
  }

  auto twm{twitch::twitch_message(irc_message)};
  switch(twm.get_type()) {

    case twitch::message_type::subscription: {
      std::cout << twm.msubscription.display_name << " subscribed, for " <<
  twm.msubscription.month_count << " months!";
      if(!twm.msubscription.message.empty()) {
        std::cout << " Message: " << twm.msubscription.message;
      }
      std::cout << '\n';
    } break;

    case twitch::message_type::chat: {
      if(twm.mchat.bits >= 50) {
        std::cout << "Thanks " << twm.mchat.display_name << " for the " << twm.mchat.bits << "
  bits." << " Message: " << twm.mchat.message << '\n';
      }
    } break;

    default: {
      // do nothing
    } break;
  }*/
}

void client::ping_response(const std::string& server)
{
  asio::write(socket, asio::buffer("PONG :" + server + "\r\n"));
}

void client::close()
{
  socket.shutdown(asio::socket_base::shutdown_type::shutdown_both);
  socket.close();
}
