#ifndef CPP2_S21_CONTAINERS_1_S21_VECTOR_VECTOR_H_
#define CPP2_S21_CONTAINERS_1_S21_VECTOR_VECTOR_H_

#include <iostream>
#include <limits>

namespace s21 {

template <typename T>
class vector {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = size_t;

 private:
  value_type *_arr;
  size_type _size;
  size_type _capacity;

 public:
  inline vector() : _arr(nullptr), _size(0), _capacity(0) {}

  inline explicit vector(size_type n)
      : _arr(new value_type[n]), _size(n), _capacity(n) {
    if (_arr == nullptr) {
      throw std::bad_alloc();
    }
    for (size_type i = 0; i < n; ++i) {
      _arr[i] = T();
    }
  }

  vector<value_type>(std::initializer_list<value_type> const &items);

  vector(const vector &v);

  vector(vector &&v) noexcept;

  ~vector();

  vector<value_type> &operator=(const vector &v);
  vector<value_type> &operator=(vector &&v) noexcept;

  reference at(size_type pos);

  reference operator[](size_type pos);

  const_reference front();

  const_reference back();

  T *data();

  //

  iterator begin();

  iterator end();

  //

  bool empty() noexcept;

  size_type size();

  size_type max_size();

  void reserve(size_type size);

  size_type capacity();

  void shrink_to_fit();

  //

  void clear();

  iterator insert(iterator pos, const_reference value);

  void erase(iterator pos);

  void push_back(const_reference value);

  void pop_back();

  void swap(vector &other);

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args);
  //
  template <typename... Args>
  void insert_many_back(Args &&...args);
};

template <typename value_type>
vector<value_type>::vector(std::initializer_list<value_type> const &items) {
  _size = items.size();
  _capacity = items.size();
  _arr = new value_type[_capacity];
  for (size_type i = 0; i < _capacity; ++i) {
    _arr[i] = items.begin()[i];
  }
}

template <typename value_type>
vector<value_type>::vector(const vector<value_type> &v) {
  _size = v._size;
  _capacity = v._capacity;
  _arr = new value_type[_capacity];
  for (size_type i = 0; i < _capacity; ++i) {
    _arr[i] = v._arr[i];
  }
}

template <typename value_type>
vector<value_type>::vector(vector<value_type> &&v) noexcept {
  _size = v._size;
  v._size = 0;
  _capacity = v._capacity;
  v._capacity = 0;
  _arr = v._arr;
  v._arr = nullptr;
}

template <typename value_type>
vector<value_type>::~vector() {
  _size = 0;
  _capacity = 0;
  delete[] _arr;
  _arr = nullptr;
}

template <typename value_type>
vector<value_type> &vector<value_type>::operator=(const vector<value_type> &v) {
  if (this != &v) {
    delete[] _arr;
    _size = v._size;
    _capacity = v._capacity;
    _arr = new value_type[_capacity];
    for (size_type i = 0; i < _size; ++i) {
      _arr[i] = v._arr[i];
    }
  }
  return *this;
}

template <typename value_type>
vector<value_type> &vector<value_type>::operator=(
    vector<value_type> &&v) noexcept {
  if (this != &v) {
    delete[] _arr;
    _arr = v._arr;
    _size = v._size;
    _capacity = v._capacity;
    v._arr = nullptr;
    v._size = 0;
    v._capacity = 0;
  }
  return *this;
}

template <typename value_type>
typename vector<value_type>::reference vector<value_type>::at(size_type pos) {
  if (pos >= _size || _size == 0) {
    throw std::out_of_range("Index out of range");
  }
  return _arr[pos];
}

template <typename value_type>
typename vector<value_type>::reference vector<value_type>::operator[](
    size_type pos) {
  return at(pos);
}

template <typename value_type>
typename vector<value_type>::const_reference vector<value_type>::front() {
  return *_arr;
}

template <typename value_type>
typename vector<value_type>::const_reference vector<value_type>::back() {
  if (_size < 1) {
    throw std::out_of_range("Index out of range");
  }
  return *(_arr + _size - 1);
}

template <typename value_type>
inline value_type *vector<value_type>::data() {
  return _arr;
}

template <typename value_type>
inline typename vector<value_type>::iterator vector<value_type>::begin() {
  return _arr;
}

template <typename value_type>
inline typename vector<value_type>::iterator vector<value_type>::end() {
  return _arr + _size;
}

template <typename value_type>
inline bool vector<value_type>::empty()

    noexcept {
  return (!_size);
}

template <typename value_type>
inline typename vector<value_type>::size_type vector<value_type>::size() {
  return _size;
}

template <typename value_type>
inline typename vector<value_type>::size_type vector<value_type>::max_size() {
  return std::numeric_limits<size_type>::max() / sizeof(value_type) / 2;
}

template <typename value_type>
inline void vector<value_type>::reserve(size_type size) {
  if (size > max_size()) {
    throw std::length_error("Can't allocate memory of this size");
  }
  if (size > _capacity) {
    auto *tmpPtr = new value_type[size];
    for (size_type i = 0; i < _size; ++i) {
      tmpPtr[i] = std::move(_arr[i]);
    }
    delete[] _arr;
    _arr = tmpPtr;
    _capacity = size;
  }
}

template <typename value_type>
inline typename vector<value_type>::size_type vector<value_type>::capacity() {
  return _capacity;
}

template <typename value_type>
inline void vector<value_type>::shrink_to_fit() {
  if (_size == _capacity) {
    return;
  }
  auto *tmpPtr = new value_type[_size];
  for (size_type i = 0; i < _size; ++i) {
    tmpPtr[i] = _arr[i];
  }
  delete[] _arr;
  _arr = tmpPtr;
  _capacity = _size;
}

template <typename value_type>
inline void vector<value_type>::clear() {
  for (size_type i = 0; i < _size; ++i) {
    _arr[i] = value_type();
  }
  _size = 0;
}

template <typename value_type>
inline typename vector<value_type>::iterator vector<value_type>::insert(
    iterator pos, const_reference value) {
  size_type offset = pos - _arr;
  if (_capacity > _size) {
    for (iterator i = end(); i != pos; --i) {
      *i = *(i - 1);
    }
    _arr[offset] = value;
    ++_size;
  } else {
    reserve(_capacity ? _capacity * 2 : 1);
    return insert(_arr + offset, value);
  }
  return _arr + offset;
}

template <typename value_type>
inline void vector<value_type>::erase(iterator pos) {
  for (iterator i = pos; i < end() - 1; ++i) {
    *i = *(i + 1);
  }
  *(end() - 1) = value_type();
  --_size;
}

template <typename value_type>
inline void vector<value_type>::push_back(const_reference value) {
  if (_size < _capacity) {
    ++_size;
    _arr[_size - 1] = value;
  } else {
    reserve(_capacity ? _capacity * 2 : 1);
    push_back(value);
  }
}

template <typename value_type>
inline void vector<value_type>::pop_back() {
  if (_size > 0) {
    --_size;
    _arr[_size] = value_type();
  }
}

template <typename value_type>
void vector<value_type>::swap(vector &other) {
  size_type tmpSize = _size;
  _size = other._size;
  other._size = tmpSize;
  size_type tmpCapacity = _capacity;
  _capacity = other._capacity;
  other._capacity = tmpCapacity;
  value_type *tmpArr = _arr;
  _arr = other._arr;
  other._arr = tmpArr;
}

template <typename value_type>
template <typename... Args>
typename vector<value_type>::iterator vector<value_type>::insert_many(
    const_iterator pos, Args &&...args) {
  size_type insert_count = sizeof...(args);
  size_type offset = pos - begin();

  // Ensure enough capacity for the new elements
  if (_size + insert_count > _capacity) {
    reserve((_size + insert_count) * 2);
  }

  iterator non_const_pos = begin() + offset;

  // Move existing elements to the right
  for (iterator it = end() + insert_count - 1;
       it >= non_const_pos + insert_count; --it) {
    *it = std::move(*(it - insert_count));
  }

  // Insert new elements
  iterator first_inserted = non_const_pos;
  (void(std::initializer_list<int>{
      (*non_const_pos++ = std::forward<Args>(args), 0)...}));

  _size += insert_count;
  return first_inserted;
}

template <typename value_type>
template <typename... Args>
void vector<value_type>::insert_many_back(Args &&...args) {
  insert_many(this->end(), args...);
}

}  // namespace s21

#endif  // CPP2_S21_CONTAINERS_1_S21_VECTOR_VECTOR_H_