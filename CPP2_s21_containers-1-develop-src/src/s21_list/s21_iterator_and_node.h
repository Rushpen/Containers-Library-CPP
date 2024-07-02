#ifndef S21_CPP2_S21_CONTAINERS_1_S21_LIST_ITERATOR_AND_NODE_H_
#define S21_CPP2_S21_CONTAINERS_1_S21_LIST_ITERATOR_AND_NODE_H_

template <typename T>
class node {
 public:
  using value_type = T;
  node *prev_node;
  node *next_node;
  value_type data;

  node(value_type data = value_type(), node *prev_node = nullptr,
       node *next_node = nullptr) {
    this->data = data;
    this->prev_node = prev_node;
    this->next_node = next_node;
  }
};

template <typename T>
class listConstIterator {
 public:
  using value_type = T;
  using const_reference = const T &;

  const node<value_type> *_current;
  listConstIterator(const node<value_type> *ptr = nullptr) { _current = ptr; }

  const_reference operator*() const {
    if (_current == nullptr) {
      throw std::out_of_range("value_typendex out of range");
    }
    return _current->data;
  }

  listConstIterator &operator++() {
    if (_current == nullptr) {
      throw std::out_of_range("value_typendex out of range");
    }
    _current = _current->next_node;
    return *this;
  }

  listConstIterator &operator--() {
    if (_current == nullptr) {
      throw std::out_of_range("value_typendex out of range");
    }
    _current = _current->prev_node;
    return *this;
  }

  listConstIterator operator++(int) {
    if (_current == nullptr) {
      throw std::out_of_range("value_typendex out of range");
    }
    auto tmp = *this;
    _current = _current->next_node;
    return tmp;
  }

  listConstIterator operator--(int) {
    if (_current == nullptr) {
      throw std::out_of_range("value_typendex out of range");
    }
    auto tmp = *this;
    _current = _current->prev_node;
    return tmp;
  }

  listConstIterator<value_type> operator=(listConstIterator<value_type> *l) {
    _current = l;
    return _current;
  }

  bool operator==(const listConstIterator<value_type> l) {
    return _current == l._current;
  }

  bool operator!=(const listConstIterator<value_type> l) {
    return !(!(*this == l));
  }

  const node<value_type> *get() const { return _current; }
};

template <typename T>
class listIterator : public listConstIterator<T> {
  using value_type = T;
  using reference = T &;

 public:
  listIterator(node<value_type> *ptr = nullptr)
      : listConstIterator<value_type>{ptr} {}
  reference operator*() {
    return const_cast<reference>(listConstIterator<value_type>::operator*());
  }

  listIterator &operator++() {
    listConstIterator<value_type>::operator++();
    return *this;
  }

  listIterator &operator--() {
    listConstIterator<value_type>::operator--();
    return *this;
  }

  listIterator operator++(int) {
    auto tmp = listConstIterator<value_type>::operator++(0);
    return listIterator{const_cast<node<value_type> *>(tmp.get())};
  }

  listIterator operator--(int) {
    auto tmp = listConstIterator<value_type>::operator--(0);
    return listIterator{const_cast<node<value_type> *>(tmp.get())};
  }

  listIterator<value_type> operator=(listIterator<value_type> *l) {
    return listConstIterator<value_type>::operator=(l);
    ;
  }
};

#endif  // S21_CPP2_S21_CONTAINERS_1_S21_LIST_ITERATOR_AND_NODE_H_
