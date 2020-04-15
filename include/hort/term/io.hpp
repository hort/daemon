#ifndef HORT_TERM_IO_HPP_
#define HORT_TERM_IO_HPP_

#include <fmt/format.h>

#include <string>

namespace hort::term::io {

enum struct color : int {
  normal,
  black,
  red,
  green,
  yellow,
  blue,
  magenta,
  cyan,
  light_gray,
  dark_gray,
  light_red,
  light_green,
  light_yellow,
  light_blue,
  light_magenta,
  light_cyan,
  white,
};

enum struct style : int {
  normal,
  bold,
  faint,
  italic,
  underline,
};

namespace detail {

constexpr const char* ascii_color[] = {
    "39",
    "30",
    "31",
    "32",
    "33",
    "34",
    "35",
    "36",
    "37",
    "90",
    "91",
    "92",
    "93",
    "94",
    "95",
    "96",
    "97",
};


constexpr const char* ascii_style[] = {
   "0",
   "1",
   "2",
   "3",
   "4",
};

} // namespace detail

struct text {
  std::string str;
  color c;
  style s;
};

inline text operator|(const std::string &str, color c) { return text{str, c, style::normal}; }
inline text operator|(const std::string &str, style s) { return text{str, color::normal, s};  }
inline text &operator|(text &t, color color) { t.c = color; return t; }
inline text &operator|(text &t, style style) { t.s = style; return t; }

} // namespace hort::term::io

template <>
struct fmt::formatter<hort::term::io::text> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const hort::term::io::text& t, FormatContext& ctx) {
    return format_to(ctx.out(), "\033[{};{}m{}\033[0m",
                     hort::term::io::detail::ascii_style[static_cast<int>(t.s)],
                     hort::term::io::detail::ascii_color[static_cast<int>(t.c)],
                     t.str);
  }
};

#endif // HORT_TERM_IO_HPP_
