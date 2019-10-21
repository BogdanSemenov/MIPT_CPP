/*Вы - глава агенства, в котором работает N сотрудников - шпионы с кодовыми номерами от 1 до N.
 Шпионы были посланы в различные страны и получили кусочки важной информации. Ваша задача состоит в следующем:

1. Организовать встречи между некоторыми шпионами. На каждой такой встрече ровно два шпиона оказываются в
 одном месте и обмениваются всей информацией, которую получили сами или узнали от других шпионов на
 предыдущих встречах. Каждую из возможных встреч сложно и затратно организовать, поэтому вам известны все
 стоимости.

2. После того, как все встречи состоялись, выбирается группа шпионов, и они посылаются на задание
 по спасению мира. Для каждого шпиона известно, сколько денег он изведет, если его включить в эту группу.
 Задание должно быть выполнено, поэтому очень важно, чтобы выбранные шпионы все вместе обладали всей
 информацией, полученной агенством.

Найдите минимальную цену, за которую можно спасти мир.

Input format

Первая строка содержит натуральное число N - количество шпионов (2 ≤ N ≤ 1000). Каждая из следующих строк
 содержит N натуральных чисел, не превосходящих 106. Число в k-ой строке и m-ом столбце обозначает стоимость
 встречи между шпионами k и m и совпадает с числом в m-ой строке и k-ом столбце (если k=m, то соответствующее
 число будет равно 0). Следующая строка содержит n чисел, i-ое из них равно стоимости послать i-го шпиона на
 задание. Все стоимости являются положительными целыми числами.

Output format

Выведите одно число - минимальную стоимость спасения мира.

 Input	     Output
3              17
0 6 9
6 0 4
9 4 0
7 7 7

3              34
0 17 20
17 0 10
20 10 0
15 9 12

5              28
0 3 12 15 11
3 0 14 3 20
12 14 0 11 7
15 3 11 0 15
11 20 7 15 0
5 10 10 10 10
 */


#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

class Graph {
 protected:
  size_t vertex_count_;
  size_t edge_count_;
  bool is_directed_;

 public:
  typedef size_t Vertex;

  struct Edge {
    Vertex from;
    Vertex to;
    int weight;

    bool operator<(const Edge &other) const {
      return weight < other.weight;
    }
  };

  explicit Graph(size_t vertex_count, bool is_directed)
      : vertex_count_(vertex_count),
        is_directed_(is_directed),
        edge_count_(0) {}

  virtual void AddEdge(const Vertex &start, const Vertex &finish, int weight = 1) = 0;

  virtual std::vector<Edge> GetEdges() const = 0;

  virtual std::vector<Vertex> GetAllVertices() const = 0;
};

class GraphAdjList : public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;
  std::vector<Edge> edges_;

 public:
  explicit GraphAdjList(size_t vertex_count, bool is_directed)
      : Graph(vertex_count, is_directed),
        adj_list_(vertex_count + 1) {}

  void AddEdge(const Vertex &start, const Vertex &finish, int weight = 1) override {
    adj_list_[start].push_back(finish);
    edges_.emplace_back(start, finish, weight);
    if (!is_directed_) {
      adj_list_[finish].push_back(start);
      edges_.emplace_back(finish, start, weight);
    }
    ++edge_count_;
  }

  std::vector<Edge> GetEdges() const override {
    return edges_;
  }

  std::vector<Vertex> GetAllVertices() const override {
    std::vector<Vertex> all_vertices;
    for (Vertex vertex = 1; vertex <= vertex_count_; ++vertex) {
      all_vertices.push_back(vertex);
    }
    return all_vertices;
  }
};

namespace GraphProcessing {

  template<typename T>
  class DSU {
   private:
    std::map<T, size_t> rank_;
    std::map<T, T> predecessors_;

    T FindSet(const T &x) {
      if (x == predecessors_[x]) {
        return x;
      }
      return predecessors_[x] = FindSet(predecessors_[x]);
    }

   public:
    explicit DSU(const std::vector<T> &elements) {
      for (int i = 0; i < elements.size(); ++i) {
        predecessors_[elements[i]] = elements[i];
        rank_[elements[i]] = 0;
      }
    }

    void Union(const T &x, const T &y) {
      T x_predecessor = FindSet(x);
      T y_predecessor = FindSet(y);

      if (x_predecessor == y_predecessor) {
        return;
      }
      if (rank_[x_predecessor] < rank_[y_predecessor]) {
        std::swap(x_predecessor, y_predecessor);
      }
      predecessors_[y_predecessor] = x_predecessor;
      if (rank_[x_predecessor] == rank_[y_predecessor]) {
        ++rank_[x_predecessor];
      }
    }

    bool InSameSet(const T &x, const T &y) {
      return FindSet(x) == FindSet(y);
    }
  };

  std::vector<Graph::Edge> Kruskal_Get_MST(const Graph &graph) {
    auto edges = graph.GetEdges();
    std::sort(edges.begin(), edges.end());
    std::vector<Graph::Edge> min_span_tree;
    DSU<Graph::Vertex> dsu(graph.GetAllVertices());
    for (auto edge : edges) {
      if (!dsu.InSameSet(edge.from, edge.to)) {
        dsu.Union(edge.from, edge.to);
        min_span_tree.push_back(edge);
      }
    }
    return min_span_tree;
  }
}

int main() {
  size_t n;

  std::cin >> n;
  GraphAdjList graph_adj_list = GraphAdjList(n + 1, false);

  for (size_t start = 1; start <= n; ++start) {
    for (size_t finish = 1; finish <= n; ++finish) {
      int weight;
      std::cin >> weight;
      if (start != finish) {
        graph_adj_list.AddEdge(start, finish, weight);
      }
    }
  }

  for (size_t vertex = 1; vertex <= n; ++vertex) {
    int weight;
    std::cin >> weight;
    graph_adj_list.AddEdge(vertex, n + 1, weight);
  }
  auto MST = GraphProcessing::Kruskal_Get_MST(graph_adj_list);
  size_t MST_weight = 0;
  for (const auto &edge : MST) {
    MST_weight += edge.weight;
  }
  std::cout << MST_weight;

  return 0;
}