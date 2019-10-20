#include <iostream>
#include <vector>
#include <algorithm>

class DSU {
 private:
  std::vector<size_t> rank_;
  std::vector<int> predecessors_;
  std::vector<int> weight_;

  int FindSet(int x) {
    if (x == predecessors_[x]) {
      return x;
    }
    return predecessors_[x] = FindSet(predecessors_[x]);
  }

 public:
  explicit DSU(int num) {
    weight_.resize(num + 1, 0);
    rank_.resize(num + 1, 0);
    predecessors_.resize(num + 1);
    for (int i = 1; i <= num; ++i) {
      predecessors_[i] = i;
    }
  }

  void Union(int x, int y, int weight) {
    int x_predecessor = FindSet(x);
    int y_predecessor = FindSet(y);

    if (x_predecessor == y_predecessor) {
      weight_[x_predecessor] += weight;
      return;
    }
    if (rank_[x_predecessor] < rank_[y_predecessor]) {
      std::swap(x_predecessor, y_predecessor);
    }
    predecessors_[y_predecessor] = x_predecessor;
    weight_[x_predecessor] += weight_[y_predecessor] + weight;
    if (rank_[x_predecessor] == rank_[y_predecessor]) {
      ++rank_[x_predecessor];
    }
  }

  int GetWeight(int x) {
    int  x_predecessor = FindSet(x);
    return weight_[x_predecessor];
  }
};

int main() {
  size_t n, m;
  std::cin >> n >> m;
  DSU dsu(n);

  for (int i = 0; i < m; ++i) {
    int action;
    std::cin >> action;
    if (action == 1) {
      size_t start, finish;
      int weight;
      std::cin >> start >> finish >> weight;
      dsu.Union(start, finish, weight);
    } else if (action == 2) {
      size_t vertex;
      std::cin >> vertex;
      std::cout << dsu.GetWeight(vertex) << std::endl;
    }
  }

  return 0;
}