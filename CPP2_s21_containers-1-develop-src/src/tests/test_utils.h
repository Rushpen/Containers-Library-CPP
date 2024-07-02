#include <gtest/gtest.h>

#include <iostream>
#include <queue>
#include <stack>
#include <string_view>
#include <type_traits>
#include <utility>

#include "../s21_queue/s21_queue.h"
#include "../s21_stack/s21_stack.h"

namespace s21 {
template <typename Iterator, typename = void>
struct HasFirstAndSecond : std::false_type {};

template <typename Iterator>
struct HasFirstAndSecond<Iterator,
                         std::void_t<decltype(std::declval<Iterator>().first),
                                     decltype(std::declval<Iterator>().second)>>
    : std::true_type {};

template <typename T>
struct HasIterator {
 private:
  template <typename U>
  static auto detect(int)
      -> decltype(std::declval<U>().begin(), std::true_type{});

  template <typename>
  static auto detect(...) -> std::false_type;

 public:
  static constexpr bool value{decltype(detect<T>(0))::value};
};

template <typename T, typename U>
void AssertContainerEquality(T &first_container, U &second_container) {
  ASSERT_EQ(first_container.empty(), second_container.empty());
  ASSERT_EQ(first_container.size(), second_container.size());

  if constexpr (HasIterator<T>::value && HasIterator<U>::value) {
    auto first_iter = first_container.begin();
    auto second_iter = second_container.begin();
    while (first_iter != first_container.end()) {
      if constexpr (HasFirstAndSecond<decltype(first_iter)>::value &&
                    HasFirstAndSecond<decltype(second_iter)>::value) {
        ASSERT_EQ(first_iter->first, second_iter->first);
        ASSERT_EQ(first_iter->second, second_iter->second);
      } else {
        ASSERT_EQ(*first_iter, *second_iter);
      }
      ++first_iter;
      ++second_iter;
    }
  }
}

}  // namespace s21