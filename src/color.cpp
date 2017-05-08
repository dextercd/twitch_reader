#include "color.hpp"

std::ostream&
operator<<(std::ostream& os, const color c) {
  auto rf = os.flags();

  os << '#';

  os << std::hex;
  os.fill('0');

  os.width(2);
  os << (unsigned int)c.rgb[0];

  os.width(2);
  os << (unsigned int)c.rgb[1];

  os.width(2);
  os << (unsigned int)c.rgb[2];

  os.flags(rf);
  return os;
}

void
set_foreground_color(std::ostream& os, color c) {
  os << "\x1b[38;2;" << (unsigned)c.rgb[0] << ';' << (unsigned)c.rgb[1] << ';' << (unsigned)c.rgb[2]
     << 'm';
}
void
set_background_color(std::ostream& os, color c) {
  os << "\x1b[48;2;" << (unsigned)c.rgb[0] << ';' << (unsigned)c.rgb[1] << ';' << (unsigned)c.rgb[2]
     << 'm';
}

void
reset_color(std::ostream& os) {
  os << "\x1b[0m";
}

void
color::assign(const std::string& s) {
  assign(s.c_str());
}
