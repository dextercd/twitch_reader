#ifndef TWITCH_MESSAGE_HPP
#define TWITCH_MESSAGE_HPP

#include <cassert>
#include <iostream>
#include <string>
using namespace std::literals;

#include "color.hpp"
#include "message.hpp"

namespace twitch {

namespace defaults {
constexpr color user_color{"#000"};
}

template <typename T>
void set_name_g(T& message, const irc::message& irc)
{
  // user prefix is the user name
  message.user_name = irc.prefix.user;

  // display-name: The user’s display name. This is empty if it is never set.
  auto display_name_it{irc.tags.find("display-name")};
  assert(display_name_it != irc.tags.end());

  if(display_name_it->second.empty()) {
    message.display_name = message.user_name;
  } else {
    message.display_name = display_name_it->second;
  }
}

template <typename T>
void set_name_color_g(T& message, const irc::message& irc)
{
  // color: Hexadecimal RGB color code. This is empty if it is never set.
  auto color_it{irc.tags.find("color")};
  assert(color_it != irc.tags.end());

  if(color_it->second.empty()) {
    message.name_color = defaults::user_color;
  } else {
    message.name_color = color_it->second;
  }
}

struct chat_message {
  chat_message(const irc::message& irc)
  {
    set_name_color(irc);
    set_name(irc);
    set_message(irc);
    set_bits(irc);
  }

  chat_message() = default;

  friend void set_name_color_g<chat_message>(chat_message&, const irc::message&);
  void set_name_color(const irc::message& irc)
  {
    set_name_color_g(*this, irc);
  }

  friend void set_name_g<chat_message>(chat_message&, const irc::message&);
  void set_name(const irc::message& irc)
  {
    twitch::set_name_g(*this, irc);
  }

  void set_message(const irc::message& irc)
  {
    assert(irc.parameters.size() == 2);
    message = irc.parameters[1];
  }

  void set_bits(const irc::message& irc)
  {
    auto bits_it{irc.tags.find("bits")};
    if(bits_it == irc.tags.end()) {
      bits = 0;
    } else {
      bits = std::atoi(bits_it->second.c_str());
    }
  }

  int bits;
  color name_color;
  std::string display_name;
  std::string user_name;

  std::string message;
};

struct subscription {
  subscription(const irc::message& irc)
  {
    set_name_color(irc);
    set_name(irc);
    set_message(irc);
    set_month_count(irc);
  }

  subscription() = default;

  friend void set_name_color_g<subscription>(subscription&, const irc::message&);
  void set_name_color(const irc::message& irc)
  {
    set_name_color_g(*this, irc);
  }

  void set_name(const irc::message& irc)
  {
    auto name_it{irc.tags.find("login")};
    assert(name_it != irc.tags.end());

    user_name = name_it->second;

    auto display_name_it{irc.tags.find("display-name")};
    assert(display_name_it != irc.tags.end());

    if(display_name_it->second.empty()) {
      display_name = user_name;
    } else {
      display_name = display_name_it->second;
    }
  }

  void set_message(const irc::message& irc)
  {
    if(irc.parameters.size() == 2) {
      message = irc.parameters[1];
    }
  }

  void set_month_count(const irc::message& irc)
  {
    auto month_count_it{irc.tags.find("msg-param-months")};
    assert(month_count_it != irc.tags.end());

    month_count = std::atoi(month_count_it->second.c_str());
  }

  color name_color;

  int month_count;
  std::string display_name;
  std::string user_name;

  std::string message;
};


enum class message_type { none, chat, subscription };

class message {
  template <typename... Args>
  void construct(message_type t, Args&&... args)
  {
    switch(t) {
      case message_type::none: {
        // do nothing
      } break;

      case message_type::chat: {
        new(&mchat) chat_message{std::forward<Args>(args)...};
      } break;

      case message_type::subscription: {
        new(&msubscription) subscription{std::forward<Args>(args)...};
      } break;

      default: {
        std::cerr << "UNHANDLED CASE IN message::construct\n";
      } break;
    }

    type = t;
  }

  void destruct();

  // all different messages
  message_type type = message_type::none;

public:
  template <typename... Args>
  explicit message(message_type t, Args&&... args)
  {
    construct(t, std::forward<Args>(args)...);
  }

  message(const message& other);
  message(message&& other);

  template <typename... Args>
  void set_type(message_type t, Args&&... args)
  {
    destruct();
    construct(t, std::forward<Args>(args)...);
  }

  message_type get_type();

  union {
    chat_message mchat;
    subscription msubscription;
  };

  ~message();
};

message twitch_message(const irc::message& m);
}

#endif
