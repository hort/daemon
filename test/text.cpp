#include <gtest/gtest.h>
#include <array>

#include "hort/term/io.hpp"

/*
TEST(Text, Normal) {
  using namespace hort::term::io;

  struct pair {
    text first;
    std::string_view second;
  };

  std::array tests = {
    pair{ text{"bar", color::normal, style::normal}, "\x1B[0;39mbar\x1B[0m" },
    pair{ text{"bar", color::black,  style::normal}, "\x1B[0;30mbar\x1B[0m" },
    pair{ text{"bar", color::white,  style::normal}, "\x1B[0;97mbar\x1B[0m" },
    pair{ text{"bar", color::normal, style::bold},   "\x1B[1;39mbar\x1B[0m" },
    pair{ text{"bar", color::black,  style::bold},   "\x1B[1;30mbar\x1B[0m" },
    pair{ text{"bar", color::white,  style::bold},   "\x1B[1;97mbar\x1B[0m" },
    pair{ text{"bar", color::normal, style::italic}, "\x1B[3;39mbar\x1B[0m" },
    pair{ text{"bar", color::black,  style::italic}, "\x1B[3;30mbar\x1B[0m" },
    pair{ text{"bar", color::white,  style::italic}, "\x1B[3;97mbar\x1B[0m" },
  };

  for (const auto &[s, f] : tests) {
    ASSERT_EQ(fmt::format("foo {}", s), fmt::format("foo {}", f));
  }

}

TEST(Text, Implicit) {
  using namespace hort::term::io;
  std::array tests = {
    "foo" | color::normal | style::normal,
    "foo" | style::normal | color::normal,
    "foo" | color::normal,
    "foo" | style::normal,
  };
  for (const auto & s : tests) {
    ASSERT_EQ(fmt::format("{}", s), "\x1B[0;39mfoo\x1B[0m");
  }
}
*/
