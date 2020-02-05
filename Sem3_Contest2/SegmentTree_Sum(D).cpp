/*
Реализуйте структуру данных для хранения элементов и увеличения нескольких подряд идущих элементов на одно и то же число.

 Input format
В первой строке вводится одно натуральное число N(1 ≤ N ≤ 105) - количество чисел в массиве.

Во второй строке вводятся N чисел от 1 до 100000 - элементы массива.

В третьей строке вводится одно натуральное число M(1 ≤ M ≤ 500000) - количество запросов.

Каждая из следующих M строк представляет собой описание запроса. Сначала вводится одна буква,
 кодирующая вид запроса (g - получить текущее значение элемента по номеру, a - увеличить все элементы
 на отрезке). Следом за g вводится одно число - номер элемента. Следом за a вводится три числа - левый
 и правый концы отрезка и число add, на которое нужно увеличить все элементы данного отрезка
 массва(1 ≤ add ≤ 100000).

Output format

Выведите ответы на запросы вида g, разделяя их переводами строк.

Examples
Input	    Output
5             4
2 4 3 5 2     2
5             14
g 2           5
g 5
a 1 3 10
g 2
g 4
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <limits>

////////////////////////////// class SegmentTree (Declaration) //////////////////////////////

template<typename T>
class SegmentTree {
 private:
  std::vector<T> segment_tree_;

  static size_t Log_2(size_t value);
  static size_t PowerOfTwo_Ceil(size_t value);

  T Parent(int64_t idx) const {
    return idx / 2;
  }

  T LeftChild(int64_t idx) const {
    return idx * 2;
  }

  T RightChild(int64_t idx) const {
    return idx * 2 + 1;
  }

 public:
  explicit SegmentTree(const std::vector<T> &values);

  void Update(size_t left, size_t right, const T &value);

  T GetValue(size_t idx) const;
};

////////////////////////////// class SegmentTree (Implementation) //////////////////////////////

template<typename T>
size_t SegmentTree<T>::Log_2(size_t value) {
  size_t result = 0;
  while (value >>= 1u) {
    ++result;
  }
  return result;
}

template<typename T>
size_t SegmentTree<T>::PowerOfTwo_Ceil(size_t value) {
  return 1u << (Log_2(value - 1) + 1);
}

template<typename T>
SegmentTree<T>::SegmentTree(const std::vector<T> &values)
    : segment_tree_(2 * PowerOfTwo_Ceil(values.size()), T(0)) {
  const int64_t half_tree_size = segment_tree_.size() / 2;
  const int64_t values_size = values.size();
  for (int64_t idx = 0; idx < values_size; ++idx) {
    segment_tree_[idx + half_tree_size] = values[idx];
  }
}

template<typename T>
void SegmentTree<T>::Update(size_t left, size_t right, const T &value) {
  const int64_t tree_size = segment_tree_.size();
  left += tree_size / 2;
  right += tree_size / 2;
  while (left <= right) {
    if (left == RightChild(Parent(left))) {
      segment_tree_[left] += value;
    }
    if (right == LeftChild(Parent(right))) {
      segment_tree_[right] += value;
    }
    left = Parent(left + 1);
    right = Parent(right - 1);
  }
}

template<typename T>
T SegmentTree<T>::GetValue(size_t idx) const {
  idx += segment_tree_.size() / 2;
  T value = segment_tree_[idx];
  while ((idx = Parent(idx)) > 0) {
    value += segment_tree_[idx];
  }
  return value;
}

////////////////////////////////////////////////////////////////////////////////

int main() {
  size_t values_num;
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cin >> values_num;
  std::vector<int64_t> values(values_num);
  for (size_t i = 0; i < values_num; ++i) {
    std::cin >> values[i];
  }

  SegmentTree<int64_t> segment_tree(values);
  size_t quires_num;
  std::cin >> quires_num;
  for (size_t i = 0; i < quires_num; ++i) {
    char query;
    std::cin >> query;
    if (query == 'a') {
      size_t left, right;
      int64_t new_value;
      std::cin >> left >> right >> new_value;
      segment_tree.Update(left - 1, right - 1, new_value);
    } else if (query == 'g') {
      size_t idx;
      std::cin >> idx;
      std::cout << segment_tree.GetValue(idx - 1) << std::endl;
    }
  }

  return 0;
}