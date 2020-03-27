#ifndef HORT_REGEXP_HPP_
#define HORT_REGEXP_HPP_

#include <re2/re2.h>

#include "hort/vector.hpp"
#include "hort/string.hpp"

namespace hort::regexp {

namespace detail {

vector<string> findall(const re2::RE2& re, const std::string& target);

vector<string> find(const re2::RE2& re, const std::string& target);

inline bool match(const re2::RE2& re, const std::string& target) {
  re2::StringPiece input{target};
  return re2::RE2::FullMatch(input, re);
}

template <typename... Args>
inline void find(const re2::RE2& re, const std::string& target, Args&... args) {
  re2::StringPiece input{target};
  re2::RE2::FullMatch(input, re, *args...);
}

} // namespace detail

/// \brief Find all occurence of `pattern` in `target`.
[[nodiscard]] inline vector<string> findall(const std::string& pattern,
                                   const std::string& target) {
  re2::RE2 re(pattern);
  return detail::find(re, target);
}

/// \brief Find first occurence of `pattern` in `target`.
[[nodiscard]] inline vector<string> find(const std::string& pattern,
                                const std::string& target) {
  re2::RE2 re(pattern);
  return detail::findall(re, target);
}

/// \brief Check if `pattern` matches `target`.
[[nodiscard]] inline bool match(const std::string& pattern,
                                const std::string& target) {
  re2::RE2 re(pattern);
  return detail::match(re, target);
}

/// \brief Find first occurence of "pattern" in "target" and save the results
/// inside the variadic arguments.
template <typename... Args>
inline void find(const std::string& pattern,
                 const std::string& target,
                 Args&... args) {
  re2::RE2 re{pattern};
  detail::find(re, target, args...);
}

/// \brief Replace first occurence of `re` in `str` with `rewrite`.
void replace(const std::string& pattern,
             const std::string& rewrite,
             std::string& str);

/// \brief Replace all occurences of `re` in `str` with `rewrite`.
void replaceall(const std::string& pattern,
                const std::string& rewrite,
                std::string& str);

class regex {

  re2::RE2 r;

  const std::string pattern;

  static const re2::RE2::Options& build_options() {
    static re2::RE2::Options options{};
    options.set_dot_nl(true);
    return options;
  }

  static const re2::RE2::Options& create_options() {
    static const auto options = build_options();
    return options;
  }

public:
  explicit regex(const std::string& pattern_)
      : r{pattern_, create_options()}
      , pattern{pattern_} {}

  explicit regex(const regex& other)
      : r{other.pattern, create_options()}
      , pattern{other.pattern} {}

  /// \brief Find all occurence of `pattern` in `target`.
  [[nodiscard]] vector<string> findall(const std::string& target) const {
    return detail::findall(r, target);
  }

  /// \brief Find first occurence of `pattern` in `target`.
  [[nodiscard]] vector<string> find(const std::string& target) const {
    return detail::find(r, target);
  }

  /// \brief Check if `pattern` matches `target`.
  [[nodiscard]] bool match(const std::string& target) const {
    return detail::match(r, target);
  }

  /// \brief Find first occurence of `pattern` in `target`.
  template <typename... Args>
  void find(const std::string& target, Args&... args) const {
    detail::find(r, target, args...);
  }
};

} // namespace hort::regexp

#endif /* HORT_REGEXP_HPP_ */
