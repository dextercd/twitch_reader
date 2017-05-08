#include "twitch_message.hpp"

void twitch::message::destruct()
{
  switch(type) {
    case message_type::none: {
      // do nothing
    } break;

    case message_type::chat: {
      mchat.~chat_message();
    } break;

    case message_type::subscription: {
      msubscription.~subscription();
    } break;

    default: {
      std::cerr << "UNHANDLED CASE IN message::destruct\n";
    } break;
  }
}

twitch::message::message(const message& other)
  : type{other.type}
{
  switch(type) {
    case message_type::none: {
      // do nothing
    } break;

    case message_type::chat: {
      new(&mchat) chat_message{other.mchat};
    } break;

    case message_type::subscription: {
      new(&msubscription) subscription{other.msubscription};
    } break;

    default: {
      std::cerr << "UNHANDLED CASE IN message::message(const message& other)\n";
    } break;
  }
}

twitch::message::message(message&& other)
  : type{other.type}
{
  switch(type) {
    case message_type::none: {
      // do nothing
    } break;

    case message_type::chat: {
      new(&mchat) chat_message{std::move(other.mchat)};
    } break;

    case message_type::subscription: {
      new(&msubscription) subscription{std::move(other.msubscription)};
    } break;

    default: {
      std::cerr << "UNHANDLED CASE IN message::message(const message& other)\n";
    } break;
  }
}

twitch::message_type twitch::message::get_type()
{
  return type;
}

twitch::message::~message()
{
  destruct();
}

twitch::message twitch::twitch_message(const irc::message& m)
{
  if(m.command == irc::command_type::PRIVMSG) {
    if(m.prefix.nick == "twitchnotify") { // Old subscription api
      return message{message_type::none};
    }

    return message{message_type::chat, m};
  }

  if(m.command == irc::command_type::USERNOTICE) {
    return message{message_type::subscription, m};
  }

  return message{message_type::none};
}
