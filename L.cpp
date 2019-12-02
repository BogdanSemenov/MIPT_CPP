#include <iostream>
#include <vector>
#include <random>
#include <memory>
#include <ctime>
#include <string>

////////////////////////////// class SetInterface //////////////////////////////

template<typename KeyType>
class SetInterface {
 public:
  virtual void Insert(const KeyType &key) = 0;

  virtual void Remove(const KeyType &key) = 0;

  virtual bool FindKey(const KeyType &key) const = 0;
};

////////////////////////////// class Treap (Declaration) //////////////////////////////

template<typename KeyType>
class Treap : public SetInterface<KeyType> {
 private:
  struct TreapNode;
  std::mt19937 random_priority_generator;
  std::shared_ptr<TreapNode> root_;

  bool FindKey(const KeyType &key, std::shared_ptr<TreapNode> treap_node) const;

  static std::pair<std::shared_ptr<TreapNode>, std::shared_ptr<TreapNode>> Split(std::shared_ptr<TreapNode> node,
                                                                          const KeyType &key);

  static std::shared_ptr<TreapNode> Merge(std::shared_ptr<TreapNode> left_node,
                                   std::shared_ptr<TreapNode> right_node);

  KeyType GetKthSmallestElement(std::shared_ptr<TreapNode> treap_node, size_t k) const;

  int GenerateRandomPriority();

 public:
  explicit Treap() = default;

  bool FindKey(const KeyType &key) const override;

  void Insert(const KeyType &key) override;

  void Remove(const KeyType &key) override;

  void Insert(const KeyType &key, int priority);

  KeyType GetSum(const KeyType &left, const KeyType &right);
};

////////////////////////////// class TreapNode //////////////////////////////

template<typename KeyType>
struct Treap<KeyType>::TreapNode {
  KeyType key_;

  KeyType sum_;

  int priority_;

  size_t size_;

  std::shared_ptr<TreapNode> left_child_;

  std::shared_ptr<TreapNode> right_child_;
  explicit TreapNode() = default;

  explicit TreapNode(const KeyType &key, const int priority, const size_t size = 1)
      : key_(key), priority_(priority), size_(size), sum_(key) {}

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
  }

  static size_t GetSize(std::shared_ptr<TreapNode> treap_node) {
    return treap_node ? treap_node->size_ : 0;
  }

  static KeyType GetSum(std::shared_ptr<TreapNode> treap_node) {
    return treap_node ? treap_node->sum_ : 0;
  }

  static void Update(std::shared_ptr<TreapNode> treap_node) {
    if (treap_node == nullptr) {
      return;
    }
    treap_node->size_ = GetSize(treap_node->left_child_) + GetSize(treap_node->right_child_) + 1;
    treap_node->sum_ = GetSum(treap_node->left_child_) + GetSum(treap_node->right_child_) + treap_node->key_;
  }
};


////////////////////////////// class Treap (Implementation) //////////////////////////////

template<typename KeyType>
int Treap<KeyType>::GenerateRandomPriority() {
  return random_priority_generator();
}

template<typename KeyType>
std::shared_ptr<typename Treap<KeyType>::TreapNode>
Treap<KeyType>::Merge(std::shared_ptr<TreapNode> left_node,
                      std::shared_ptr<TreapNode> right_node) {
  if (left_node == nullptr || right_node == nullptr) {
    return (left_node != nullptr) ? left_node : right_node;
  }
  if (left_node->priority_ < right_node->priority_) {
    right_node->SetChild(Merge(left_node, right_node->left_child_), TreapNode::ChildType::LeftChild);
    TreapNode::Update(right_node);
    return right_node;
  } else {
    left_node->SetChild(Merge(left_node->right_child_, right_node), TreapNode::ChildType::RightChild);
    TreapNode::Update(left_node);
    return left_node;
  }
}

template<typename KeyType>
std::pair<std::shared_ptr<typename Treap<KeyType>::TreapNode>,
          std::shared_ptr<typename Treap<KeyType>::TreapNode>>
Treap<KeyType>::Split(std::shared_ptr<TreapNode> node,
                      const KeyType &key) {
  if (node == nullptr) {
    return {nullptr, nullptr};
  } else if (key < node->key_) {
    auto[left_node, right_node] = Split(node->left_child_, key);
    node->SetChild(right_node, TreapNode::ChildType::LeftChild);
    TreapNode::Update(node);
    return {left_node, node};
  } else {
    auto[left_node, right_node] = Split(node->right_child_, key);
    node->SetChild(left_node, TreapNode::ChildType::RightChild);
    TreapNode::Update(node);
    return {node, right_node};
  }
}

template<typename KeyType>
bool Treap<KeyType>::FindKey(const KeyType &key) const {
  return FindKey(key, root_);
}

template<typename KeyType>
void Treap<KeyType>::Insert(const KeyType &key) {
  return Insert(key, GenerateRandomPriority());
}

template<typename KeyType>
void Treap<KeyType>::Insert(const KeyType &key, int priority) {
  if (FindKey(key, root_)) {
    return;
  }
  auto[left_node, right_node] = Split(root_, key);
  std::shared_ptr<TreapNode> new_node = std::make_shared<TreapNode>(key, priority);
  root_ = Merge(Merge(left_node, new_node), right_node);
}

template<typename KeyType>
void Treap<KeyType>::Remove(const KeyType &key) {
  if (!FindKey(key, root_)) {
    return;
  }
  auto[left_node, right_node] = Split(root_, key);
  auto[new_left_node, new_right_node] = Split(left_node, key - 1);
  root_ = Merge(new_left_node, right_node);
}

template<typename KeyType>
bool Treap<KeyType>::FindKey(const KeyType &key, std::shared_ptr<TreapNode> treap_node) const {
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

template<typename KeyType>
KeyType Treap<KeyType>::GetSum(const KeyType &left, const KeyType &right){
  auto[first_left_node, first_right_node] = Split(root_, right);
  auto[second_left_node, second_right_node] = Split(first_left_node, left - 1);
  KeyType result_sum = TreapNode::GetSum(second_right_node);
  root_ = Merge(Merge(second_left_node, second_right_node), first_right_node);
  return result_sum;
}

int main() {
  constexpr size_t MOD = 1000000000;
  size_t query_num;
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cin >> query_num;
  Treap<int64_t> treap;
  int64_t sum = 0;
  for (size_t i = 0; i < query_num; ++i) {
    char query;
    std::cin >> query;
    if (query == '+') {
      int64_t key;
      std::cin >> key;
      treap.Insert((key + sum) % MOD);
      sum = 0;
    } else if (query == '?') {
      int64_t left, right;
      std::cin >> left >> right;
      sum = treap.GetSum(left, right);
      std::cout << sum << std::endl;
    }
  }

  return 0;
}