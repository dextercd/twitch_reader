#ifndef BINARY_FIND
#define BINARY_FIND

#include <algorithm>
#include <functional>

template <class Iter, class T, typename L>
Iter binary_find(Iter begin, Iter end, T val, L&& comp)
{
  // Finds the lower bound in at most log(last - first) + 1 comparisons
  Iter i = std::lower_bound(begin, end, val, comp);

  if(i != end && !comp(val, *i)) {
    return i; // found
  } else {
    return end; // not found
  }
}

#endif
