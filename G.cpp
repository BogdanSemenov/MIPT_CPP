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
    T min_;
    T sum_;
    static constexpr T min_pending_value_ = std::numeric_limits<T>::max();
    static constexpr T sum_pending_value_ = T(0);

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

  struct Summary_ {
    T operator()(const Node &lhs, const Node &rhs) const;

    T operator()(const T &value, const Node &node) const;
  };

  struct Min_ {
    T operator()(const Node &lhs, const Node &rhs) const;

    T operator()(const T &value, const Node &node) const;
  };

  template<typename BinaryOperation>
  T Query(size_t left, size_t right, const BinaryOperation &binary_operation, const T &pending_value) const;

 public:
  explicit SegmentTree(const std::vector<T> &values);

  void Update(size_t idx, const T &value);

  T RSQ(size_t left, size_t right) const;

  T RMQ(size_t left, size_t right) const;
};

////////////////////////////// class SegmentTree (Implementation) //////////////////////////////

template<typename T>
SegmentTree<T>::Node::Node() {
  min_ = min_pending_value_;
  sum_ = sum_pending_value_;
}

template<typename T>
SegmentTree<T>::Node::Node(const Node &first, const Node &second) {
  sum_ = Summary_()(first, second);
  min_ = Min_()(first, second);
}

template<typename T>
typename SegmentTree<T>::Node &SegmentTree<T>::Node::operator=(const T &value) {
  min_ = value;
  sum_ = value;
  return *this;
}

template<typename T>
template<typename BinaryOperation>
T SegmentTree<T>::Query(size_t left, size_t right, const BinaryOperation &binary_operation,
                        const T &pending_value) const {
  T answer = pending_value;
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
T SegmentTree<T>::Summary_::operator()(const Node &lhs, const Node &rhs) const {
  return lhs.sum_ + rhs.sum_;
}

template<typename T>
T SegmentTree<T>::Summary_::operator()(const T &value, const Node &node) const {
  return value + node.sum_;
}

template<typename T>
T SegmentTree<T>::Min_::operator()(const Node &lhs, const Node &rhs) const {
  return std::min(lhs.min_, rhs.min_);
}

template<typename T>
T SegmentTree<T>::Min_::operator()(const T &value, const Node &node) const {
  return std::min(value, node.min_);
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
void SegmentTree<T>::Update(size_t idx, const T &value) {
  idx += segment_tree_.size() / 2;
  segment_tree_[idx] = value;
  while ((idx = Parent(idx)) > 0) {
    segment_tree_[idx] = Node(segment_tree_[LeftChild(idx)], segment_tree_[RightChild(idx)]);
  }
}

template<typename T>
T SegmentTree<T>::RSQ(size_t left, size_t right) const {
  return Query(left, right, Summary_(), Node::sum_pending_value_);
}

template<typename T>
T SegmentTree<T>::RMQ(size_t left, size_t right) const {
  return Query(left, right, Min_(), Node::min_pending_value_);
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
    char query;
    std::cin >> query;
    if (query == 's') {
      size_t left, right;
      std::cin >> left >> right;
      std::cout << segment_tree.RSQ(left - 1, right - 1) << ' ';
    } else if (query == 'u') {
      size_t idx;
      int64_t value;
      std::cin >> idx >> value;
      segment_tree.Update(idx - 1, value);
    }
  }

  return 0;
}