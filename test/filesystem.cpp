#include <gtest/gtest.h>
#include <array>

#include "hort/filesystem.hpp"

TEST(Filesystem, Mkpath) {
  ASSERT_EQ(hort::filesystem::mkpath("foo", "bar", "baz"), false);
  ASSERT_EQ(hort::filesystem::exists("foo", "bar", "baz"), true);
}

TEST(Filesystem, BaseName) {
  struct pair {
    std::string first;
    std::string second;
  };

  std::array tests = {
    pair{"baz.foo",             "baz.foo" },
    pair{"/foo/bar/baz.foo",    "baz.foo" },
    pair{"/foo//bar/baz.foo",   "baz.foo" },
    pair{"/foo///bar/baz.foo/", ""        },
    pair{"/",                   ""        },
    pair{"",                    ""        },
  };

  for (const auto &[p, s] : tests) {
    ASSERT_EQ(hort::filesystem::base_name(p), s);
  }
}
