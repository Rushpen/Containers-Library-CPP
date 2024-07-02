#ifndef CPP2_S21_CONTAINERS_1_S21_MULTISET_MULTISET_H_
#define CPP2_S21_CONTAINERS_1_S21_MULTISET_MULTISET_H_

#include "../s21_red_black_tree/RedBlackTree.h"
#include "../s21_vector/s21_vector.h"

namespace s21 {

template <typename Key>
class multiset {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = typename RedBlackTree<value_type>::iterator;
  using const_iterator = typename RedBlackTree<value_type>::const_iterator;
  using size_type = size_t;

  // Multiset Constructors and operators
 public:
  multiset();
  multiset(std::initializer_list<value_type> const &items);
  multiset(const multiset &ms);
  multiset(multiset &&ms);
  ~multiset();

  multiset<Key> &operator=(const multiset &ms);
  multiset<Key> &operator=(multiset &&ms);

  // Multiset Main Iterators
 public:
  iterator begin();
  iterator end();

  // Multiset Capacity and Modifiers
 public:
  bool empty();
  size_type size();
  size_type max_size();

  void clear();
  iterator insert(const value_type &value);
  void erase(iterator pos);
  void swap(multiset &other);
  void merge(multiset &other);

  // Multiset Lookup
 public:
  size_type count(const Key &key);
  iterator find(const Key &key);
  bool contains(const Key &key);
  std::pair<iterator, iterator> equal_range(const Key &key);
  iterator lower_bound(const Key &key);
  iterator upper_bound(const Key &key);

  template <typename... Args>
  s21::vector<std::pair<iterator, bool>> insert_many(Args &&...args);

 private:
  RedBlackTree<value_type> tree_;
  iterator FindByKey(const Key &key);
  // iterator GetMaxKey();
  iterator GetMinKey();
};

template <typename Key>
multiset<Key>::multiset() = default;

template <typename Key>
multiset<Key>::multiset(std::initializer_list<value_type> const &items) {
  for (const auto &item : items) {
    insert(item);
  }
}

template <typename Key>
multiset<Key>::multiset(const multiset &ms) : tree_(ms.tree_) {}

template <typename Key>
multiset<Key>::multiset(multiset &&ms) {
  this->tree_ = std::move(ms.tree_);
}

template <typename Key>
multiset<Key>::~multiset() = default;

template <typename Key>
multiset<Key> &multiset<Key>::operator=(const multiset &ms) {
  if (this != &ms) {
    tree_ = ms.tree_;
  }
  return *this;
}

template <typename Key>
multiset<Key> &multiset<Key>::operator=(multiset &&ms) {
  if (this != &ms) {
    tree_.ClearNodes(tree_.GetRoot());
    tree_ = std::move(ms.tree_);
  }
  return *this;
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::begin() {
  return GetMinKey();
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::end() {
  return iterator(tree_.GetNil(), tree_.GetNil());
}

template <typename Key>
bool multiset<Key>::empty() {
  return begin() == end();
}

template <typename Key>
typename multiset<Key>::size_type multiset<Key>::size() {
  return tree_.GetSize();
}

template <typename Key>
typename multiset<Key>::size_type multiset<Key>::max_size() {
  return tree_.GetMaxSize();
}

template <typename Key>
void multiset<Key>::clear() {
  tree_.ClearNodes(tree_.GetRoot());
  tree_.SetRoot(tree_.GetNil());
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::insert(
    const value_type &value) {
  tree_.Insert(value);
  return find(value);
}

template <typename Key>
template <typename... Args>
s21::vector<std::pair<typename multiset<Key>::iterator, bool>>
multiset<Key>::insert_many(Args &&...args) {
  s21::vector<std::pair<iterator, bool>> result_vector;
  s21::vector<value_type> new_vector;

  (new_vector.push_back(std::forward<Args>(args)), ...);

  for (size_t i = 0; i < sizeof...(args); ++i) {
    auto result = insert(new_vector[i]);
    result_vector.push_back(std::make_pair(result, true));
  }
  return result_vector;
}

template <typename Key>
void multiset<Key>::erase(iterator pos) {
  iterator it = find(*pos);
  if (it != end()) {
    tree_.RemoveNode(tree_.FindNode(*it));
  }
}

template <typename Key>
void multiset<Key>::swap(multiset &other) {
  std::swap(tree_, other.tree_);
}

//--------------------------------------------//
template <typename Key>
void multiset<Key>::merge(multiset &other) {
  iterator it;
  for (it = other.begin(); it != other.end(); ++it) {
    insert(*it);
  }
  other.clear();
}

template <typename Key>
typename multiset<Key>::size_type multiset<Key>::count(const Key &key) {
  size_type count = 0;
  for (iterator it = begin(); it != end(); ++it) {
    if (*it == key) {
      ++count;
    }
  }
  return count;
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::find(const Key &key) {
  return FindByKey(key);
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::FindByKey(const Key &key) {
  for (iterator it = begin(); it != end(); ++it) {
    if (*it == key) {
      return it;
    }
  }
  return end();
}

template <typename Key>
bool multiset<Key>::contains(const Key &key) {
  iterator it = find(key);
  bool result = !(it == end());
  return result;
}

template <typename Key>
std::pair<typename multiset<Key>::iterator, typename multiset<Key>::iterator>
multiset<Key>::equal_range(const Key &key) {
  return {lower_bound(key), upper_bound(key)};
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::lower_bound(const Key &key) {
  iterator it;
  for (it = begin(); it != end(); ++it) {
    if (!(*it < key)) {
      return it;
    }
  }
  return end();
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::upper_bound(const Key &key) {
  for (iterator it = begin(); it != end(); ++it) {
    if (*it > key) {
      return it;
    }
  }
  return end();
}

// template <typename Key>
// typename multiset<Key>::iterator multiset<Key>::GetMaxKey() {
//   return iterator(tree_.MaxNode(tree_.GetRoot()), tree_.GetNil());
// }

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::GetMinKey() {
  return iterator(tree_.MinNode(tree_.GetRoot()), tree_.GetNil());
}

}  // namespace s21

#endif  // CPP2_S21_CONTAINERS_1_S21_MULTISET_MULTISET_H_
