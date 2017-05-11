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
  std::string value = get_value();

  current_message->tags.insert(std::make_pair(std::move(key), std::move(value)));
}

std::string irc::message_parser::get_key()
{
  const char* start = it;
  while(is_alpha_numeric(*it) || *it == '-') {
    ++it;
  }

  if(*it == '/') { // previous part is vendor
    ++it;
    while(is_alpha_numeric(*it) || *it == '-') {
      ++it;
    }
  }

  return std::string{start, it};
}

std::string irc::message_parser::get_value()
{


  std::string escaped_value;

  auto begin = it;

  // find end
  for(; *it != ' ' && *it != ';'; ++it)
    ;

  // it is end

  return unescape(std::string{begin, it});
}

constexpr static std::pair<char, char> escaped_rep[]{{':', ';'},
                                                     {'s', ' '},
                                                     {'\\', '\\'},
                                                     {'r', '\r'},
                                                     {'n', '\n'}};

std::string irc::unescape(const std::string& val)
{
  std::string ret;

  for(auto it{val.begin()}; it != val.end(); ++it) {
    if(*it == '\\') {
      ++it;
      for(const auto& rep : escaped_rep) {
        if(*it == rep.first) {
          ret += rep.second;
          break;
        }
      }
    } else {
      ret += *it;
    }
  }

  return ret;
}

std::string irc::escape(const std::string& val)
{
  std::string ret;

  for(auto it{val.begin()}; it != val.end(); ++it) {
    // check if *it should be escaped
    for(const auto& rep : escaped_rep) {
      // escape
      if(*it == rep.second) {
        ret += rep.first;
      }

      goto next;
    }

    // no escape
    ret += *it;

  next:;
  }

  return ret;
}


// parses:   servername / ( nickname [ [ "!" user ] "@" host ] ) (theres a space bar after this
// part)
void irc::message_parser::parse_prefix()
{
  const char* prefix_element_start = it;

  // nickname
  while(!std::strchr("!@ ", *it)) { // find end of <nickname>
    ++it;
  }
  current_message->prefix.nick.assign(prefix_element_start, it);

  // user
  if(*it != '!') { // we don't have !<user>, so we should have @<host>
    goto host;
  }

  prefix_element_start = ++it;
  while(!std::strchr("@ ", *it)) { // find end of <user>
    ++it;
  }
  current_message->prefix.user.assign(prefix_element_start, it);

// host
host:
  if(*it != '@') {
    goto end_prefix;
  }

  prefix_element_start = ++it;
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
