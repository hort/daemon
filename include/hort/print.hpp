#ifndef __HORT_PRINT_HPP_
#define __HORT_PRINT_HPP_

#include <iostream>
#include <fmt/format.h>
#include <map>

#include "hort/vector.hpp"
#include "hort/string.hpp"

#include "hort/term/io.hpp"

#include "json.hpp"
using json = nlohmann::json;

namespace hort {

namespace detail {

template <typename T>
inline void print(const T& i) {
  fmt::print("{}", i);
}

inline void print(json j) {
  fmt::print("{}", j.dump(2));
}

template <typename T, typename... Args>
inline void print(const T& i, const Args&... args) {
  detail::print(i);
  putchar(' ');
  detail::print(args...);
}

template <typename T, typename U>
inline void print(const std::map<T, U>& m) {
  auto it = m.begin();
  putchar('{');
  while (it != m.end()) {
    detail::print(it->first);
    puts(": ");
    detail::print(it->second);
    if (++it != m.end()) {
      puts(", ");
    }
  }
  putchar('}');
}

template <typename T>
inline void print(const vector<T>& v) {
  if (!v.size()) {
    fmt::print("[]");
    return;
  }
  fmt::print("[");
  for (unsigned i = 0; i < v.size() - 1; i++) {
    detail::print(v[i]);
    puts(", ");
  }
  detail::print(v[v.size() - 1]);
  putchar(']');
}

} // namespace detail

template <typename... Args>
inline void print(const Args&... args) {
  detail::print(args...);
}

template <typename... Args>
inline void println(const Args&... args) {
  detail::print(args...);
  putchar('\n');
}

template <typename... Args>
inline void printf(std::string_view fmt, const Args&... args) {
  fmt::format_args argspack = fmt::make_format_args(args...);
  print(fmt::vformat(fmt, argspack));
}

template <typename... Args>
inline string sprintf(std::string_view fmt, const Args&... args) {
  fmt::format_args argspack = fmt::make_format_args(args...);
  return fmt::vformat(fmt, argspack);
}

} // namespace hort

#endif // __HORT_PRINT_HPP_
