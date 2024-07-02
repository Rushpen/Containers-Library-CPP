#ifndef CPP2_S21_CONTAINERS_1_S21_STACK_STACK_H_
#define CPP2_S21_CONTAINERS_1_S21_STACK_STACK_H_

#include "../s21_list/s21_list.h"

namespace s21 {

template <typename T>
class stack {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

 private:
  list<value_type> _stack;

 public:
  stack();
  stack(std::initializer_list<value_type> const &items);
  stack(const stack &s);
  stack(stack &&s);
  ~stack();
  stack<value_type> &operator=(stack<value_type> &&s);

  // element access
  const_reference top();

  // capacity
  bool empty();
  size_type size();

  // modifiers
  void push(const_reference value);
  void pop();
  void swap(stack<value_type> &other);

  template <typename... Args>
  void insert_many_front(Args &&...args);
};

template <typename value_type>
stack<value_type>::stack() : _stack() {}

template <typename value_type>
stack<value_type>::stack(std::initializer_list<value_type> const &items)
    : _stack(items) {}

template <typename value_type>
stack<value_type>::stack(const stack &s) : _stack(s._stack) {}

template <typename value_type>
stack<value_type>::stack(stack &&s) : _stack(std::move(s._stack)) {}

template <typename value_type>
stack<value_type>::~stack() {
  this->_stack.clear();
}

template <typename value_type>
stack<value_type> &stack<value_type>::operator=(stack<value_type> &&s) {
  this->_stack = s._stack;

  return *this;
}

template <typename value_type>
typename stack<value_type>::const_reference stack<value_type>::top() {
  return _stack.back();
}

template <typename value_type>
bool stack<value_type>::empty() {
  return _stack.empty();
}

template <typename value_type>
typename stack<value_type>::size_type stack<value_type>::size() {
  return _stack.size();
}

template <typename value_type>
void stack<value_type>::push(const_reference value) {
  _stack.push_back(value);
}

template <typename value_type>
void stack<value_type>::pop() {
  _stack.pop_back();
}

template <typename value_type>
void stack<value_type>::swap(stack<value_type> &other) {
  _stack.swap(other._stack);
}

template <typename value_type>
template <typename... Args>
inline void stack<value_type>::insert_many_front(Args &&...args) {
  _stack.insert_many_front(args...);
}
}  // namespace s21

#endif  // CPP2_S21_CONTAINERS_1_S21_STACK_STACK_H_
