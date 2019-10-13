/*
Одно разбросанное на островах Океании государство решило создать сеть автомобильных дорог (вернее, мостов).
 По каждому мосту можно перемещаться в обе стороны. Был разработан план очередности строительства мостов и известно,
 что после постройки всех мостов можно будет проехать по ним с каждого острова на каждый (возможно, через некоторые
 промежуточные острова

Однако, этот момент может наступить до того, как будут построены все мосты. Вам необходимо определить
 такое минимальное количество мостов, после строительства которых (в порядке, определенном планом), можно будет
 попасть с любого острова на любой другой.

Входные данные
Первая строка содержит два числа: число островов N (1 ≤ N ≤ 10000) и количество мостов в плане M (1 ≤ M ≤ 50000).
 Далее идет M строк, каждая содержит два числа x и y (1 ≤ x, y ≤ N) - номера городов, которые соединяет очередной
 мост в плане.

Выходные данные
Программа должна вывести единственное число - минимальное количество построенных мостов, после которого можно будет
 попасть с любого острова на любой другой.
 */

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

template <typename T>
class DSU {
 private:
  std::map<T, size_t> rank;
  std::map<T, T> prev;

 public:
  explicit DSU(std::vector<T> elements) {
    for (int i = 0; i < elements.size(); ++i) {
      prev[elements[i]] = elements[i];
      rank[elements[i]] = 0;
    }
  }

  T FindSet(T elem) {
    if (elem == prev[elem]) {
      return elem;
    }
    return prev[elem] = FindSet(prev[elem]);
  }

  void Union(T x, T y) {
    T x_parent = FindSet(x);
    T y_parent = FindSet(y);

    if (x_parent == y_parent) {
      return;
    }
    if (rank[x_parent] < rank[y_parent]) {
      std::swap(x_parent, y_parent);
    }
    prev[y_parent] = x_parent;
    if (rank[x_parent] == rank[y_parent]) {
      ++rank[x_parent];
    }
  }

  bool IsOneSet(T x, T y) {
    return FindSet(x) == FindSet(y);
  }
};

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<int> v;
  for (int i = 1; i <= n; ++i) {
    v.push_back(i);
  }

  DSU<int> dsu(v);
  int necessary_bridges_number = n;
  for (int i = 1; i <= m; ++i) {
    int a, b;
    std::cin >> a >> b;
    if (!dsu.IsOneSet(a, b)) {
      dsu.Union(a, b);
      if (--necessary_bridges_number == 1) {
        std::cout << i;
        break;
      }
    }
  }
  return 0;
}