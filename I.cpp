#include <iostream>
#include <vector>

template<typename T>
class FenwickTree_3D {
 private:
  std::vector<std::vector<std::vector<T>>> tree_;

  T RSQ(int64_t x, int64_t y, int64_t z) const;
 public:
  explicit FenwickTree_3D(size_t height, size_t length, size_t width);
  T RSQ(int64_t x1, int64_t y1, int64_t z1, int64_t x2, int64_t y2, int64_t z2) const;
  void Update(size_t x, size_t y, size_t z, const T &value);
  size_t GetWidth() const;
  size_t GetHeight() const;
  size_t GetLength() const;
};

template<typename T>
FenwickTree_3D<T>::FenwickTree_3D(size_t height, size_t length, size_t width)
    : tree_(height, std::vector<std::vector<T>>(length, std::vector<T>(width))) {}

template<typename T>
T FenwickTree_3D<T>::RSQ(int64_t x, int64_t y, int64_t z) const {
  T sum = 0;
  for (int64_t i = x; i >= 0; i = ((i & (i + 1)) - 1)) {
    for (int64_t j = y; j >= 0; j = ((j & (j + 1)) - 1)) {
      for (int64_t k = z; k >= 0; k = ((k & (k + 1)) - 1)) {
        sum += tree_[i][j][k];
      }
    }
  }
  return sum;
}

template<typename T>
T FenwickTree_3D<T>::RSQ(int64_t x1, int64_t y1, int64_t z1, int64_t x2, int64_t y2, int64_t z2) const {
  return RSQ(x2, y2, z2) - RSQ(x1 - 1, y2, z2) - RSQ(x2, y1 - 1, z2) - RSQ(x2, y2, z1 - 1) +
      RSQ(x1 - 1, y1 - 1, z2) + RSQ(x1 - 1, y2, z1 - 1) + RSQ(x2, y1 - 1, z1 - 1) - RSQ(x1 - 1, y1 - 1, z1 - 1);
}

template<typename T>
size_t FenwickTree_3D<T>::GetLength() const {
  return tree_[0].size();
}

template<typename T>
size_t FenwickTree_3D<T>::GetHeight() const {
  return tree_.size();
}

template<typename T>
size_t FenwickTree_3D<T>::GetWidth() const {
  return tree_[0][0].size();
}

template<typename T>
void FenwickTree_3D<T>::Update(size_t x, size_t y, size_t z, const T &value) {
  for (size_t i = x; i < GetHeight(); i = (i | (i + 1))) {
    for (size_t j = y; j < GetLength(); j = (j | (j + 1))) {
      for (size_t k = z; k < GetWidth(); k = (k | (k + 1))) {
        tree_[i][j][k] += value;
      }
    }
  }
}

int main() {
  size_t n;
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cin >> n;
  FenwickTree_3D<int64_t> fenwick_tree(n, n, n);
  int query;
  std::cin >> query;
  while (query != 3) {
    if (query == 1) {
      size_t x, y, z;
      int64_t value;
      std::cin >> x >> y >> z >> value;
      fenwick_tree.Update(x, y, z, value);
    } else if (query == 2) {
      int64_t x1, y1, z1, x2, y2, z2;
      std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
      std::cout << fenwick_tree.RSQ(x1, y1, z1, x2, y2, z2) << std::endl;
    }
    std::cin >> query;
  }

  return 0;
}