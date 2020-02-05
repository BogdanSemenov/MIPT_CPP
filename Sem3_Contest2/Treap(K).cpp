/*
 Реализуйте сбалансированное двоичное дерево поиска.
Input format
Входной файл содержит описание операций с деревом, их количество не превышает 10^5.
 В каждой строке находится одна из следующих операций.

• insert x - добавить в дерево ключ x.
• delete x - удалить из дерева ключ x. Если его там нет, то ничего делать не надо.
• exists x - если ключ x есть в дереве, выведите "true", иначе, "false"
• next x - выведите минимальный элемент в дереве, строго больший x или "none", если такого нет.
• prev x - выведите максимальный элемент в дереве, строго меньший x или "none", если такого нет.
• kth x - выведите k-тый по величине элемент(нумерация с единицы). Если такого не существует, то выведите "none".
Все числа во входном файле не превышают 10^9.

Output format
Выведите последовательно результат выполнения всех операций exists, next, prev. Следуйте формату
 выходного файла из примера.

Examples
Input

insert 2
insert 5
insert 3
exists 2
exists 4
next 4
prev 4
delete 5
next 4
prev 4
kth 1
kth 3

Output

true
false
5
3
none
3
2
none

 */


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
  mutable std::shared_ptr<TreapNode> root_;

  bool FindKey(const KeyType &key, std::shared_ptr<TreapNode> treap_node) const;

  static std::pair<std::shared_ptr<TreapNode>, std::shared_ptr<TreapNode>> Split(std::shared_ptr<TreapNode> node,
                                                                                 const KeyType &key);

  static std::shared_ptr<TreapNode> Merge(std::shared_ptr<TreapNode> left_node,
                                          std::shared_ptr<TreapNode> right_node);

  KeyType GetKthSmallestElement(std::shared_ptr<TreapNode> treap_node, size_t k) const;

  int GenerateRandomPriority();

 public:
  Treap() = default;

  bool FindKey(const KeyType &key) const override;

  void Insert(const KeyType &key) override;

  void Remove(const KeyType &key) override;

  void Insert(const KeyType &key, int priority);

  KeyType LowerBound(const KeyType &key) const;

  KeyType UpperBound(const KeyType &key) const;

  KeyType GetKthSmallestElement(size_t k) const;
};

////////////////////////////// class TreapNode //////////////////////////////

template<typename KeyType>
struct Treap<KeyType>::TreapNode {
  KeyType key_;

  int priority_;

  size_t size_;

  std::shared_ptr<TreapNode> left_child_;

  std::shared_ptr<TreapNode> right_child_;
  explicit TreapNode() = default;

  explicit TreapNode(const KeyType &key, const int priority, const size_t size = 1)
      : key_(key), priority_(priority), size_(size) {}

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

  static void UpdateSize(std::shared_ptr<TreapNode> treap_node) {
    if (treap_node == nullptr) {
      return;
    }
    treap_node->size_ = GetSize(treap_node->left_child_) + GetSize(treap_node->right_child_) + 1;
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
    TreapNode::UpdateSize(right_node);
    return right_node;
  } else {
    left_node->SetChild(Merge(left_node->right_child_, right_node), TreapNode::ChildType::RightChild);
    TreapNode::UpdateSize(left_node);
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
    TreapNode::UpdateSize(node);
    return {left_node, node};
  } else {
    auto[left_node, right_node] = Split(node->right_child_, key);
    node->SetChild(left_node, TreapNode::ChildType::RightChild);
    TreapNode::UpdateSize(node);
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
KeyType Treap<KeyType>::LowerBound(const KeyType &key) const {
  constexpr KeyType NOT_SET(-1);
  auto[left_node, right_node] = Split(root_, key);
  auto move_left = right_node;
  if (right_node == nullptr) {
    return NOT_SET;
  }
  while (move_left->left_child_) {
    move_left = move_left->left_child_;
  }
  root_ = Merge(left_node, right_node);
  return move_left->key_;
}

template<typename KeyType>
KeyType Treap<KeyType>::UpperBound(const KeyType &key) const {
  constexpr KeyType NOT_SET(-1);
  auto[left_node, right_node] = Split(root_, key - 1);
  auto move_right = left_node;
  if (left_node == nullptr) {
    return NOT_SET;
  }
  while (move_right->right_child_) {
    move_right = move_right->right_child_;
  }
  root_ = Merge(left_node, right_node);
  return move_right->key_;
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
KeyType Treap<KeyType>::GetKthSmallestElement(std::shared_ptr<TreapNode> treap_node, size_t k) const {
  constexpr KeyType NOT_SET(-1);
  if (treap_node == nullptr) {
    return NOT_SET;
  }
  const size_t left_side_size = TreapNode::GetSize(treap_node->left_child_);
  if (left_side_size == k - 1) {
    return treap_node->key_;
  } else if (left_side_size < k - 1) {
    return GetKthSmallestElement(treap_node->right_child_, k - left_side_size - 1);
  } else {
    return GetKthSmallestElement(treap_node->left_child_, k);
  }
}

template<typename KeyType>
KeyType Treap<KeyType>::GetKthSmallestElement(size_t k) const {
  return GetKthSmallestElement(root_, k);
}

int main() {
  Treap<int64_t> treap;
  std::string query;
  while (std::cin >> query) {
    int64_t key;
    std::cin >> key;
    if (query == "insert") {
      treap.Insert(key);
    } else if (query == "delete") {
      treap.Remove(key);
    } else if (query == "exists") {
      std::cout << (treap.FindKey(key) ? "true\n" : "false\n");
    } else if (query == "next") {
      int64_t answer = treap.LowerBound(key);
      if (answer == -1) {
        std::cout << "none\n";
      } else {
        std::cout << answer << std::endl;
      }
    } else if (query == "prev") {
      int64_t answer = treap.UpperBound(key);
      if (answer == -1) {
        std::cout << "none\n";
      } else {
        std::cout << answer << std::endl;
      }
    } else if (query == "kth") {
      int64_t answer = treap.GetKthSmallestElement(key);
      if (answer == -1) {
        std::cout << "none\n";
      } else {
        std::cout << answer << std::endl;
      }
    }
  }
  return 0;
}