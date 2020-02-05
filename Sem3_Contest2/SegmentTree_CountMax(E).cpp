/*
 Реализуйте структуру данных для эффективного вычисления значения максимального из нескольких подряд идущих элементов
 массива, а также количества элементов, равных максимальному на данном отрезке.

Input format
В первой строке вводится одно натуральное число N(1 ≤ N ≤ 105) - количество чисел в массиве.

Во второй строке вводятся N чисел от 1 до 100000 - элементы массива.

В третьей строке вводится одно натуральное число K(1 ≤ K ≤ 30000) — количество запросов на вычисление максимума.

В следующих K строках вводится по два числа - номера левого и правого элементов отрезка массива
 (считается, что элементы массива нумеруются с единицы).

Output format
Для каждого запроса выведите в отдельной строке через пробел значение максимального элемента на указанном
 отрезке массива и количество максимальных элементов на этом отрезке.

Examples
Input	     Output
5             2 2
2 2 2 1 5     5 1
2
2 3
2 5
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
  struct Node {
    T max_;
    size_t max_count_;
    static constexpr T pending_value_ = T(0);

    Node();

    Node(const Node &first, const Node &second);

    Node &operator=(const T &value);
  };

  std::vector<Node> segment_tree_;

  static size_t Log_2(size_t value);
  static size_t PowerOfTwo_Ceil(size_t value);

  T Parent(size_t idx) const {
    return idx / 2;
  }

  T LeftChild(size_t idx) const {
    return idx * 2;
  }

  T RightChild(size_t idx) const {
    return idx * 2 + 1;
  }

  struct Max_ {
    Node operator()(const Node &lhs, const Node &rhs) const;
  };

  template<typename BinaryOperation>
  Node Query(size_t left, size_t right, const BinaryOperation &binary_operation) const;

 public:
  explicit SegmentTree(const std::vector<T> &values);

  Node RmaxQ(size_t left, size_t right) const;
};

////////////////////////////// class SegmentTree (Implementation) //////////////////////////////

template<typename T>
SegmentTree<T>::Node::Node() {
  max_ = pending_value_;
  max_count_ = pending_value_;
}

template<typename T>
SegmentTree<T>::Node::Node(const Node &first, const Node &second) {
  Node node = Max_()(first, second);
  max_ = node.max_;
  max_count_ = node.max_count_;
}

template<typename T>
typename SegmentTree<T>::Node &SegmentTree<T>::Node::operator=(const T &value) {
  max_ = value;
  max_count_ = 1;
  return *this;
}

template<typename T>
template<typename BinaryOperation>
typename SegmentTree<T>::Node SegmentTree<T>::Query(size_t left, size_t right, const BinaryOperation &binary_operation) const {
  Node answer = Node();
  const size_t tree_size = segment_tree_.size();
  left += tree_size / 2;
  right += tree_size / 2;
  while (left <= right) {
    if (left == RightChild(Parent(left))) {
      answer = binary_operation(answer, segment_tree_[left]);
    }
    if (right == LeftChild(Parent(right))) {
      answer = binary_operation(answer, segment_tree_[right]);
    }
    left = Parent(left + 1);
    right = Parent(right - 1);
  }
  return answer;
}

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
typename SegmentTree<T>::Node SegmentTree<T>::Max_::operator()(const Node &lhs, const Node &rhs) const {
  Node node;
  node.max_ = std::max(lhs.max_, rhs.max_);
  if (lhs.max_ == rhs.max_) {
    node.max_count_ = lhs.max_count_ + rhs.max_count_;
  } else {
    node.max_count_ = (lhs.max_ > rhs.max_) ? lhs.max_count_ : rhs.max_count_;
  }
  return node;
}

template<typename T>
SegmentTree<T>::SegmentTree(const std::vector<T> &values)
    : segment_tree_(2 * PowerOfTwo_Ceil(values.size())) {
  const size_t half_tree_size = segment_tree_.size() / 2;
  const size_t values_size = values.size();
  for (size_t idx = values_size; idx < PowerOfTwo_Ceil(values.size()); ++idx) {
    segment_tree_[idx + half_tree_size] = Node();
  }
  for (size_t idx = 0; idx < values_size; ++idx) {
    segment_tree_[idx + half_tree_size] = values[idx];
  }
  for (size_t idx = half_tree_size - 1; idx > 0; --idx) {
    segment_tree_[idx] = Node(segment_tree_[LeftChild(idx)], segment_tree_[RightChild(idx)]);
  }
}

template<typename T>
typename SegmentTree<T>::Node SegmentTree<T>::RmaxQ(size_t left, size_t right) const {
  return Query(left, right, Max_());
}

////////////////////////////////////////////////////////////////////////////////

int main() {
  size_t values_num;
  std::cin >> values_num;
  std::vector<int64_t> values(values_num);
  for (size_t i = 0; i < values_num; ++i) {
    std::cin >> values[i];
  }

  SegmentTree<int64_t> segment_tree(values);
  size_t quires_num;
  std::cin >> quires_num;
  for (size_t i = 0; i < quires_num; ++i) {
    size_t left, right;
    std::cin >> left >> right;
    auto node = segment_tree.RmaxQ(left - 1, right - 1);
    std::cout << node.max_ << ' ' << node.max_count_ << std::endl;
  }

  return 0;
}