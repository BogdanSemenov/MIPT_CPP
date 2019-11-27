#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <limits>
#include <string>

enum Bracket {
  OPEN_BRACKET = '(',
  CLOSE_BRACKET = ')'
};

////////////////////////////// class SegmentTree (Declaration) //////////////////////////////

template<typename T>
class SegmentTree {
 private:
  struct Node {
    size_t free_left_brackets_num_;
    size_t free_right_brackets_num_;
    size_t max_length_;
    static constexpr size_t pending_value_ = 0;

    Node();

    Node(const Node &first, const Node &second);

    Node &operator=(const char &value);
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

  struct MakeBalancedSequence_ {
    Node operator()(const Node &lhs, const Node &rhs) const;
  };

  template<typename BinaryOperation>
  Node Query(size_t left, size_t right, const BinaryOperation &binary_operation) const;

 public:
  explicit SegmentTree(const std::string &input);

  size_t BalancedSequenceMaxLength(size_t left, size_t right) const;
};

////////////////////////////// class SegmentTree (Implementation) //////////////////////////////

template<typename T>
SegmentTree<T>::Node::Node() {
  free_left_brackets_num_ = pending_value_;
  free_right_brackets_num_ = pending_value_;
  max_length_ = pending_value_;
}

template<typename T>
SegmentTree<T>::Node::Node(const Node &first, const Node &second) {
  Node node = MakeBalancedSequence_()(first, second);
  free_left_brackets_num_ = node.free_left_brackets_num_;
  free_right_brackets_num_ = node.free_right_brackets_num_;
  max_length_ = node.max_length_;
}

template<typename T>
typename SegmentTree<T>::Node &SegmentTree<T>::Node::operator=(const char &value) {
  if (value == OPEN_BRACKET) {
    free_left_brackets_num_ = 1;
    free_right_brackets_num_ = 0;
  } else {
    free_right_brackets_num_ = 1;
    free_left_brackets_num_ = 0;
  }
  max_length_ = 0;
  return *this;
}

template<typename T>
template<typename BinaryOperation>
typename SegmentTree<T>::Node SegmentTree<T>::Query(size_t left, size_t right,
                                                    const BinaryOperation &binary_operation) const {
  Node left_answer;
  Node right_answer;
  const size_t tree_size = segment_tree_.size();
  left += tree_size / 2;
  right += tree_size / 2;
  while (left <= right) {
    if (left == RightChild(Parent(left))) {
      left_answer = binary_operation(left_answer, segment_tree_[left]);
    }
    if (right == LeftChild(Parent(right))) {
      right_answer = binary_operation(segment_tree_[right], right_answer);
    }
    left = Parent(left + 1);
    right = Parent(right - 1);
  }
  return binary_operation(left_answer, right_answer);
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
typename SegmentTree<T>::Node SegmentTree<T>::MakeBalancedSequence_::operator()(const Node &lhs, const Node &rhs) const {
  Node node;
  size_t delta = std::min(lhs.free_left_brackets_num_, rhs.free_right_brackets_num_);
  node.max_length_ = lhs.max_length_ + rhs.max_length_ + 2 * delta;
  node.free_left_brackets_num_ = lhs.free_left_brackets_num_ + rhs.free_left_brackets_num_ - delta;
  node.free_right_brackets_num_ = lhs.free_right_brackets_num_ + rhs.free_right_brackets_num_ - delta;
  return node;
}

template<typename T>
SegmentTree<T>::SegmentTree(const std::string &input)
    : segment_tree_(2 * PowerOfTwo_Ceil(input.size())) {
  const size_t half_tree_size = segment_tree_.size() / 2;
  const size_t values_size = input.size();
  for (size_t idx = values_size; idx < PowerOfTwo_Ceil(input.size()); ++idx) {
    segment_tree_[idx + half_tree_size] = Node();
  }
  for (size_t idx = 0; idx < values_size; ++idx) {
    segment_tree_[idx + half_tree_size] = input[idx];
  }
  for (size_t idx = half_tree_size - 1; idx > 0; --idx) {
    segment_tree_[idx] = Node(segment_tree_[LeftChild(idx)], segment_tree_[RightChild(idx)]);
  }
}

template<typename T>
size_t SegmentTree<T>::BalancedSequenceMaxLength(size_t left, size_t right) const {
  return Query(left, right, MakeBalancedSequence_()).max_length_;
}

////////////////////////////////////////////////////////////////////////////////

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::string sequence;
  std::cin >> sequence;
  SegmentTree<int64_t> segment_tree(sequence);
  size_t query_num;
  std::cin >> query_num;
  for (size_t i = 0; i < query_num; ++i) {
    size_t left, right;
    std::cin >> left >> right;
    std::cout << segment_tree.BalancedSequenceMaxLength(left - 1, right - 1) << std::endl;
  }

  return 0;
}