#include <gtest/gtest.h>

#include "hort/vector.hpp"

TEST(Vector, Indexing) {
  hort::vector<int> v = {5, 6, 9, 7};
  ASSERT_EQ(v.at(0),  5);
  ASSERT_EQ(v.at(1),  6);
  ASSERT_EQ(v.at(2),  9);
  ASSERT_EQ(v.at(3),  7);

  ASSERT_EQ(v.at(-1), 7);
  ASSERT_EQ(v.at(-2), 9);
  ASSERT_EQ(v.at(-3), 6);
  ASSERT_EQ(v.at(-4), 5);
}

TEST(Vector, Map) {
  hort::vector<int> v = {5, 6, 9, 7};
  hort::vector<int> w = {6, 7, 10, 8};
  v.map([](int &i) { i += 1; });
  ASSERT_EQ(v, w);
}

TEST(Vector, Index) {
  hort::vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  for (int i = 0; const auto & e : v) {
    ASSERT_EQ(v.index(e), i);
    i++;
  }

  struct pair {
    int first;
    int second;
  };

  hort::vector<pair> w = {
    pair{1, 1},
    pair{2, 2},
    pair{3, 3},
    pair{4, 4},
  };

  for (int i = 0; auto & e : w) {
    const auto callback = [e](const auto &i) {
      return i.first == e.first;
    };
    ASSERT_EQ(w.index(callback), i);
    i++;
  }
}
