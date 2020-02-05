/*
 Вася любит наблюдать за звездами. Но следить за всем небом сразу ему тяжело.
 Поэтому он наблюдает только за частью пространства, ограниченной кубом размером n на n на n.
 Этот куб поделён на маленькие кубики размером 1x1x1. Во время его наблюдений могут происходить
 следующие события:
• в каком-то кубике появляются или исчезают несколько звезд;
• к нему может заглянуть его друг Петя и поинтересоваться, сколько видно звезд в части пространства,
 состоящей из нескольких кубиков.

Input format
Первая строка входного файла содержит натуральное число n (n ≤ 128). Координаты кубиков
 - целые число от 0 до n-1. Далее следуют записи о происходивших событиях по одной в строке.
В начале строки записано число m. Если m равно:
• 1, то за ним следуют 4 числа: x, y, z, k. (0 ≤ x, y, z < N, -20000 ≤ k ≤ 20000) - координаты кубика
 и величина, на которую в нем изменилось количество видимых звезд;
• 2, то за ним следуют 6 чисел: x1, y1, z1, x2, y2, z2 (0 ≤ xi, yi, zi < N, x1 ≤ x2, y1 ≤ y2, z1 ≤ z2
 которые означают, что Петя попросил подсчитать количество звезд в кубиках (x', y', z') из области:
 x1 ≤ x' ≤ x2, y1 ≤ y' ≤ y2, z1 ≤ z' ≤ z2.
• 3, то это означает, что Васе надоело наблюдать за звездами и отвечать на вопросы Пети. Эта запись
 встречается во входном файле только один раз и будет последней записью. Количество записей во входном
 файле не превосходит 100500.

Output format
Для каждого Петиного вопроса выведите на отдельной строке одно число - искомое количество звезд.
Examples
Input	          Output
2                   0
2 1 1 1 1 1 1       1
1 0 0 0 1           4
1 0 1 0 3           2
2 0 0 0 0 0 0
2 0 0 0 0 1 0
1 0 1 0 -2
2 0 0 0 1 1 1
3
 */

#include <iostream>
#include <vector>

struct Point {
  int64_t x_;
  int64_t y_;
  int64_t z_;

  Point(int64_t x, int64_t y, int64_t z) : x_(x), y_(y), z_(z) {}
};

template<typename T>
class FenwickTree_3D {
 private:
  std::vector<std::vector<std::vector<T>>> tree_;

  T RSQ(const Point &point) const;
 public:
  explicit FenwickTree_3D(size_t height, size_t length, size_t width);
  T RSQ(const Point &lhs, const Point &rhs) const;
  void Update(const Point &point, const T &value);
  size_t GetWidth() const;
  size_t GetHeight() const;
  size_t GetLength() const;
};

template<typename T>
FenwickTree_3D<T>::FenwickTree_3D(size_t height, size_t length, size_t width)
    : tree_(height, std::vector<std::vector<T>>(length, std::vector<T>(width))) {}

template<typename T>
T FenwickTree_3D<T>::RSQ(const Point &point) const {
  T sum = 0;
  for (int64_t i = point.x_; i >= 0; i = ((i & (i + 1)) - 1)) {
    for (int64_t j = point.y_; j >= 0; j = ((j & (j + 1)) - 1)) {
      for (int64_t k = point.z_; k >= 0; k = ((k & (k + 1)) - 1)) {
        sum += tree_[i][j][k];
      }
    }
  }
  return sum;
}

template<typename T>
T FenwickTree_3D<T>::RSQ(const Point &lhs, const Point &rhs) const {
  return RSQ(Point(rhs.x_, rhs.y_, rhs.z_)) - RSQ(Point(lhs.x_ - 1, rhs.y_, rhs.z_)) -
        RSQ(Point(rhs.x_, lhs.y_ - 1, rhs.z_)) - RSQ(Point(rhs.x_, rhs.y_, lhs.z_ - 1)) +
        RSQ(Point(lhs.x_ - 1, lhs.y_ - 1, rhs.z_)) + RSQ(Point(lhs.x_ - 1, rhs.y_, lhs.z_ - 1)) +
        RSQ(Point(rhs.x_, lhs.y_ - 1, lhs.z_ - 1)) - RSQ(Point(lhs.x_ - 1, lhs.y_ - 1, lhs.z_ - 1));
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
void FenwickTree_3D<T>::Update(const Point &point, const T &value) {
  for (size_t i = point.x_; i < GetHeight(); i = (i | (i + 1))) {
    for (size_t j = point.y_; j < GetLength(); j = (j | (j + 1))) {
      for (size_t k = point.z_; k < GetWidth(); k = (k | (k + 1))) {
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
      int64_t x, y, z;
      int64_t value;
      std::cin >> x >> y >> z >> value;
      fenwick_tree.Update(Point(x, y, z), value);
    } else if (query == 2) {
      int64_t x1, y1, z1, x2, y2, z2;
      std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
      std::cout << fenwick_tree.RSQ(Point(x1, y1, z1), Point(x2, y2, z2)) << std::endl;
    }
    std::cin >> query;
  }

  return 0;
}