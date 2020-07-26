#ifndef HORT_VECTOR_HPP_
#define HORT_VECTOR_HPP_

#include <functional>
#include <vector>

namespace hort {

template <typename T>
class vector : public std::vector<T> {
public:
  using size_type  = int;
  using value_type = T;

  using std::vector<T>::vector;

  explicit operator bool() const { return std::vector<T>::size() > 0; }

  /// \brief Access element at position `pos`. Negative index wraps to end.
  [[nodiscard]] const T& at(size_type pos) const noexcept {
    if (pos < 0) {
      return std::vector<T>::at(std::vector<T>::size() + pos);
    } else {
      return std::vector<T>::at(pos);
    }
  }

  /// \brief Access element at position `pos`. Negative index wraps to end.
  [[nodiscard]] T &at(size_type pos) noexcept {
    if (pos < 0) {
      return std::vector<T>::at(std::vector<T>::size() + pos);
    } else {
      return std::vector<T>::at(pos);
    }
  }

  /// \brief Return position of first instance of `val`.
  [[nodiscard]] size_type index(const T& val) const noexcept {
    size_type pos = 0;
    for (const auto& i : *this) {
      if (i == val) {
        return pos;
      }
      pos++;
    }
    return -1;
  }

  /// \brief Return position of first instance of `val`.
  template <typename Func>
  [[nodiscard]] size_type index(const Func& compare) const noexcept {
    size_type pos = 0;
    for (const auto& i : *this) {
      if (compare(i)) {
        return pos;
      }
      pos++;
    }
    return -1;
  }

  template <typename Func>
  const vector& map(const Func& lambda) const {
    for (const auto& i : *this) {
      lambda(i);
    }
  }

  template <typename Func>
  vector& map(const Func& lambda) {
    for (auto& i : *this) {
      lambda(i);
    }
  }

};

} // namespace hort

#endif // HORT_VECTOR_HPP_
