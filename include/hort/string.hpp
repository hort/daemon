#ifndef HORT_STRING_HPP_
#define HORT_STRING_HPP_

#include <string>
#include <vector>

#include <fmt/format.h>

#include "hort/vector.hpp"

namespace hort {

class string : public std::string {
  static constexpr const char* ws = " \t\n\r\f\v";

public:
  using std::string::string;
  string(const std::string& str) : std::string{str} {}

  using std::string::replace;

  /// \brief Split string by `delims`.
  [[nodiscard]] vector<string> split(const string& delims = " ") const;

  /// \brief Replace every occurence of `from` with `to`.
  /// \see rreplace if you need re support.
  [[nodiscard]] string replace(const string& from, const string& to) const;

  /// \brief Regex replace every occurence of `from` with `to`.
  //[[nodiscard]] string rreplace(const string& from, const string& to) const {
  //  auto s = *this;
  //  re::replaceall(from, to, s);
  //  return s;
  //};

  template <typename... Args>
  [[nodiscard]] string format(const Args&... args) const {
    fmt::format_args argspack = fmt::make_format_args(args...);
    return fmt::vformat(*this, argspack);
  }

  [[nodiscard]] string lower() const {
    auto s = *this;
    for (auto & i : s) {
      if (i >= 'A' && i <= 'Z') i += 32;
    }
    return s;
  }

  [[nodiscard]] string upper() const {
    auto s = *this;
    for (auto & i : s) {
      if (i >= 'a' && i <= 'z') i -= 32;
    }
    return s;
  }

  [[nodiscard]] string title() const {
    auto s = *this;
    for (auto & i : s) {
      if (i >= 'a' && i <= 'z' && *std::prev(&i) == ' ') i -= 32;
    }
    return s;
  }

  [[nodiscard]] string trim(const char* t = ws) const {
    auto s = *this;
    return s.ltrim(t).rtrim(t);
  }

  [[nodiscard]] string rtrim(const char* t = ws) const {
    auto s = *this;
    s.erase(s.find_last_not_of(t) + 1);
    return s;
  }

  [[nodiscard]] string ltrim(const char* t = ws) const {
    auto s = *this;
    s.erase(0, s.find_first_not_of(t));
    return s;
  }
};

template <typename... Args>
inline string join(std::string_view separator, const Args&... args) {
  return join(separator, args...);
}

template <typename T, typename... Args>
inline string join(std::string_view separator, const T& t, const Args&... args) {
  return t + std::string(separator) + join(separator, args...);
}

template <typename T>
inline string join(std::string_view, const T& t) {
  return t;
}

} // namespace hort

#endif // HORT_STRING_HPP_
