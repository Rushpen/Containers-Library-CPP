#ifndef CPP2_S21_CONTAINERS_1_S21_MAP_MAP_H_
#define CPP2_S21_CONTAINERS_1_S21_MAP_MAP_H_

#include "../s21_red_black_tree/RedBlackTree.h"
#include "../s21_vector/s21_vector.h"

namespace s21 {

template <typename Key, typename T>
class map {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type &;
  using const_reference = const value_type &;

  using iterator = typename RedBlackTree<value_type>::iterator;
  using const_iterator = typename RedBlackTree<value_type>::const_iterator;
  using size_type = size_t;

  //Конструкторы и основные операторы
 public:
  map();
  map(std::initializer_list<value_type> const &items);
  map(const map &m);
  map(map &&m) noexcept;
  ~map();

  map<Key, T> &operator=(const map &m);
  map<Key, T> &operator=(map &&m);
  T &at(const Key &key);
  T &operator[](const Key &key);

 public:
  iterator begin();
  iterator end();
  iterator find(const Key &key);

 public:
  bool empty();
  size_type size();
  size_type max_size();
  void clear();

  std::pair<iterator, bool> insert(const value_type &value);
  std::pair<iterator, bool> insert(const Key &key, const T &obj);
  std::pair<iterator, bool> insert_or_assign(const Key &key, const T &obj);

  void erase(iterator pos);
  void swap(map &other);
  void merge(map &other);
  bool contains(const Key &key);

  template <typename... Args>
  s21::vector<std::pair<iterator, bool>> insert_many(Args &&...args);

 private:
  RedBlackTree<value_type> tree_;
  iterator FindByKey(const Key &key);
  void ClearElements();
};

/////////////////////////////////////////////////////////

template <typename Key, typename T>
map<Key, T>::map() = default;

template <typename Key, typename T>
map<Key, T>::map(std::initializer_list<value_type> const &items) {
  for (const auto &item : items) {
    insert(item);
  }
}

template <typename Key, typename T>
map<Key, T>::map(const map &m) : tree_(m.tree_) {}

template <typename Key, typename T>
map<Key, T>::map(map &&other) noexcept {
  this->tree_ = std::move(other.tree_);
}

template <typename Key, typename T>
map<Key, T>::~map() = default;

template <typename Key, typename T>
map<Key, T> &map<Key, T>::operator=(const map &m) {
  if (this != &m) {
    tree_ = m.tree_;
  }
  return *this;
}

template <typename Key, typename T>
map<Key, T> &map<Key, T>::operator=(map &&m) {
  if (this != &m) {
    tree_.ClearNodes(tree_.GetRoot());
    tree_ = std::move(m.tree_);
  }
  return *this;
}

template <typename Key, typename T>
T &map<Key, T>::at(const Key &key) {
  iterator it = find(key);
  if (it == end()) {
    throw std::out_of_range("Key not found");
  }
  return it->second;
}

template <typename Key, typename T>
T &map<Key, T>::operator[](const Key &key) {
  iterator it = find(key);
  if (it == end()) {
    insert(std::make_pair(key, mapped_type()));
    it = find(key);
    return it->second;
  } else {
    return it->second;
  }
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::begin() {
  return iterator(tree_.MinNode(tree_.GetRoot()), tree_.GetNil());
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::end() {
  return iterator(tree_.GetNil(), tree_.GetNil());
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::find(const Key &key) {
  return FindByKey(key);
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::FindByKey(const Key &key) {
  iterator it;
  for (it = begin(); it != end(); ++it) {
    if (it->first == key) {
      return it;
    }
  }
  return end();
}

template <typename Key, typename T>
bool map<Key, T>::empty() {
  return begin() == end();
}

template <typename Key, typename T>
typename map<Key, T>::size_type map<Key, T>::size() {
  return tree_.GetSize();
}

template <typename Key, typename T>
typename map<Key, T>::size_type map<Key, T>::max_size() {
  return tree_.GetMaxSize();
}

template <typename Key, typename T>
void map<Key, T>::clear() {
  ClearElements();
  tree_.SetRoot(tree_.GetNil());
}

template <typename Key, typename T>
void map<Key, T>::ClearElements() {
  tree_.ClearNodes(tree_.GetRoot());
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const value_type &value) {
  iterator it = find(value.first);
  bool flag = true;
  if (it == end()) {
    tree_.Insert(value);
    it = find(value.first);
    flag = true;
  } else {
    flag = false;
  }
  return std::make_pair(it, flag);
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const Key &key, const T &obj) {
  return map<Key, T>::insert(std::make_pair(key, obj));
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert_or_assign(
    const Key &key, const T &obj) {
  iterator it = find(key);
  if (it != end()) {
    it->second = obj;
    return std::make_pair(it, false);
  } else {
    return map<Key, T>::insert(key, obj);
  }
}

template <typename Key, typename T>
template <typename... Args>
s21::vector<std::pair<typename map<Key, T>::iterator, bool>>
map<Key, T>::insert_many(Args &&...args) {
  s21::vector<std::pair<iterator, bool>> result_vector;
  std::initializer_list<value_type> init_list = {std::forward<Args>(args)...};

  for (const auto &element : init_list) {
    auto result = insert(element);
    result_vector.push_back(result);
  }

  return result_vector;
}

template <typename Key, typename T>
void map<Key, T>::erase(iterator pos) {
  iterator it = find(pos->first);
  if (it != end()) {
    tree_.RemoveNode(tree_.FindNode(*it));
  }
}

template <typename Key, typename T>
void map<Key, T>::swap(map &other) {
  std::swap(tree_, other.tree_);
}

template <typename Key, typename T>
void map<Key, T>::merge(map &other) {
  iterator it = other.begin();
  while (it != other.end()) {
    iterator next = std::next(it);
    if (insert(*it).second) {
      other.erase(it);
    }
    it = next;
  }
}

template <typename Key, typename T>
bool map<Key, T>::contains(const Key &key) {
  typename map<Key, T>::iterator it = find(key);
  if (it == end()) {
    return false;
  } else {
    return true;
  }
}

}  // namespace s21

#endif  // CPP2_S21_CONTAINERS_1_MAP_H_