#ifndef HORT_STRING_HPP_
#define HORT_STRING_HPP_

#include <string>
#include <vector>

#include "hort/vector.hpp"

namespace hort {

class string : public std::string {
  static constexpr const char* ws = " \t\n\r\f\v";

public:
  string() : std::string{""} {}
  string(const char* str) : std::string{str} {}
  string(const std::string& str) : std::string{str} {}

  using std::string::replace;

  /// \brief Split string by `delims`.
  vector<string> split(const string& delims = " ") const;

  /// \brief Replace every occurence of `from` with `to`.
  ///
  /// Use hort::regexp::replace if you need regexp support.
  string replace(const string& from, const string& to) const;

  string trim(const char* t = ws) const {
    auto s = *this;
    return s.ltrim(t).rtrim(t);
  }

  string rtrim(const char* t = ws) const {
    auto s = *this;
    s.erase(s.find_last_not_of(t) + 1);
    return s;
  }

  string ltrim(const char* t = ws) const {
    auto s = *this;
    s.erase(0, s.find_first_not_of(t));
    return s;
  }
};

} // namespace hort

#endif // STRING_HPP_
