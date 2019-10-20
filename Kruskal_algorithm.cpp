/* Требуется найти в связном графе остовное дерево минимального веса.

Input format

Первая строка входного файла содержит два натуральных числа n и m — количество вершин и ребер графа
 соответственно (1 ≤ n ≤ 20000, 0 ≤ m ≤ 100000). Следующие m строк содержат описание ребер по одному на строке.
 Ребро номер i описывается тремя натуральными числами b_i, e_i и w_i — номера концов ребра и его вес
 соответственно (1 ≤ b_i,e_i ≤ n, 0 ≤ w_i ≤ 100000).
Граф является связным.

Output format

Выведите единственное целое число — вес минимального остовного дерева.
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

    Edge(const Vertex &from, const Vertex &to, int weight) :
        from(from), to(to), weight(weight) {}

    bool operator<(const Edge &other) const {
      return weight < other.weight;
    }
  };

  explicit Graph(size_t vertex_count, bool is_directed)
      : vertex_count_(vertex_count),
        is_directed_(is_directed),
        edge_count_(0) {}

  size_t GetVertexCount() const {
    return vertex_count_;
  }

  virtual void AddEdge(const Vertex &start, const Vertex &finish, int weight) = 0;

  virtual std::vector<Edge> GetEdges() const = 0;
};

class GraphAdjList : public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;
  std::vector<Edge> edges_;

 public:
  explicit GraphAdjList(size_t vertex_count, bool is_directed)
      : Graph(vertex_count, is_directed),
        adj_list_(vertex_count + 1) {}

  void AddEdge(const Vertex &start, const Vertex &finish, int weight) override {
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
  explicit DSU(const T &start, const T &finish) {
    for (T set = start; set <= finish; ++set) {
      predecessors_[set] = set;
      rank_[set] = 0;
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
  DSU<Graph::Vertex> dsu(1, graph.GetVertexCount());
  for (auto edge : edges) {
    if (!dsu.InSameSet(edge.from, edge.to)) {
      dsu.Union(edge.to, edge.from);
      min_span_tree.push_back(edge);
    }
  }
  return min_span_tree;
}
}

int main() {
  size_t n, m;

  std::cin >> n >> m;
  GraphAdjList graph_adj_list = GraphAdjList(n, false);

  for (int j = 0; j < m; ++j) {
    Graph::Vertex start, finish;
    int weight;
    std::cin >> start >> finish >> weight;
    graph_adj_list.AddEdge(start, finish, weight);
  }
  auto MST = GraphProcessing::Kruskal_Get_MST(graph_adj_list);
  size_t MST_weight = 0;
  for (const auto &edge : MST) {
    MST_weight += edge.weight;
  }
  std::cout << MST_weight;

  return 0;
}
