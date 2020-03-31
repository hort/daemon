#ifndef HORT_PRINT_HPP_
#define HORT_PRINT_HPP_

#include <fmt/format.h>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include "json.hpp"
using json = nlohmann::json;

namespace hort {

namespace detail {

template <typename T>
inline void print(const T& i) {
  fmt::print("{}", i);
}

template <typename T>
inline void print(const std::string& str) {
  fmt::print("\"{}\"", str);
}

template <typename T>
inline void print(json j) {
  fmt::print("{}", j.dump(2));
}

template <typename T, typename... Args>
inline void print(const T& i, const Args&... args) {
  detail::print(i);
  fmt::print(" ");
  detail::print(args...);
}

template <typename T, typename U>
inline void print(const std::map<T, U>& m) {
  auto it = m.begin();
  fmt::print("{");
  while (it != m.end()) {
    detail::print(it->first);
    fmt::print(": ");
    detail::print(it->second);
    if (++it != m.end()) {
      fmt::print(", ");
    }
  }
  fmt::print("}");
}

template <typename T>
inline void print(const std::vector<T>& v) {
  if (!v.size()) {
    fmt::print("[");
    return;
  }
  fmt::print("[");
  for (unsigned i = 0; i < v.size() - 1; i++) {
    detail::print(v[i]);
    fmt::print(", ");
  }
  detail::print(v[v.size() - 1]);
  fmt::print("]");
}

struct formatter {
  std::string_view format;

  constexpr formatter(std::string_view format_) : format{format_} {}

  template <typename... Args>
  auto operator()(const Args&... args) {
    fmt::format_args argspack = fmt::make_format_args(args...);
    return fmt::vformat(format, argspack);
  }
};

} // namespace detail}

template <typename... Args>
inline void print(const Args&... args) {
  detail::print(args...);
  fmt::print("\n");
}

constexpr auto operator"" _format(const char* format, std::size_t) {
  return detail::formatter{format};
}

} // namespace hort

#endif // HORT_PRINT_HPP_
