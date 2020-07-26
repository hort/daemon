#ifndef HORT_TERM_IO_HPP_
#define HORT_TERM_IO_HPP_

#include "hort/string.hpp"

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

static const char* ascii_color[] = {
  "39", // normal
  "30", // black
  "31", // red
  "32", // green
  "33", // yellow
  "34", // blue
  "35", // magenta
  "36", // cyan
  "37", // light_gray
  "90", // dark_gray
  "91", // light_red
  "92", // light_green
  "93", // light_yellow
  "94", // light_blue
  "95", // light_magenta
  "96", // light_cyan
  "97", // white
};

static const char* ascii_style[] = {
  "0", // normal
  "1", // bold
  "2", // faint
  "3", // italic
  "4", // underline
};

} // namespace detail

struct text {
  string data;
  color c;
  style s;
};

[[nodiscard]] inline text operator|(const string& str, color c) {
  return {str, c, style::normal};
}

[[nodiscard]] inline text operator|(const string& str, style s) {
  return {str, color::normal, s};
}

[[nodiscard]] inline text operator|(text&& t, color color) {
  t.c = color;
  return std::move(t);
}

[[nodiscard]] inline text operator|(text&& t, style style) {
  t.s = style;
  return std::move(t);
}

} // namespace hort::term::io

template <>
struct fmt::formatter<hort::term::io::text> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const hort::term::io::text& t, FormatContext& ctx) {
    return format_to(ctx.out(), "\033[{};{}m{}\033[0m",
                     hort::term::io::detail::ascii_style[static_cast<int>(t.s)],
                     hort::term::io::detail::ascii_color[static_cast<int>(t.c)],
                     t.data);
  }
};

#endif // HORT_TERM_IO_HPP_
