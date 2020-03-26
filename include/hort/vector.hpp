#ifndef HORT_VECTOR_HPP_
#define HORT_VECTOR_HPP_

#include <functional>
#include <vector>

namespace hort {

template <typename T>
class vector : public std::vector<T> {
public:
  using size_type = int;

  explicit operator bool() const { return std::vector<T>::size(); }

  /// \brief Access element at position `pos`. If number is negative wrap to end
  /// of vector.
  const T& operator[](size_type pos) const {
    if (pos < 0) {
      return std::vector<T>::operator[](std::vector<T>::size() + pos - 1);
    } else {
      return std::vector<T>::operator[](pos);
    }
  }

  /// \brief Access element at position `pos`. If number is negative wrap to end
  /// of vector.
  T operator[](size_type pos) {
    if (pos < 0) {
      return std::vector<T>::operator[](std::vector<T>::size() + pos - 1);
    } else {
      return std::vector<T>::operator[](pos);
    }
  }

  /// \brief Return position of first instance of `val`.
  size_type index(const T& val) {
    int pos = 0;
    for (const auto& i : *this) {
      if (i == val) {
        return pos;
      }
      pos++;
    }
    return -1;
  }

  /// \brief Return position of first instance of `val`.
  size_type index(const std::function<bool(const T&)>& compare) {
    size_type pos = 0;
    for (const auto& i : *this) {
      if (compare(i)) {
        return pos;
      }
      pos++;
    }
    return -1;
  }
};

} // namespace hort

#endif // HORT_VECTOR_HPP_
