#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include <cassert>
#include <cstring>
#include <string>
#include <utility>

#include "message.hpp"

namespace irc {

bool is_numeric(const char c);

bool is_alpha(const char c);

bool is_alpha_numeric(const char c);

class message_parser {
public:
  message parse_message(const char* beg, const char* ed);

private:
  void parse_tags();

  void parse_tag();

  std::string get_key();

  std::string get_value();

  void parse_prefix();

  void parse_command();

  void parse_parameters();

  void parse_space();

  message* current_message;
  const char* it;
  const char* end;
};

message irc_message(const char* beg, const char* end);
std::string unescape(std::string val);
std::string escape(std::string val);

} // irc::

#endif
