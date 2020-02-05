/*
 Саша любит наблюдать за ромашками. Но следить за всем полем сразу ей тяжело. Поэтому она наблюдает только за его частью,
 ограниченной прямоугольником размером x на y. Этот прямоугольник поделён на маленькие прямоугольники размером 1x1.
 Во время её наблюдений могут происходить следующие события:
• в каком-то прямоугольнике появляются или исчезают несколько ромашек;
• к ней может заглянуть её подруга Настя и поинтересоваться, сколько видно ромашек в части поля, состоящей из нескольких
 прямоугольников.

Input format
Первая строка входного файла содержит два натуральных числа x и y (1 ≤ x, y ≤ 1000). Следующая строка содержит
 единственное натуральное число n (1 ≤ n ≤ 100000) - количество произошедших событий. Далее следуют n записей о
 происходивших событиях по одной в строке. В начале строки записано число t. Если t равно:

• 1, то за ним следуют 3 числа: x1, y1 (1 ≤ x1 ≤ x, 1 ≤ y1 ≤ y) и k(-10000 ≤ k ≤ 10000) - координаты прямоугольника
 и величина, на которую в нём изменилось количество видимых ромашек;
• 2, то за ним следуют 4 числа: x1, y1, x2, y2 (1 ≤ x1 ≤ x2 ≤ x, 1 ≤ y1 ≤ y2 ≤ y), которые означают, что Настя попросила
 подсчитать количество ромашек в прямоугольниках (x', y') из области: x1 ≤ x' ≤ x2, y1 ≤ y' ≤ y2.

Output format
Для каждого Настиного вопроса выведите в отдельной строке искомое количество ромашек (оно может быть отрицательным).
Examples
Input	     Output
8 8            3
3
1 2 2 2
1 1 1 1
2 1 1 2 2
 */


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
      int64_t x1, y1;
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