#ifndef COLOR_H
#define COLOR_H

#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

constexpr std::uint8_t
  hex_char_conversion_map[256]{0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0,
                               0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0,
                               0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4, 5, 6, 7, 8,
                               9, 0, 0,  0,  0,  0,  0,  0,  10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0,
                               0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0,
                               0, 0, 10, 11, 12, 13, 14, 15, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0,
                               0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0,
                               0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0,
                               0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0,
                               0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0,
                               0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0,
                               0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0,
                               0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0,
                               0, 0, 0,  0,  0,  0,  0,  0};

constexpr std::uint8_t
hex_char_to_value(char c) {
  return hex_char_conversion_map[(unsigned)c];
}

class color
{
public:
  std::uint8_t rgb[3]{0, 0, 0};

  constexpr color() = default;

  constexpr color(const char* const s) {
    assign(s);
  }


  inline color(const std::string& s) {
    assign(s);
  }

  constexpr color(std::uint8_t r, std::uint8_t g, std::uint8_t b)
    : rgb{r, g, b} {
  }

  constexpr color(const color& other)
    : rgb{other.rgb[0], other.rgb[1], other.rgb[2]} {
  }

  inline void
  assign(const std::string& s) {
    assign(s.c_str());
  }

  constexpr void
  assign(const char* s) {
    // assert(s != nullptr);

    if(s[0] != '#') {
      throw std::runtime_error{"color string doesn't start with '#'"};
    }

    // get lenght and check if the string contains invalid characters
    std::size_t i{1};
    for(; s[i] != '\0'; ++i) {
      // check format [0-9a-zA-Z]
      if((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'A' && s[i] <= 'F') ||
         (s[i] >= 'a' && s[i] <= 'f')) {
        // valid character
      }
      else {
        throw std::runtime_error{"invalid character in color string (may only contain 0-9a-zA-Z)"};
      }
    }
    // i = length

    if(i == 7) { // #rrggbb
      rgb[0] = (hex_char_to_value(s[1]) << 4) + hex_char_to_value(s[2]);
      rgb[1] = (hex_char_to_value(s[3]) << 4) + hex_char_to_value(s[4]);
      rgb[2] = (hex_char_to_value(s[5]) << 4) + hex_char_to_value(s[6]);
    }
    else if(i == 4) { // #rgb
      rgb[0] = (hex_char_to_value(s[1]) << 4) + hex_char_to_value(s[1]);
      rgb[1] = (hex_char_to_value(s[2]) << 4) + hex_char_to_value(s[2]);
      rgb[2] = (hex_char_to_value(s[3]) << 4) + hex_char_to_value(s[3]);
    }
    else {
      throw std::runtime_error{"color string is not 3 or 6 hex values"};
    }
  }

  constexpr void
  assign(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
  }
};

inline std::ostream&
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

inline void
set_foreground_color(std::ostream& os, color c) {
  os << "\x1b[38;2;" << (unsigned)c.rgb[0] << ';' << (unsigned)c.rgb[1] << ';' << (unsigned)c.rgb[2]
     << 'm';
}
inline void
set_background_color(std::ostream& os, color c) {
  os << "\x1b[48;2;" << (unsigned)c.rgb[0] << ';' << (unsigned)c.rgb[1] << ';' << (unsigned)c.rgb[2]
     << 'm';
}

inline void
reset_color(std::ostream& os) {
  os << "\x1b[0m";
}

#endif
