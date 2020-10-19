#include <gtest/gtest.h>

#include "hort/string.hpp"

TEST(String, Trim) {
  struct pair {
    std::string_view first;
    std::string_view second;
    std::string_view third;
    std::string_view fourth;
  };

  std::array tests = {
    pair{" foo   ", "foo", "foo   ", " foo"},
  };

  for (const auto &[s, t, l, r] : tests) {
    ASSERT_EQ(hort::string(s).trim(),  t);
    ASSERT_EQ(hort::string(s).ltrim(), l);
    ASSERT_EQ(hort::string(s).rtrim(), r);
  }
}
