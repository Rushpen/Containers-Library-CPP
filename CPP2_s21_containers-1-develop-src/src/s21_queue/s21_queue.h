#ifndef CPP2_S21_CONTAINERS_1_S21_QUEUE_QUEUE_H_
#define CPP2_S21_CONTAINERS_1_S21_QUEUE_QUEUE_H_

#include "../s21_list/s21_list.h"

namespace s21 {

template <typename T>
class queue {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

 private:
  list<value_type> _queue;

 public:
  queue();
  queue(std::initializer_list<value_type> const &items);
  queue(const queue &q);
  queue(queue &&q);
  ~queue();
  queue<value_type> &operator=(queue<value_type> &&q);

  // element access
  const_reference front();
  const_reference back();

  // capacity
  bool empty();
  size_type size();

  // modifiers
  void push(const_reference value);
  void pop();
  void swap(queue<value_type> &other);

  template <typename... Args>
  void insert_many_back(Args &&...args);
};

template <typename value_type>
queue<value_type>::queue() : _queue() {}

template <typename value_type>
queue<value_type>::queue(std::initializer_list<value_type> const &items)
    : _queue(items) {}

template <typename value_type>
queue<value_type>::queue(const queue &q) : _queue(q._queue) {}

template <typename value_type>
queue<value_type>::queue(queue &&q) : _queue(std::move(q._queue)) {}

template <typename value_type>
queue<value_type>::~queue() {
  this->_queue.clear();
}

template <typename value_type>
queue<value_type> &queue<value_type>::operator=(queue<value_type> &&q) {
  this->_queue = q._queue;

  return *this;
}

template <typename value_type>
typename queue<value_type>::const_reference queue<value_type>::front() {
  return _queue.front();
}

template <typename value_type>
typename queue<value_type>::const_reference queue<value_type>::back() {
  return _queue.back();
}

template <typename value_type>
bool queue<value_type>::empty() {
  return _queue.empty();
}

template <typename value_type>
typename queue<value_type>::size_type queue<value_type>::size() {
  return _queue.size();
}

template <typename value_type>
void queue<value_type>::push(const_reference value) {
  _queue.push_back(value);
}

template <typename value_type>
void queue<value_type>::pop() {
  _queue.pop_front();
}

template <typename value_type>
void queue<value_type>::swap(queue<value_type> &other) {
  _queue.swap(other._queue);
}

template <typename value_type>
template <typename... Args>
void queue<value_type>::insert_many_back(Args &&...args) {
  _queue.insert_many_back(args...);
}
}  // namespace s21

#endif  // CPP2_S21_CONTAINERS_1_S21_QUEUE_QUEUE_H_
