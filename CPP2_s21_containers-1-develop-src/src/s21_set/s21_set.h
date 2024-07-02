#ifndef CPP2_S21_CONTAINERS_1_S21_SET_SET_H_
#define CPP2_S21_CONTAINERS_1_S21_SET_SET_H_

#include <iostream>

#include "../s21_red_black_tree/RedBlackTree.h"
#include "../s21_vector/s21_vector.h"

namespace s21 {

template <typename Key>
class set {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = typename RedBlackTree<value_type>::iterator;
  using const_iterator = typename RedBlackTree<value_type>::const_iterator;
  using size_type = size_t;

  // Set Constructors and operators
 public:
  set();
  set(std::initializer_list<value_type> const &items);
  set(const set &s);
  set(set &&s) noexcept;
  ~set();

  set<Key> &operator=(const set &s);
  set<Key> &operator=(set &&s);

  // Set Main Iterators
 public:
  iterator begin();
  iterator end();

  // Set Capacity and Modifiers
 public:
  bool empty();
  size_type size();
  size_type max_size();

  void clear();
  std::pair<iterator, bool> insert(const value_type &value);
  iterator find(const Key &key);
  void erase(iterator pos);
  void swap(set &other);
  void merge(set &other);
  bool contains(const Key &key);

  template <typename... Args>
  s21::vector<std::pair<iterator, bool>> insert_many(Args &&...args);

 private:
  RedBlackTree<value_type> tree_;
  iterator FindByKey(const Key &key);
  iterator GetMinKey();
};

template <typename Key>
set<Key>::set() = default;

template <typename Key>
set<Key>::set(
    std::initializer_list<typename set<Key>::value_type> const &items) {
  for (const auto &item : items) {
    insert(item);
  }
}

template <typename Key>
set<Key>::set(const set &s) : tree_(s.tree_) {}

template <typename Key>
set<Key>::set(set &&s) noexcept {
  this->tree_ = std::move(s.tree_);
}

template <typename Key>
set<Key>::~set() = default;

template <typename Key>
set<Key> &set<Key>::operator=(const set &s) {
  if (this != &s) {
    tree_ = s.tree_;
  }
  return *this;
}

template <typename Key>
set<Key> &set<Key>::operator=(set &&s) {
  if (this != &s) {
    tree_.ClearNodes(tree_.GetRoot());
    tree_ = std::move(s.tree_);
  }
  return *this;
}

template <typename Key>
typename set<Key>::iterator set<Key>::begin() {
  return GetMinKey();
}

template <typename Key>
typename set<Key>::iterator set<Key>::end() {
  return iterator(tree_.GetNil(), tree_.GetNil());
}

template <typename Key>
typename set<Key>::iterator set<Key>::find(const Key &key) {
  return FindByKey(key);
}

template <typename Key>
typename set<Key>::iterator set<Key>::FindByKey(const Key &key) {
  iterator it;
  for (it = begin(); it != end(); ++it) {
    if (*it == key) {
      return it;
    }
  }
  return end();
}

template <typename Key>
bool set<Key>::empty() {
  return begin() == end();
}

template <typename Key>
typename set<Key>::size_type set<Key>::size() {
  return tree_.GetSize();
}

template <typename Key>
typename set<Key>::size_type set<Key>::max_size() {
  return tree_.GetMaxSize();
}

template <typename Key>
void set<Key>::clear() {
  tree_.ClearNodes(tree_.GetRoot());
  tree_.SetRoot(tree_.GetNil());
}

template <typename Key>
std::pair<typename set<Key>::iterator, bool> set<Key>::insert(
    const value_type &value) {
  iterator it = find(value);
  bool flag = true;
  if (it == end()) {
    tree_.Insert(value);
    it = find(value);
    flag = true;
  } else {
    flag = false;
  }
  return std::make_pair(it, flag);
}

template <typename Key>
template <typename... Args>
s21::vector<std::pair<typename set<Key>::iterator, bool>> set<Key>::insert_many(
    Args &&...args) {
  s21::vector<std::pair<iterator, bool>> result_vector;
  s21::vector<value_type> new_vector;

  (new_vector.push_back(std::forward<Args>(args)), ...);

  for (size_t i = 0; i < sizeof...(args); ++i) {
    auto result = insert(new_vector[i]);
    result_vector.push_back(result);
  }
  return result_vector;
}

template <typename Key>
void set<Key>::erase(iterator pos) {
  iterator it = find(*pos);
  if (it != end()) {
    tree_.RemoveNode(tree_.FindNode(*it));
  }
}

template <typename Key>
void set<Key>::swap(set &other) {
  std::swap(tree_, other.tree_);
}

template <typename Key>
void set<Key>::merge(set &other) {
  iterator it = other.begin();
  while (it != other.end()) {
    iterator next = std::next(it);
    if (insert(*it).second) {
      other.erase(it);
    }
    it = next;
  }
}

template <typename Key>
bool set<Key>::contains(const Key &key) {
  iterator it = find(key);
  bool result = !(it == end());
  return result;
}

template <typename Key>
typename set<Key>::iterator set<Key>::GetMinKey() {
  return iterator(tree_.MinNode(tree_.GetRoot()), tree_.GetNil());
}

}  // namespace s21

#endif  // CPP2_S21_CONTAINERS_1_SET_H_