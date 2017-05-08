#ifndef CONVERT_H
#define CONVERT_H

#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <limits>

enum class int_convert_res { success, overflow, underflow, bad_string };

bool convert_success(int_convert_res r)
{
  return r == int_convert_res::success;
}

int_convert_res string_convert(const char* str, int& var, int base = 10)
{
  errno = 0;
  char* end;
  long v = std::strtol(str, &end, base);

  if(errno == ERANGE) {
    if(v == std::numeric_limits<long>::max()) {
      return int_convert_res::overflow;
    } else {
      assert(v == std::numeric_limits<long>::min());
      return int_convert_res::underflow;
    }
  }

  if(v > std::numeric_limits<int>::max()) {
    return int_convert_res::overflow;
  }
  if(v < std::numeric_limits<int>::min()) {
    return int_convert_res::underflow;
  }

  if(*str == '\0' || *end != '\0') {
    return int_convert_res::bad_string;
  }

  var = v;
  return int_convert_res::success;
}

int_convert_res string_convert(const char* str, long& var, int base = 10)
{
  errno = 0;
  char* end;
  long v = std::strtol(str, &end, base);

  if(errno == ERANGE) {
    if(v == std::numeric_limits<long>::max()) {
      return int_convert_res::overflow;
    } else {
      assert(v == std::numeric_limits<long>::min());
      return int_convert_res::underflow;
    }
  }

  if(*str == '\0' || *end != '\0') {
    return int_convert_res::bad_string;
  }

  var = v;
  return int_convert_res::success;
}

int_convert_res string_convert(const char* str, unsigned& var, int base = 10)
{
  errno = 0;
  char* end;
  unsigned long v = std::strtoul(str, &end, base);

  if(errno == ERANGE) {
    assert(v == std::numeric_limits<unsigned long>::max());
    return int_convert_res::overflow;
  }

  if(v > std::numeric_limits<int>::max()) {
    return int_convert_res::overflow;
  }

  if(*str == '\0' || *end != '\0') {
    return int_convert_res::bad_string;
  }

  var = v;
  return int_convert_res::success;
}

int_convert_res string_convert(const char* str, unsigned long int& var, int base = 10)
{
  errno = 0;
  char* end;
  unsigned long v = std::strtoul(str, &end, base);

  if(errno == ERANGE) {
    assert(v == std::numeric_limits<unsigned long>::max());
    return int_convert_res::overflow;
  }

  if(*str == '\0' || *end != '\0') {
    return int_convert_res::bad_string;
  }

  var = v;
  return int_convert_res::success;
}

/*
 * TODO: add long long and unsigned long long
 */

#endif
