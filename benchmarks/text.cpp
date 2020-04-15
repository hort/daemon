#include <benchmark/benchmark.h>

#include <fmt/format.h>
#include <string>

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

struct text1 {
  std::string str;
  color c;
  style s;

  operator const char*() const {
    return fmt::format("\033[{};{}m{}\033[0m",
      ascii_style[static_cast<int>(s)],
      ascii_color[static_cast<int>(c)],
      str).data();
  }

};

static void Operator(benchmark::State& state) {
  for (auto _ : state) {
    fmt::format("{}", text1{"foo bar", color::white, style::bold});
  }
}
BENCHMARK(Operator);

struct text2 {
  std::string str;
  color c;
  style s;
};

template <>
struct fmt::formatter<text2> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const text2& t, FormatContext& ctx) {
    return format_to(ctx.out(), "\033[{};{}m{}\033[0m",
                     ascii_style[static_cast<int>(t.s)],
                     ascii_color[static_cast<int>(t.c)],
                     t.str);
  }
};

static void Formatter(benchmark::State& state) {
  for (auto _ : state) {
    fmt::format("{}", text2{"foo bar", color::white, style::bold});
  }
}
BENCHMARK(Formatter);

BENCHMARK_MAIN();
