#include <iostream>
#include <vector>

template <typename T>
class FenwickTree_2D {
 private:
  std::vector<std::vector<T>> tree_;

  T RSQ(int64_t x, int64_t y) const;
 public:
  explicit FenwickTree_2D(size_t height, size_t width);
  T RSQ(int64_t x1, int64_t y1, int64_t x2, int64_t y2) const;
  void Update(size_t x, size_t y, const T &value);
  size_t GetWidth() const;
  size_t GetHeight() const;
};

template <typename T>
FenwickTree_2D<T>::FenwickTree_2D(size_t height, size_t width) : tree_(height, std::vector<T>(width)) {}

template <typename T>
T FenwickTree_2D<T>::RSQ(int64_t x, int64_t y) const {
  T sum = 0;
  for(int64_t i = x; i >= 0; i = ((i & (i + 1)) - 1)) {
    for (int64_t j = y; j >= 0; j = ((j & (j + 1)) - 1)) {
      sum += tree_[i][j];
    }
  }
  return sum;
}

template <typename T>
T FenwickTree_2D<T>::RSQ(int64_t x1, int64_t y1, int64_t x2, int64_t y2) const {
  return RSQ(x2, y2) - RSQ(x1 - 1, y2) - RSQ(x2, y1 - 1) + RSQ(x1 - 1, y1 - 1);
}

template <typename T>
size_t FenwickTree_2D<T>::GetWidth() const {
  return tree_[0].size();
}

template <typename T>
size_t FenwickTree_2D<T>::GetHeight() const {
  return tree_.size();
}

template <typename T>
void FenwickTree_2D<T>::Update(size_t x, size_t y, const T &value) {
  for (size_t i = x; i < GetHeight(); i = (i | (i + 1))) {
    for (size_t j = y; j < GetWidth(); j = (j | (j + 1))) {
      tree_[i][j] += value;
    }
  }
}

int main() {
  size_t x, y;
  size_t n;
  std::cin >> x >> y >> n;
  FenwickTree_2D<int64_t> fenwick_tree(x, y);
  for (size_t i = 0; i < n; ++i) {
    int query;
    std::cin >> query;
    if (query == 1) {
      size_t x1, y1;
      int64_t value;
      std::cin >> x1 >> y1 >> value;
      fenwick_tree.Update(x1 - 1, y1 - 1, value);
    } else if (query == 2) {
      int64_t x1, y1, x2, y2;
      std::cin >> x1 >> y1 >> x2 >> y2;
      std::cout << fenwick_tree.RSQ(x1 - 1, y1 - 1, x2 - 1, y2 - 1) << std::endl;
    }
  }

  return 0;
}