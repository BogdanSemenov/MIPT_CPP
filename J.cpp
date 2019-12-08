#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <memory>
#include <ctime>
#include <string>

template<typename KeyType, typename PriorityType>
struct Node {
  KeyType key;
  PriorityType priority;
  size_t num;

  bool operator<(const Node &node) const {
    return key < node.key;
  }
};

////////////////////////////// class Treap (Declaration) //////////////////////////////

template<typename KeyType, typename PriorityType>
class Treap {
 private:
  struct TreapNode;
  std::shared_ptr<TreapNode> root_;
  struct TreapNodeInfo {
    size_t parent_num;
    size_t left_child_num;
    size_t right_child_num;
  };

  bool FindKey(const KeyType &key, std::shared_ptr<TreapNode> treap_node) const;

  static std::pair<std::shared_ptr<TreapNode>, std::shared_ptr<TreapNode>> Split(std::shared_ptr<TreapNode> node,
                                                                                 const KeyType &key);

  static std::shared_ptr<TreapNode> Merge(std::shared_ptr<TreapNode> left_node,
                                          std::shared_ptr<TreapNode> right_node);

  void GetTreapNodeInfo(std::shared_ptr<TreapNode> node, std::vector<TreapNodeInfo> &treap_nodes_info) const;

 public:
  Treap() = default;

  template<typename Iter>
  Treap(Iter begin, Iter end);

  bool FindKey(const KeyType &key) const;

  void Insert(const KeyType &key, const PriorityType &priority, size_t num);

  void InsertSortedOrder(const KeyType &key, const PriorityType &priority, size_t num,
                         std::shared_ptr<TreapNode> &last_pushed);

  void PrintTreap(size_t num) const;
};

////////////////////////////// class TreapNode //////////////////////////////

template<typename KeyType, typename PriorityType>
struct Treap<KeyType, PriorityType>::TreapNode : public std::enable_shared_from_this<TreapNode> {
  KeyType key_;

  PriorityType priority_;

  size_t num_;

  std::shared_ptr<TreapNode> left_child_;

  std::shared_ptr<TreapNode> right_child_;

  std::weak_ptr<TreapNode> parent_;

  explicit TreapNode() = default;

  explicit TreapNode(const KeyType &key, const PriorityType &priority, size_t num)
      : key_(key), priority_(priority), num_(num) {}

  enum class ChildType {
    LeftChild,
    RightChild
  };

  void SetChild(std::shared_ptr<TreapNode> child, const ChildType &child_type) {
    if (child_type == ChildType::LeftChild) {
      left_child_ = child;
    } else {
      right_child_ = child;
    }
    if (child != nullptr) {
      child->parent_ = this->shared_from_this();
    }
  }
};

////////////////////////////// class Treap (Implementation) //////////////////////////////
template<typename KeyType, typename PriorityType>
template<typename Iter>
Treap<KeyType, PriorityType>::Treap(Iter begin, Iter end) : Treap() {
  std::shared_ptr<TreapNode> last_pushed = nullptr;
  if (!std::is_sorted(begin, end)) {
    for (auto iter = begin; iter != end; ++iter) {
      Insert(iter->key, iter->priority, iter->num);
    }
  } else {
    for (auto iter = begin; iter != end; ++iter) {
      InsertSortedOrder(iter->key, iter->priority, iter->num, last_pushed);
    }
  }
}

template<typename KeyType, typename PriorityType>
std::shared_ptr<typename Treap<KeyType, PriorityType>::TreapNode>
Treap<KeyType, PriorityType>::Merge(std::shared_ptr<TreapNode> left_node,
                                    std::shared_ptr<TreapNode> right_node) {
  if (left_node == nullptr || right_node == nullptr) {
    return (left_node != nullptr) ? left_node : right_node;
  }
  if (left_node->priority_ < right_node->priority_) {
    right_node->SetChild(Merge(left_node, right_node->left_child_), TreapNode::ChildType::LeftChild);
    return right_node;
  } else {
    left_node->SetChild(Merge(left_node->right_child_, right_node), TreapNode::ChildType::RightChild);
    return left_node;
  }
}

template<typename KeyType, typename PriorityType>
std::pair<std::shared_ptr<typename Treap<KeyType, PriorityType>::TreapNode>,
          std::shared_ptr<typename Treap<KeyType, PriorityType>::TreapNode>>
Treap<KeyType, PriorityType>::Split(std::shared_ptr<TreapNode> node,
                                    const KeyType &key) {
  if (node == nullptr) {
    return {nullptr, nullptr};
  } else if (key < node->key_) {
    auto[left_node, right_node] = Split(node->left_child_, key);
    node->SetChild(right_node, TreapNode::ChildType::LeftChild);
    if (left_node) {
      left_node->parent_.lock() = nullptr;
    }
    if (right_node) {
      right_node->parent_ = node;
    }
    return {left_node, node};
  } else {
    auto[left_node, right_node] = Split(node->right_child_, key);
    node->SetChild(left_node, TreapNode::ChildType::RightChild);
    if (left_node) {
      left_node->parent_ = node;
    }
    if (right_node) {
      right_node->parent_.lock() = nullptr;
    }
    return {node, right_node};
  }
}

template<typename KeyType, typename PriorityType>
bool Treap<KeyType, PriorityType>::FindKey(const KeyType &key) const {
  return FindKey(key, root_);
}

template<typename KeyType, typename PriorityType>
void Treap<KeyType, PriorityType>::Insert(const KeyType &key, const PriorityType &priority, size_t num) {
  if (FindKey(key, root_)) {
    return;
  }
  auto[left_node, right_node] = Split(root_, key);
  std::shared_ptr<TreapNode> new_node = std::make_shared<TreapNode>(key, priority, num);
  root_ = Merge(Merge(left_node, new_node), right_node);
}

template<typename KeyType, typename PriorityType>
void Treap<KeyType, PriorityType>::InsertSortedOrder(const KeyType &key,
                                                     const PriorityType &priority,
                                                     size_t num,
                                                     std::shared_ptr<TreapNode> &last_pushed) {
  std::shared_ptr<TreapNode> current_node = last_pushed;
  if (root_ == nullptr) {
    Insert(key, priority, num);
    last_pushed = root_;
    return;
  }
  current_node = last_pushed;
  while (current_node && current_node->priority_ < priority) {
    current_node = current_node->parent_.lock();
  }
  std::shared_ptr<TreapNode> new_node = std::make_shared<TreapNode>(key, priority, num);
  if (current_node) {
    new_node->SetChild(current_node->right_child_, TreapNode::ChildType::LeftChild);
    current_node->SetChild(new_node, TreapNode::ChildType::RightChild);
  } else {
    new_node->SetChild(root_, TreapNode::ChildType::LeftChild);
    root_ = new_node;
  }
  last_pushed = new_node;
}

template<typename KeyType, typename PriorityType>
bool Treap<KeyType, PriorityType>::FindKey(const KeyType &key, std::shared_ptr<TreapNode> treap_node) const {
  if (treap_node == nullptr) {
    return false;
  }
  if (key == treap_node->key_) {
    return true;
  } else if (key < treap_node->key_) {
    return FindKey(key, treap_node->left_child_);
  } else {
    return FindKey(key, treap_node->right_child_);
  }
}

template<typename KeyType, typename PriorityType>
void Treap<KeyType, PriorityType>::GetTreapNodeInfo(std::shared_ptr<Treap<KeyType, PriorityType>::TreapNode> node,
                                                    std::vector<TreapNodeInfo> &treap_nodes_info) const {
  if (node == nullptr) {
    return;
  }
  GetTreapNodeInfo(node->left_child_, treap_nodes_info);
  treap_nodes_info[node->num_ - 1].parent_num = (node->parent_.lock() ? node->parent_.lock()->num_ : 0);
  treap_nodes_info[node->num_ - 1].left_child_num = (node->left_child_ ? node->left_child_->num_ : 0);
  treap_nodes_info[node->num_ - 1].right_child_num = (node->right_child_ ? node->right_child_->num_ : 0);
  GetTreapNodeInfo(node->right_child_, treap_nodes_info);
}

template<typename KeyType, typename PriorityType>
void Treap<KeyType, PriorityType>::PrintTreap(size_t num) const {
  std::vector<TreapNodeInfo> treap_nodes_info(num);
  GetTreapNodeInfo(root_, treap_nodes_info);
  for (auto &node : treap_nodes_info) {
    std::cout << node.parent_num << ' ' << node.left_child_num << ' ' << node.right_child_num << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t query_num;
  std::cin >> query_num;
  std::vector<Node<int64_t, int64_t>> nodes;
  for (size_t i = 0; i < query_num; ++i) {
    int64_t key;
    int priority;
    std::cin >> key >> priority;
    nodes.push_back({key, -priority, i + 1});
  }
  std::sort(nodes.begin(), nodes.end());
  Treap<int64_t, int64_t> treap(nodes.begin(), nodes.end());
  std::cout << "YES" << std::endl;
  treap.PrintTreap(query_num);
  return 0;
}