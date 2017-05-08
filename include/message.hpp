#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "binary_find.hpp"

namespace irc {

class message_parser;

struct prefix_t {
  std::string nick;
  std::string user;
  std::string host;
};

enum class command_type {
#define COMMAND_ITEM(x) x,
#define COMMAND_NUMBER_ITEM(x) NC##x,
#include "command_type_list.hpp"
};


extern const char* command_names[203];

#if 0 // for C++17
inline const char* command_names[]{
#define COMMAND_ITEM(x) #x,
#define COMMAND_NUMBER_ITEM(x) #x,
#include "command_type_list.hpp"
};
#endif

std::ostream& operator<<(std::ostream& os, command_type c);

command_type text_to_command(const char* text);
command_type text_to_command(const std::string& text);

class message;
std::ostream& operator<<(std::ostream&, const message&);

class message {
  friend class message_parser;
  friend std::ostream& operator<<(std::ostream&, const message&);

public:
  std::vector<std::string> parameters;
  std::unordered_map<std::string, std::string> tags;

  command_type command;
  prefix_t prefix;

  bool has_prefix() const noexcept;

  bool has_prefix_user() const noexcept;

  bool has_prefix_host() const noexcept;
};

} // irc::

#endif // header guard
