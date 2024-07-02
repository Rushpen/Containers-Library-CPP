#ifndef S21_CPP2_S21_CONTAINERS_1_S21_LIST_LIST_H
#define S21_CPP2_S21_CONTAINERS_1_S21_LIST_LIST_H

#include <iostream>
#include <limits>

#include "s21_iterator_and_node.h"

namespace s21 {

template <typename T>
class list {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  using iterator = listIterator<value_type>;
  using const_iterator = listConstIterator<value_type>;

 private:
  node<value_type> *_head_l;
  node<value_type> *_tail_l;
  size_type _size_l;

 public:
  list();
  list(size_type n);
  list(std::initializer_list<value_type> const &items);
  list(const list &l);
  list(list &&l);
  ~list();
  list<value_type> &operator=(list &&l);
  list<value_type> &operator=(const list &l);

  // element access
  const_reference front();
  const_reference back();

  // iterators
  iterator begin();
  iterator end();
  iterator tail();

  // capacity
  bool empty();
  size_type size();
  size_type max_size();

  // modifiers
  void clear();
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos);
  void push_back(const_reference value);
  void pop_back();
  void push_front(const_reference value);
  void pop_front();
  void swap(list<value_type> &other);
  void merge(list<value_type> &other);
  void splice(const_iterator pos, list &other);
  void reverse();
  void unique();
  void sort();

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args);

  template <typename... Args>
  void insert_many_back(Args &&...args);

  template <typename... Args>
  void insert_many_front(Args &&...args);
};

template <typename value_type>
list<value_type>::list() {
  _size_l = 0;
  _head_l = nullptr;
  _tail_l = nullptr;
}

template <typename value_type>
list<value_type>::list(size_type n) : list() {
  for (size_type i = 0; i < n; ++i) {
    value_type data = value_type();
    push_back(data);
  }
}

template <typename value_type>
list<value_type>::list(std::initializer_list<value_type> const &items)
    : list() {
  for (size_type i = 0; i < items.size(); ++i) {
    push_back(items.begin()[i]);
  }
}

template <typename value_type>
list<value_type>::list(const list &l) : list() {
  typename list<value_type>::iterator it(l._head_l);
  for (size_type i = 0; i < l._size_l; ++i) {
    push_back(*it);
    ++it;
  }
}

template <typename value_type>
list<value_type>::list(list &&l) {
  _size_l = l._size_l;
  _head_l = l._head_l;
  _tail_l = l._tail_l;
  l._size_l = 0;
  l._head_l = nullptr;
  l._tail_l = nullptr;
}

template <typename value_type>
list<value_type>::~list() {
  clear();
}

template <typename value_type>
list<value_type> &list<value_type>::operator=(list<value_type> &&l) {
  if (!empty() && _head_l != l._head_l) {
    this->~list();
  }

  if (_head_l != l._head_l) {
    this->_size_l = l._size_l;
    this->_head_l = l._head_l;
    this->_tail_l = l._tail_l;
    l._size_l = 0;
    l._head_l = nullptr;
    l._tail_l = nullptr;
  }

  return *this;
}

template <typename value_type>
list<value_type> &list<value_type>::operator=(const list &l) {
  if (this != &l) {
    if (!empty()) {
      this->~list();
    }
    _head_l = nullptr;
    _tail_l = nullptr;
    _size_l = 0;
    typename list<value_type>::iterator it(l._head_l);
    for (size_type i = 0; i < l._size_l; ++i) {
      push_back(*it);
      ++it;
    }
    _size_l = l._size_l;
  }
  return *this;
}

template <typename value_type>
typename list<value_type>::const_reference list<value_type>::front() {
  if (empty()) {
    throw std::out_of_range("out_of_range");
  }
  return const_reference(_head_l->data);
}

template <typename value_type>
typename list<value_type>::const_reference list<value_type>::back() {
  if (empty()) {
    throw std::out_of_range("out_of_range");
  }
  return const_reference(_tail_l->data);
}

template <typename value_type>
typename list<value_type>::iterator list<value_type>::begin() {
  return iterator(_head_l);
}

template <typename value_type>
typename list<value_type>::iterator list<value_type>::end() {
  typename list<value_type>::iterator i;
  if (empty()) {
    i = _tail_l;
  } else {
    i = _tail_l->next_node;
  }
  return i;
}

template <typename value_type>
typename list<value_type>::iterator list<value_type>::tail() {
  return iterator(_tail_l);
}

template <typename value_type>
bool list<value_type>::empty() {
  bool result = true;
  if (_size_l) {
    result = false;
  }
  return result;
}

template <typename value_type>
typename list<value_type>::size_type list<value_type>::size() {
  return _size_l;
}

template <typename value_type>
typename list<value_type>::size_type list<value_type>::max_size() {
  return std::numeric_limits<size_type>::max() / sizeof(node<value_type>) / 2;
}

template <typename value_type>
void list<value_type>::clear() {
  size_type c = _size_l;
  for (size_type i = 0; i < c; ++i) {
    pop_back();
  }
}

template <typename value_type>
typename list<value_type>::iterator list<value_type>::insert(
    iterator pos, const_reference value) {
  typename list<value_type>::iterator i;
  if (pos == end()) {
    push_back(value);
    i = iterator(_tail_l);
  } else {
    node<value_type> *ptr = const_cast<node<value_type> *>(pos.get());
    node<value_type> *result = new node<value_type>(value);
    result->next_node = ptr;
    result->prev_node = ptr->prev_node;

    if (pos == begin()) {
      _head_l = result;
    } else {
      ptr->prev_node->next_node = result;
    }
    ptr->prev_node = result;
    i = iterator(result);
    ++_size_l;
  }
  return i;
}

template <typename value_type>
void list<value_type>::erase(iterator pos) {
  if (pos == end()) {
  } else {
    node<value_type> *ptr = const_cast<node<value_type> *>(pos.get());
    if (pos == begin()) {
      _head_l = ptr->next_node;
    } else {
      ptr->prev_node->next_node = ptr->next_node;
    }

    if (ptr == _tail_l) {
      _tail_l = ptr->prev_node;
    } else {
      ptr->next_node->prev_node = ptr->prev_node;
    }
    delete ptr;
    --_size_l;
  }
}

template <typename value_type>
void list<value_type>::push_back(const_reference value) {
  if (_tail_l == nullptr) {
    _tail_l = _head_l = new node<value_type>(value, nullptr, nullptr);

  } else {
    _tail_l->next_node = new node<value_type>(value, _tail_l, nullptr);

    _tail_l->next_node->prev_node = _tail_l;
    _tail_l = _tail_l->next_node;
  }

  ++_size_l;
}

template <typename value_type>
void list<value_type>::pop_back() {
  if (!empty()) {
    node<value_type> *temp = _tail_l;
    _tail_l = _tail_l->prev_node;
    if (temp == _head_l) _head_l = nullptr;
    delete temp;
    --_size_l;
  } else {
    throw std::out_of_range("out_of_range");
  }
}

template <typename value_type>
void list<value_type>::push_front(const_reference value) {
  if (_head_l == nullptr) {
    _head_l = _tail_l = new node<value_type>(value);

  } else {
    _head_l->prev_node = new node<value_type>(value, nullptr, _head_l);
    _head_l = _head_l->prev_node;
  }

  ++_size_l;
}

template <typename value_type>
void list<value_type>::pop_front() {
  if (!empty()) {
    node<value_type> *temp = _head_l;
    _head_l = _head_l->next_node;
    delete temp;
    --_size_l;
  } else {
    throw std::out_of_range("out_of_range");
  }
}

template <typename value_type>
void list<value_type>::swap(list<value_type> &other) {
  size_type tmp_size = other._size_l;
  node<value_type> *temp_head = other._head_l;
  node<value_type> *temp_tail = other._tail_l;

  other._size_l = _size_l;
  other._head_l = _head_l;
  other._tail_l = _tail_l;

  _size_l = tmp_size;
  _head_l = temp_head;
  _tail_l = temp_tail;
}

template <typename value_type>
void list<value_type>::merge(list<value_type> &other) {
  typename list<value_type>::iterator it(_tail_l);
  splice(it, other);
  sort();
}

template <typename value_type>
void list<value_type>::splice(const_iterator pos, list &other) {
  if (pos == end()) {
    _tail_l->next_node = other._head_l;
    other._head_l->prev_node = _tail_l;

    _tail_l = other._tail_l;

  } else {
    node<value_type> *ptr = const_cast<node<value_type> *>(pos.get());
    other._tail_l->next_node = ptr;

    if (pos == begin()) {
      _head_l = other._head_l;
    } else {
      other._head_l->prev_node = ptr->prev_node;
      ptr->prev_node->next_node = other._head_l;
    }
    ptr->prev_node = other._tail_l;
  }
  _size_l = _size_l + other._size_l;
  other._size_l = 0;
  // other._head_l = nullptr;
  // other._tail_l = nullptr;
}

template <typename value_type>
void list<value_type>::reverse() {
  if (!empty()) {
    node<value_type> *temp_head = _head_l;
    typename list<value_type>::iterator it(_head_l);
    for (size_type i = 0; i < size(); ++i) {
      node<value_type> *ptr = const_cast<node<value_type> *>(it.get());
      ++it;
      node<value_type> *temp = ptr->next_node;
      ptr->next_node = ptr->prev_node;
      ptr->prev_node = temp;
    }
    _head_l = _tail_l;
    _tail_l = temp_head;
  }
}

template <typename value_type>
void list<value_type>::unique() {
  using namespace std;
  if (!empty()) {
    typename list<value_type>::iterator it(_head_l->next_node);
    size_type c = _size_l;
    for (size_type i = 1; i < c; ++i) {
      node<value_type> *ptr = const_cast<node<value_type> *>(it.get());
      if (ptr->data == ptr->prev_node->data) {
        typename list<value_type>::iterator it_temp(ptr);
        ++it_temp;
        erase(it);
        it = it_temp;
      } else {
        ++it;
      }
    }
  }
}

template <typename value_type>
void list<value_type>::sort() {
  if (empty()) {
    return;
  }

  typename list<value_type>::iterator left(_head_l);
  typename list<value_type>::iterator right(_tail_l);
  int i_left = 0;
  int i_right = size() - 1;

  while (i_left <= i_right) {
    typename list<value_type>::iterator it1 = right;
    for (int i = i_right; i > i_left; i--) {
      node<value_type> *temp = const_cast<node<value_type> *>(it1.get());
      if (temp->prev_node->data > temp->data) {
        value_type data_swap = temp->prev_node->data;
        temp->prev_node->data = temp->data;
        temp->data = data_swap;
      }
      --it1;
    }
    ++left;
    ++i_left;
    typename list<value_type>::iterator it2 = left;
    for (int i = i_left; i < i_right; ++i) {
      node<value_type> *temp = const_cast<node<value_type> *>(it2.get());
      if (temp->data > temp->next_node->data) {
        value_type data_swap = temp->next_node->data;
        temp->next_node->data = temp->data;
        temp->data = data_swap;
      }
      ++it2;
    }
    --right;
    --i_right;
  }
}

template <typename value_type>
template <typename... Args>
typename list<value_type>::iterator list<value_type>::insert_many(
    const_iterator pos, Args &&...args) {
  typename list<value_type>::iterator it;

  list<value_type> list_temp{args...};
  if (empty()) {
    *this = list_temp;
    it = _head_l;
  } else {
    node<value_type> *ptr = const_cast<node<value_type> *>(pos.get());
    it = ptr;
    splice(pos, list_temp);
  }
  return it;
}

template <typename value_type>
template <typename... Args>
void list<value_type>::insert_many_back(Args &&...args) {
  insert_many(end(), args...);
}

template <typename value_type>
template <typename... Args>
void list<value_type>::insert_many_front(Args &&...args) {
  insert_many(this->begin(), args...);
}
}  // namespace s21

#endif  // S21_CPP2_S21_CONTAINERS_1_S21_LIST_LIST_H_