#include <iostream>
#include <vector>

struct Point {
  int64_t x_;
  int64_t y_;

  Point(int64_t x, int64_t y) : x_(x), y_(y) {}
};

template<typename T>
class FenwickTree_2D {
 private:
  std::vector<std::vector<T>> tree_;

  T RSQ(const Point &point) const;
 public:
  explicit FenwickTree_2D(size_t height, size_t width);
  T RSQ(const Point &lhs, const Point &rhs) const;
  void Update(const Point &point, const T &value);
  size_t GetWidth() const;
  size_t GetHeight() const;
};

template<typename T>
FenwickTree_2D<T>::FenwickTree_2D(size_t height, size_t width) : tree_(height, std::vector<T>(width)) {}

template<typename T>
T FenwickTree_2D<T>::RSQ(const Point &point) const {
  T sum = 0;
  for (int64_t i = point.x_; i >= 0; i = ((i & (i + 1)) - 1)) {
    for (int64_t j = point.y_; j >= 0; j = ((j & (j + 1)) - 1)) {
      sum += tree_[i][j];
    }
  }
  return sum;
}

template<typename T>
T FenwickTree_2D<T>::RSQ(const Point &lhs, const Point &rhs) const {
  return RSQ(Point(rhs.x_, rhs.y_)) -
         RSQ(Point(lhs.x_ - 1, rhs.y_)) -
         RSQ(Point(rhs.x_, lhs.y_ - 1)) +
         RSQ(Point(lhs.x_ - 1, lhs.y_ - 1));
}

template<typename T>
size_t FenwickTree_2D<T>::GetWidth() const {
  return tree_[0].size();
}

template<typename T>
size_t FenwickTree_2D<T>::GetHeight() const {
  return tree_.size();
}

template<typename T>
void FenwickTree_2D<T>::Update(const Point &point, const T &value) {
  for (size_t i = point.x_; i < GetHeight(); i = (i | (i + 1))) {
    for (size_t j = point.y_; j < GetWidth(); j = (j | (j + 1))) {
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
      fenwick_tree.Update(Point(x1 - 1, y1 - 1), value);
    } else if (query == 2) {
      int64_t x1, y1, x2, y2;
      std::cin >> x1 >> y1 >> x2 >> y2;
      std::cout << fenwick_tree.RSQ(Point(x1 - 1, y1 - 1), Point(x2 - 1, y2 - 1)) << std::endl;
    }
  }

  return 0;
}
