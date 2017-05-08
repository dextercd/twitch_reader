#include "message.hpp"

std::ostream& irc::operator<<(std::ostream& os, irc::command_type c)
{
  std::size_t index = static_cast<std::size_t>(c);
  if(index > std::size(command_names)) {
    index = 0;
  }

  return os << command_names[index];
}

irc::command_type irc::text_to_command(const char* text)
{
  auto n = binary_find(std::begin(command_names),
                       std::end(command_names),
                       text,
                       [](const char* f, const char* t) { return std::strcmp(f, t) < 0; });

  if(n == std::end(command_names)) {
    return command_type::unknown;
  }

  // Cast the index to the command_type enum
  return static_cast<command_type>(n - std::begin(command_names));
}

irc::command_type irc::text_to_command(const std::string& text)
{
  return text_to_command(text.c_str());
}

std::ostream& irc::operator<<(std::ostream& os, const irc::message& m)
{
  os << "Command: " << m.command << '\n';
  if(m.has_prefix()) {
    os << "Prefix nick: " << m.prefix.nick << '\n';
  }
  if(m.has_prefix_user()) {
    os << "Prefix user: " << m.prefix.user << '\n';
  }
  if(m.has_prefix_host()) {
    os << "Prefix host: " << m.prefix.host << '\n';
  }

  os << '{';
  for(const auto& p : m.parameters) {
    os << p << ", ";
  }
  os << "}\n";

  for(const auto& tag : m.tags) {
    os << "[ " << tag.first << " = " << tag.second << " ]\n";
  }

  return os;
}

bool irc::message::has_prefix() const noexcept
{
  return !prefix.nick.empty();
}
bool irc::message::has_prefix_user() const noexcept
{
  return !prefix.user.empty();
}
bool irc::message::has_prefix_host() const noexcept
{
  return !prefix.host.empty();
}
