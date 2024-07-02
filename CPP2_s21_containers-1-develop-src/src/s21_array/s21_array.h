#ifndef S21_CPP2_S21_CONTAINERS_1_S21_ARRAY_ARRAY_H_
#define S21_CPP2_S21_CONTAINERS_1_S21_ARRAY_ARRAY_H_

#include <iostream>
#include <limits>

namespace s21 {

template <typename T, std::size_t N>
class array {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = size_t;

 private:
  size_type _size = N;
  value_type _arr[N];

 public:
  // array Member functions
  array() = default;
  array(std::initializer_list<value_type> const &items);
  array(const array &a);
  array(array &&a) noexcept;
  ~array();
  array<value_type, N> &operator=(array &&a) noexcept;
  // array Element access
  reference at(size_type pos);
  reference operator[](size_type pos);
  const_reference front();
  const_reference back();
  iterator data();
  // array Iterators
  iterator begin();
  iterator end();
  // array Capacity
  bool empty();
  size_type size();
  size_type max_size();
  // array Modifiers
  void swap(array &other);
  void fill(const_reference value);
};

template <typename value_type, std::size_t N>
inline array<value_type, N>::array(
    std::initializer_list<value_type> const &items) {
  if (items.size() > N) {
    throw std::out_of_range("Initializer list size exceeds array size");
  }
  for (size_type i = 0; i < _size; ++i) {
    _arr[i] = items.begin()[i];
  }
}

template <typename value_type, std::size_t N>
inline array<value_type, N>::array(const array &a) {
  for (size_type i = 0; i < _size; ++i) {
    _arr[i] = a._arr[i];
  }
}

template <typename value_type, std::size_t N>
inline array<value_type, N>::array(array &&a) noexcept : array(a) {}

template <typename value_type, std::size_t N>
inline array<value_type, N>::~array() = default;

template <typename value_type, std::size_t N>
array<value_type, N> &array<value_type, N>::operator=(
    array<value_type, N> &&a) noexcept {
  if (this != &a) {
    for (size_type i = 0; i < _size; ++i) {
      _arr[i] = a._arr[i];
    }
  }
  return *this;
}

template <typename value_type, std::size_t N>
inline typename array<value_type, N>::reference array<value_type, N>::at(
    size_t pos) {
  if (pos >= _size || _size == 0) {
    throw std::out_of_range("Index out of range");
  }
  return _arr[pos];
}

template <typename value_type, std::size_t N>
inline typename array<value_type, N>::reference
array<value_type, N>::operator[](size_t pos) {
  return _arr[pos];
}

template <typename value_type, std::size_t N>
inline typename array<value_type, N>::const_reference
array<value_type, N>::front() {
  return *_arr;
}

template <typename value_type, std::size_t N>
inline typename array<value_type, N>::const_reference
array<value_type, N>::back() {
  return _arr[_size - 1];
}

template <typename value_type, std::size_t N>
inline typename array<value_type, N>::iterator array<value_type, N>::data() {
  return _arr;
}

template <typename value_type, std::size_t N>
inline typename array<value_type, N>::iterator array<value_type, N>::begin() {
  return _arr;
}

template <typename value_type, std::size_t N>
inline typename array<value_type, N>::iterator array<value_type, N>::end() {
  return _arr + _size;
}

template <typename value_type, std::size_t N>
inline bool array<value_type, N>::empty() {
  return !_size;
}

template <typename value_type, std::size_t N>
inline typename array<value_type, N>::size_type array<value_type, N>::size() {
  return _size;
}

template <typename value_type, std::size_t N>
inline typename array<value_type, N>::size_type
array<value_type, N>::max_size() {
  return _size;
}

template <typename value_type, std::size_t N>
inline void array<value_type, N>::swap(array &other) {
  value_type temp;
  for (size_type i = 0; i < _size; ++i) {
    temp = _arr[i];
    _arr[i] = other._arr[i];
    other._arr[i] = temp;
  }
}

template <typename value_type, std::size_t N>
inline void array<value_type, N>::fill(const_reference value) {
  for (size_type i = 0; i < _size; ++i) {
    _arr[i] = value;
  }
}

}  // namespace s21

#endif  // S21_CPP2_S21_CONTAINERS_1_S21_ARRAY_ARRAY_H_
