#include "message_parser.hpp"

bool irc::is_numeric(const char c)
{
  return (c >= '0' && c <= '9');
}
bool irc::is_alpha(const char c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool irc::is_alpha_numeric(const char c)
{
  return is_alpha(c) || is_numeric(c);
}

irc::message irc::message_parser::parse_message(const char* beg, const char* ed)
{
  message new_message;
  current_message = &new_message;

  it = beg;
  end = ed;

  if(*it == '@') {
    ++it;
    parse_tags();
    parse_space();
  }

  if(*it == ':') {
    ++it;
    parse_prefix();
    parse_space();
  }

  parse_command();
  parse_parameters();

  assert(it[0] == '\r' && it[1] == '\n'); // the message should end in \r\n

  return new_message;
}

void irc::message_parser::parse_tags()
{
more_tag:
  parse_tag();
  if(*it == ';') {
    ++it;
    goto more_tag;
  }
}

void irc::message_parser::parse_tag()
{
  std::string key = get_key();
  assert(*it == '='); // after <key> there should be an '='
  ++it;
  std::string value = get_escaped_value();

  current_message->tags.insert(std::make_pair(std::move(key), std::move(value)));
}

std::string irc::message_parser::get_key()
{
  const char* start = it;
  while(is_alpha_numeric(*it) || *it == '-') {
    ++it;
  }

  if(*it == '/') { // previous part is vendor
    while(is_alpha_numeric(*it) || *it == '-') {
      ++it;
    }
  }

  return std::string{start, it};
}

std::string irc::message_parser::get_escaped_value()
{
  constexpr static std::pair<char, char> escaped_rep[]{{':', ';'},
                                                       {'s', ' '},
                                                       {'\\', '\\'},
                                                       {'r', '\r'},
                                                       {'n', '\n'}};

  std::string escaped_value;

  for(; *it != ' ' && *it != ';'; ++it) {
    if(*it == '\\') { // escaped value
      ++it;
      for(auto& esc : escaped_rep) {
        if(*it == esc.first) {
          escaped_value += esc.second;
          break;
        }
        // if we can't find a escaped value, just ignore it
      }
    } else {
      escaped_value += *it;
    }
  }

  return escaped_value;
}

void irc::message_parser::parse_prefix()
{
  const char* prefix_element_start = it;

  while(!std::strchr("! ", *it)) { // find end of <nick>
    ++it;
  }
  current_message->prefix.nick.assign(prefix_element_start, it);

  if(*it != '!') { // we don't have !<user>
    goto end_prefix;
  }

  ++it;
  prefix_element_start = it;
  while(!std::strchr("@ ", *it)) { // find end of <user>
    ++it;
  }
  current_message->prefix.user.assign(prefix_element_start, it);

  if(*it != '@') { // we don't have @<host>
    goto end_prefix;
  }

  ++it;
  prefix_element_start = it;
  while(*it != ' ') { // find end of <host>
    ++it;
  }
  current_message->prefix.host.assign(prefix_element_start, it);

end_prefix:; // done
}

void irc::message_parser::parse_command()
{
  const char* command_begin = it;

  if(is_alpha_numeric(*it)) {
    ++it;
    while(is_alpha_numeric(*it)) {
      ++it;
    }
  }

  current_message->command = text_to_command(std::string{command_begin, it});
}

void irc::message_parser::parse_parameters()
{
  do {
    // before each parameter there is one space
    parse_space();

    const char* parameter_begin;
    const char* parameter_end;

    // trailing parameter
    if(*it == ':') {
      parameter_begin = ++it;

      while(true) {
        if(*it == '\r') {
          break;
        }
        ++it;
      }

      parameter_end = it;
    } else {
      // middle parameter
      parameter_begin = it;
      while(*it != ' ' && *it != '\r') {
        ++it;
      }
      parameter_end = it;
    }

    current_message->parameters.emplace_back(parameter_begin, parameter_end);
  } while(*it != '\r');
}

void irc::message_parser::parse_space()
{
  if(*it != ' ') { // there needs to be one space
    throw std::runtime_error{"Missing space"};
  }

  ++it;
}

irc::message irc::irc_message(const char* beg, const char* end)
{
  return message_parser{}.parse_message(beg, end);
}
