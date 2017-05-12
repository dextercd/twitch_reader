#include "twitch_chat_reader.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <utility>

#include "irc/message_parser.hpp"

using namespace std::placeholders;
using namespace std::literals;

using asio::ip::tcp;

client::client(asio::io_service& service,
               const std::string& user,
               const std::string& password,
               const std::string& channel)
  : io_service{&service}
  , socket{*io_service}
{
  tcp::resolver resolver(*io_service);
  tcp::resolver::query query("irc.chat.twitch.tv", "6667");

  asio::error_code connect_error;
  auto con = asio::connect(socket, resolver.resolve(query), connect_error);

  if(connect_error) {
    std::cout << "Couldn't connect to the IRC server\n";
    return;
  }

  std::cout << "Connected to " << con->host_name() << ':' << con->service_name() << '\n';
  login(user, password, channel);
}

void client::login(const std::string& user, const std::string& password, const std::string& channel)
{
  asio::error_code error;
  asio::write(socket,
              asio::buffer("PASS "s + password + "\r\nNICK " + user + "\r\nJOIN #" + channel
                           + "\r\nCAP REQ :twitch.tv/membership\r\nCAP REQ "
                             ":twitch.tv/commands\r\nCAP REQ "
                             ":twitch.tv/tags\r\n"),
              error);

  if(error) {
    std::cout << "Error when sending login info: " << error.message() << '\n';
    return;
  }

  std::cout << "Successfully send login info.\n";
  do_read();
}

void client::do_read()
{
  asio::async_read_until(socket,
                         receive_buffer,
                         "\r\n",
                         std::bind(&client::do_read_completer, this, _1, _2));
}

void client::do_read_completer(const asio::error_code& error, std::size_t bytes_transferred)
{
  if(error) {
    return;
  }

  std::unique_ptr<char[]> message_data{new char[bytes_transferred]};

  // copy the data into message_data
  receive_buffer.sgetn(message_data.get(), bytes_transferred);

  handle_message(irc::irc_message(message_data.get(), message_data.get() + bytes_transferred));

  if(can_send()) {
    send_responses();
  } else {
    do_read();
  }
}

void client::send_responses()
{
  std::string response_data;

  std::unique_lock<std::mutex> lock{responses_mutex};
  for(auto& r : responses) {
    response_data += r;
  }

  responses.clear();
  lock.unlock();

  asio::async_write(socket,
                    asio::buffer(response_data),
                    std::bind(&client::send_responses_completer, this, _1, _2));
}

void client::send_responses_completer(const asio::error_code& error, std::size_t bytes_transferred)
{
  if(error) {
    return;
  }

  // once we've send something we go back to reading
  do_read();
}

void client::handle_message(const irc::message& irc_message)
{
  // log the message
  std::cout << irc_message << "\n\n";

  switch(irc_message.command) {
    case irc::command_type::PING: {
      ping_response(irc_message.parameters[0]);
    } break;

    default: {
      // do nothing
    } break;
  }
}

void client::ping_response(const std::string& server)
{
  std::lock_guard<std::mutex> lock{responses_mutex};
  responses.push_back("PONG :" + server + "\r\n");
}

void client::close()
{
  socket.close();
}

// TODO: track how many messages you've send to not get a timeout
bool client::can_send()
{
  std::lock_guard<std::mutex> lock{responses_mutex};
  return !responses.empty();
}
