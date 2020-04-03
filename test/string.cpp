#include <gtest/gtest.h>

#include "hort/string.hpp"

TEST(String, Joiner) {
  EXPECT_EQ("/"_join("foo", "bar"), "foo/bar");
}

TEST(String, Formatter) {
  EXPECT_EQ("{} {}"_format("foo", "bar"), "foo bar");
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
