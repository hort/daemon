#ifndef HORT_PRINT_HPP_
#define HORT_PRINT_HPP_

#include <fmt/format.h>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include "hort/vector.hpp"
#include "hort/string.hpp"

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

inline void print(const vector<string>& v) {
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

} // namespace detail

template <typename... Args>
inline void print(const Args&... args) {
  detail::print(args...);
  fmt::print("\n");
}

} // namespace hort

#endif // HORT_PRINT_HPP_
