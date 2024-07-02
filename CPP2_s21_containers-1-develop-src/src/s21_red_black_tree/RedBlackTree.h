#ifndef CPP2_S21_CONTAINERS_1_RED_BLACK_TREE_H_
#define CPP2_S21_CONTAINERS_1_RED_BLACK_TREE_H_

#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>

#include "Node.h"

namespace s21 {

template <typename T>
class RedBlackTree {
 public:
  template <bool IsConst>
  class RedBlackTreeIteratorBase {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = std::conditional_t<IsConst, const T *, T *>;
    using reference = std::conditional_t<IsConst, const T &, T &>;
    using RedBlackTreeIterator = RedBlackTreeIteratorBase<false>;
    using RedBlackTreeConstIterator = RedBlackTreeIteratorBase<true>;

   public:
    RedBlackTreeIteratorBase() = default;
    explicit RedBlackTreeIteratorBase(Node<T> *node, Node<T> *nil)
        : current_node_(node), nil_(nil){};

   public:
    reference operator*() const;
    pointer operator->() const;

    RedBlackTreeIteratorBase &operator++();
    RedBlackTreeIteratorBase &operator--();
    RedBlackTreeIteratorBase operator++(int);
    RedBlackTreeIteratorBase operator--(int);

    bool operator==(const RedBlackTreeIteratorBase &other) const;
    bool operator!=(const RedBlackTreeIteratorBase &other) const;

   private:
    Node<T> *current_node_;
    Node<T> *nil_;
  };

 public:
  using iterator = RedBlackTreeIteratorBase<false>;
  using const_iterator = RedBlackTreeIteratorBase<true>;

  RedBlackTree();
  RedBlackTree(std::initializer_list<T> const &items);
  RedBlackTree(const RedBlackTree &other);
  RedBlackTree(RedBlackTree &&other) noexcept;
  ~RedBlackTree();

  RedBlackTree &operator=(const RedBlackTree &other);
  RedBlackTree &operator=(RedBlackTree &&other) noexcept;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  Node<T> *GetRoot() const { return root_; }
  Node<T> *GetNil() const { return nil_; }
  void SetRoot(Node<T> *node) { root_ = node; }

  Node<T> *MinNode(Node<T> *node) const;
  Node<T> *MaxNode(Node<T> *node) const;
  Node<T> *FindNode(const T &key) const;

  void Insert(const T &data);

  void RemoveNode(Node<T> *z);
  void ClearNodes(Node<T> *node);

  size_t GetSize() const;
  size_t GetMaxSize() const;

 private:
  Node<T> *root_;
  Node<T> *nil_;

  void RotateLeft(Node<T> *node);
  void RotateRight(Node<T> *node);
  void FixInsert(Node<T> *&node);

  void Transplant(Node<T> *u, Node<T> *v);
  void FixDelete(Node<T> *x);

  Node<T> *CopyTree(Node<T> *node, Node<T> *other_nil);

  size_t GetSize(Node<T> *node) const;

 public:
  iterator my_end();
  const_iterator my_end() const;
  // For working tests
  // void PrintTree();
  // void PrintHelper(Node<T> *node, std::string indent, bool last);
};

//////////////////////////////////////////////////////

// For RedBlackTreeIterator
template <typename T>
template <bool IsConst>
typename RedBlackTree<T>::template RedBlackTreeIteratorBase<IsConst>::reference
RedBlackTree<T>::RedBlackTreeIteratorBase<IsConst>::operator*() const {
  return current_node_->data_;
}

template <typename T>
template <bool IsConst>
typename RedBlackTree<T>::template RedBlackTreeIteratorBase<IsConst>::pointer
RedBlackTree<T>::RedBlackTreeIteratorBase<IsConst>::operator->() const {
  return &current_node_->data_;
}

template <typename T>
template <bool IsConst>
typename RedBlackTree<T>::template RedBlackTreeIteratorBase<IsConst>
    &RedBlackTree<T>::RedBlackTreeIteratorBase<IsConst>::operator++() {
  if (current_node_ == nullptr || current_node_ == nil_ || nil_ == nullptr) {
    return *this;
  }
  if (current_node_->right_ != nil_) {
    current_node_ = current_node_->right_;
    while (current_node_->left_ != nil_) {
      current_node_ = current_node_->left_;
    }
  } else {
    while (current_node_->parent_ != nullptr &&
           current_node_ == current_node_->parent_->right_) {
      current_node_ = current_node_->parent_;
    }
    if (current_node_->parent_ != nullptr) {
      current_node_ = current_node_->parent_;
    } else {
      current_node_ = nil_;
    }
  }
  return *this;
}

template <typename T>
template <bool IsConst>
typename RedBlackTree<T>::template RedBlackTreeIteratorBase<IsConst>
    &RedBlackTree<T>::RedBlackTreeIteratorBase<IsConst>::operator--() {
  if (current_node_ == nil_) {
    current_node_ = nil_->parent_;
    if (current_node_ == nullptr) {
      return *this;
    }
  }
  if (current_node_->left_ != nil_) {
    current_node_ = current_node_->left_;
    while (current_node_->right_ != nil_) {
      current_node_ = current_node_->right_;
    }
  } else {
    while (current_node_->parent_ != nullptr &&
           current_node_ == current_node_->parent_->left_) {
      current_node_ = current_node_->parent_;
    }
    current_node_ = current_node_->parent_;
  }
  return *this;
}

template <typename T>
template <bool IsConst>
typename RedBlackTree<T>::template RedBlackTreeIteratorBase<IsConst>
RedBlackTree<T>::RedBlackTreeIteratorBase<IsConst>::operator++(int) {
  RedBlackTreeIteratorBase temp = *this;
  ++(*this);
  return temp;
}

template <typename T>
template <bool IsConst>
typename RedBlackTree<T>::template RedBlackTreeIteratorBase<IsConst>
RedBlackTree<T>::RedBlackTreeIteratorBase<IsConst>::operator--(int) {
  RedBlackTreeIteratorBase temp = *this;
  --(*this);
  return temp;
}

template <typename T>
template <bool IsConst>
bool RedBlackTree<T>::RedBlackTreeIteratorBase<IsConst>::operator==(
    const RedBlackTreeIteratorBase &other) const {
  return current_node_ == other.current_node_;
}

template <typename T>
template <bool IsConst>
bool RedBlackTree<T>::RedBlackTreeIteratorBase<IsConst>::operator!=(
    const RedBlackTreeIteratorBase &other) const {
  return current_node_ != other.current_node_;
}
//////////////////////////////////////////////////////

// For RedBlackTree

template <typename T>
RedBlackTree<T>::RedBlackTree() {
  nil_ = new Node<T>();
  nil_->color_ = Color::kBlack;
  root_ = nil_;
}

template <typename T>
RedBlackTree<T>::RedBlackTree(std::initializer_list<T> const &items) {
  for (const auto &item : items) {
    Insert(item);
  }
}

template <typename T>
RedBlackTree<T>::RedBlackTree(const RedBlackTree &other) {
  nil_ = new Node<T>{};
  nil_->color_ = Color::kBlack;
  root_ = CopyTree(other.root_, other.nil_);
}

template <typename T>
RedBlackTree<T>::RedBlackTree(RedBlackTree &&other) noexcept
    : root_(other.root_), nil_(other.nil_) {
  other.root_ = nullptr;
  other.nil_ = nullptr;
}

template <typename T>
RedBlackTree<T>::~RedBlackTree() {
  ClearNodes(root_);
  delete nil_;
  nil_ = nullptr;
  root_ = nil_;
}

template <typename T>
RedBlackTree<T> &RedBlackTree<T>::operator=(const RedBlackTree &other) {
  if (this != &other) {
    ClearNodes(root_);
    const_iterator other_iter = other.begin();
    while (other_iter != other.end()) {
      Insert(*other_iter);
      ++other_iter;
    }
  }
  return *this;
}

template <typename T>
RedBlackTree<T> &RedBlackTree<T>::operator=(RedBlackTree<T> &&other) noexcept {
  if (this != &other) {
    root_ = other.root_;
    delete nil_; /* Need to free already allocated for nil_ memory */
    nil_ = other.nil_;
    other.nil_ = new Node<T>{};
    other.root_ = other.nil_;
  }
  return *this;
}

////////////iterators for RedBlackTree////////////////
template <typename T>
typename RedBlackTree<T>::iterator RedBlackTree<T>::begin() {
  return iterator(MinNode(root_), nil_);
}

template <typename T>
typename RedBlackTree<T>::iterator RedBlackTree<T>::end() {
  return iterator(nil_, nil_);
}

template <typename T>
typename RedBlackTree<T>::const_iterator RedBlackTree<T>::begin() const {
  return const_iterator(MinNode(root_), nil_);
}

template <typename T>
typename RedBlackTree<T>::const_iterator RedBlackTree<T>::end() const {
  return const_iterator(nil_, nil_);
}

template <typename T>
typename RedBlackTree<T>::iterator RedBlackTree<T>::my_end() {
  return iterator(MaxNode(root_), nil_);
}

template <typename T>
typename RedBlackTree<T>::const_iterator RedBlackTree<T>::my_end() const {
  return const_iterator(MaxNode(root_), nil_);
}

template <typename T>
Node<T> *RedBlackTree<T>::MinNode(Node<T> *node) const {
  while (node->left_ != nil_ && node->left_ != nullptr) {
    node = node->left_;
  }
  return node;
}

template <typename T>
Node<T> *RedBlackTree<T>::MaxNode(Node<T> *node) const {
  while (node->right_ != nil_ && node->right_ != nullptr) {
    node = node->right_;
  }
  return node;
}

template <typename T>
Node<T> *RedBlackTree<T>::FindNode(const T &key) const {
  Node<T> *current = root_;
  while (current != nil_) {
    if (key == current->data_) {
      return current;
    } else if (key < current->data_) {
      current = current->left_;
    } else {
      current = current->right_;
    }
  }
  return nullptr;
}
///////////////////////////////////////////

////////////////////////////////////////////////////////
// For private
template <typename T>
void RedBlackTree<T>::ClearNodes(Node<T> *node) {
  if (node != nil_) {
    ClearNodes(node->left_);
    ClearNodes(node->right_);
    delete node;
  }
}

template <typename T>
Node<T> *RedBlackTree<T>::CopyTree(Node<T> *node, Node<T> *other_nil) {
  if (node == other_nil) {
    return nil_;
  }

  Node<T> *new_node = new Node<T>{node->data_};
  new_node->color_ = node->color_;
  new_node->left_ = CopyTree(node->left_, other_nil);
  new_node->right_ = CopyTree(node->right_, other_nil);

  if (new_node->left_ != nil_) {
    new_node->left_->parent_ = new_node;
  }
  if (new_node->right_ != nil_) {
    new_node->right_->parent_ = new_node;
  }

  return new_node;
}

template <typename T>
void RedBlackTree<T>::RotateLeft(Node<T> *node) {
  Node<T> *y = node->right_;
  node->right_ = y->left_;
  if (y->left_ != nil_) {
    y->left_->parent_ = node;
  }
  y->parent_ = node->parent_;
  if (node->parent_ == nullptr) {
    root_ = y;
  } else if (node == node->parent_->left_) {
    node->parent_->left_ = y;
  } else {
    node->parent_->right_ = y;
  }
  y->left_ = node;
  node->parent_ = y;
}

template <typename T>
void RedBlackTree<T>::RotateRight(Node<T> *node) {
  Node<T> *x = node->left_;
  node->left_ = x->right_;

  if (x->right_ != nil_) {
    x->right_->parent_ = node;
  }
  x->parent_ = node->parent_;

  if (node->parent_ == nullptr) {
    root_ = x;
  } else if (node == node->parent_->left_) {
    node->parent_->left_ = x;
  } else {
    node->parent_->right_ = x;
  }

  x->right_ = node;
  node->parent_ = x;
}

template <typename T>
void RedBlackTree<T>::FixInsert(Node<T> *&k) {
  Node<T> *u;
  while (k->parent_->color_ == Color::kRed) {
    if (k->parent_ == k->parent_->parent_->left_) {
      u = k->parent_->parent_->right_;
      if (u->color_ == Color::kRed) {
        k->parent_->color_ = Color::kBlack;
        u->color_ = Color::kBlack;
        k->parent_->parent_->color_ = Color::kRed;
        k = k->parent_->parent_;
      } else {
        if (k == k->parent_->right_) {
          k = k->parent_;
          RotateLeft(k);
        }
        k->parent_->color_ = Color::kBlack;
        k->parent_->parent_->color_ = Color::kRed;
        RotateRight(k->parent_->parent_);
      }
    } else {
      u = k->parent_->parent_->left_;
      if (u->color_ == Color::kRed) {
        k->parent_->color_ = Color::kBlack;
        u->color_ = Color::kBlack;
        k->parent_->parent_->color_ = Color::kRed;
        k = k->parent_->parent_;
      } else {
        if (k == k->parent_->left_) {
          k = k->parent_;
          RotateRight(k);
        }
        k->parent_->color_ = Color::kBlack;
        k->parent_->parent_->color_ = Color::kRed;
        RotateLeft(k->parent_->parent_);
      }
    }
    if (k == root_) {
      break;
    }
  }
  root_->color_ = Color::kBlack;
}

// For public
template <typename T>
void RedBlackTree<T>::Insert(const T &data) {
  Node<T> *node = new Node<T>(data);
  node->left_ = nil_;
  node->right_ = nil_;
  node->color_ = Color::kRed;

  Node<T> *y = nullptr;
  Node<T> *root = root_;

  while (root != nil_) {
    y = root;
    if (node->data_ < root->data_) {
      root = root->left_;
    } else {
      root = root->right_;
    }
  }

  node->parent_ = y;

  if (y == nullptr) {
    root_ = node;
  } else if (node->data_ < y->data_) {
    y->left_ = node;
  } else {
    y->right_ = node;
  }

  if (node->parent_ == nullptr) {
    node->color_ = Color::kRed;
    return;
  }

  if (node->parent_->parent_ == nullptr) {
    return;
  }

  FixInsert(node);
}

template <typename T>
void RedBlackTree<T>::RemoveNode(Node<T> *z) {
  Node<T> *y = z;
  Node<T> *x;
  Color y_original_color = y->color_;

  if (z->left_ == nil_) {
    x = z->right_;
    Transplant(z, z->right_);
  } else if (z->right_ == nil_) {
    x = z->left_;
    Transplant(z, z->left_);
  } else {
    y = MinNode(z->right_);
    y_original_color = y->color_;
    x = y->right_;
    if (y->parent_ == z) {
      x->parent_ = y;
    } else {
      Transplant(y, y->right_);
      y->right_ = z->right_;
      y->right_->parent_ = y;
    }
    Transplant(z, y);
    y->left_ = z->left_;
    y->left_->parent_ = y;
    y->color_ = z->color_;
  }

  delete z;

  if (y_original_color == Color::kBlack) {
    FixDelete(x);
  }
}

template <typename T>
void RedBlackTree<T>::Transplant(Node<T> *u, Node<T> *v) {
  if (u->parent_ == nullptr) {
    root_ = v;
  } else if (u == u->parent_->left_) {
    u->parent_->left_ = v;
  } else {
    u->parent_->right_ = v;
  }
  v->parent_ = u->parent_;
}

template <typename T>
void RedBlackTree<T>::FixDelete(Node<T> *x) {
  while (x != root_ && x->color_ == Color::kBlack) {
    if (x == x->parent_->left_) {
      Node<T> *w = x->parent_->right_;
      if (w->color_ == Color::kRed) {
        w->color_ = Color::kBlack;
        x->parent_->color_ = Color::kRed;
        RotateLeft(x->parent_);
        w = x->parent_->right_;
      }
      if (w->left_->color_ == Color::kBlack &&
          w->right_->color_ == Color::kBlack) {
        w->color_ = Color::kRed;
        x = x->parent_;
      } else {
        if (w->right_->color_ == Color::kBlack) {
          w->left_->color_ = Color::kBlack;
          w->color_ = Color::kRed;
          RotateRight(w);
          w = x->parent_->right_;
        }
        w->color_ = x->parent_->color_;
        x->parent_->color_ = Color::kBlack;
        w->right_->color_ = Color::kBlack;
        RotateLeft(x->parent_);
        x = root_;
      }
    } else {
      Node<T> *w = x->parent_->left_;
      if (w->color_ == Color::kRed) {
        w->color_ = Color::kBlack;
        x->parent_->color_ = Color::kRed;
        RotateRight(x->parent_);
        w = x->parent_->left_;
      }
      if (w->right_->color_ == Color::kBlack &&
          w->left_->color_ == Color::kBlack) {
        w->color_ = Color::kRed;
        x = x->parent_;
      } else {
        if (w->left_->color_ == Color::kBlack) {
          w->right_->color_ = Color::kBlack;
          w->color_ = Color::kRed;
          RotateLeft(w);
          w = x->parent_->left_;
        }
        w->color_ = x->parent_->color_;
        x->parent_->color_ = Color::kBlack;
        w->left_->color_ = Color::kBlack;
        RotateRight(x->parent_);
        x = root_;
      }
    }
  }
  x->color_ = Color::kBlack;
}

template <typename T>
size_t RedBlackTree<T>::GetSize() const {
  return GetSize(root_);
}

template <typename T>
size_t RedBlackTree<T>::GetSize(Node<T> *node) const {
  if (node == nullptr || node == nil_) {
    return 0;
  }
  return 1 + GetSize(node->left_) + GetSize(node->right_);
}

template <typename T>
size_t RedBlackTree<T>::GetMaxSize() const {
  return std::numeric_limits<size_t>::max() / sizeof(Node<T>);
}

// template <typename T>
// void RedBlackTree<T>::PrintTree() {
//   if (root_) {
//     PrintHelper(root_, "", true);
//   }
// }

// template <typename T>
// void RedBlackTree<T>::PrintHelper(Node<T> *root, std::string indent,
//                                   bool last) {
//   if (root != nil_) {
//     std::cout << indent;
//     if (last) {
//       std::cout << "R----";
//       indent += "   ";
//     } else {
//       std::cout << "L----";
//       indent += "|  ";
//     }

//     std::string sColor = root->color_ == Color::kRed ? "RED" : "BLACK";
//     std::cout << root->data_ << "(" << sColor << ")" << std::endl;
//     PrintHelper(root->left_, indent, false);
//     PrintHelper(root->right_, indent, true);
//   }
// }

}  // namespace s21

#endif  // CPP2_S21_CONTAINERS_1_RED_BLACK_TREE_H_