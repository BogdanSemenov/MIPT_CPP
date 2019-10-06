/*От вас требуется определить вес минимального остовного дерева для неориентированного взвешенного связного графа.

Входные данные

В первой строке входного файла находятся числа N и M (1 ≤ N ≤ 100; 1 ≤ M ≤ 6000),
 где N - количество вершин в графе, а M - количество рёбер. В каждой из последующих M строк записано по тройке
 чисел A, B, C, где A и B - номера вершин, соединённых ребром, а C - вес ребра (натуральное число,
 не превышающее 30000).

Выходные данные

В выходной файл выведите одно число - искомый вес. */

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <limits>

class Graph {
 protected:
  size_t vertex_count_;
  size_t edge_count_;
  bool is_directed_;

 public:
  typedef size_t Vertex;

  explicit Graph(size_t vertex_count, bool is_directed)
      : vertex_count_(vertex_count),
        is_directed_(is_directed),
        edge_count_(0) {}

  size_t GetVertexCount() const {
    return vertex_count_;
  }

  size_t GetEdgeCount() const {
    return edge_count_;
  }

  bool IsDirected() const {
    return is_directed_;
  }

  virtual void AddEdge(const Vertex &start, const Vertex &finish) = 0;

  virtual bool HasEdge(const Vertex &start, const Vertex &finish) const = 0;

  virtual std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const = 0;
};

class GraphAdjMatrix : public Graph {
 private:
  std::vector<std::vector<int>> adj_matrix_;

 public:
  explicit GraphAdjMatrix(size_t vertex_count, bool is_directed)
      : Graph(vertex_count, is_directed),
        adj_matrix_(vertex_count + 1, std::vector<int>(vertex_count + 1, 0)) {}

  void AddEdge(const Vertex &start, const Vertex &finish) override {
    ++adj_matrix_[start][finish];
    if (!is_directed_) {
      ++adj_matrix_[finish][start];
    }
    ++edge_count_;
  }

  bool HasEdge(const Vertex &start, const Vertex &finish) const override {
    return adj_matrix_[start][finish] > 0;
  }

  std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const override {
    std::vector<Vertex> result;
    for (Vertex v = 1; v < vertex_count_ + 1; ++v) {
      if (HasEdge(vertex, v)) {
        result.push_back(v);
      }
    }
    return result;
  }
};

class WeightedGraphAdjMatrix : public GraphAdjMatrix {
 private:
  std::map<std::pair<Graph::Vertex, Graph::Vertex>, size_t> edge_weight_;

 public:
  explicit WeightedGraphAdjMatrix(size_t vertex_count, bool is_directed)
      : GraphAdjMatrix(vertex_count, is_directed) {}

  void AddEdge(const Vertex &start, const Vertex &finish, size_t weight) {
    GraphAdjMatrix::AddEdge(start, finish);
    edge_weight_[{start, finish}] = weight;
    if (!is_directed_) {
      edge_weight_[{finish, start}] = weight;
    }
  }

  size_t GetWeight(const Vertex &start, const Vertex &finish) const {
    return edge_weight_.at({start, finish});
  }
};

namespace GraphProcessing {

  struct MinimalSpanningTreeStatus {
    const size_t INF = std::numeric_limits<size_t>::max();
    const int NOT_SET = -1;
    std::set<std::pair<Graph::Vertex, Graph::Vertex>> MST;
    std::vector<size_t> min_neighbor_weight;
    std::vector<Graph::Vertex> parent;
    std::vector<bool> visited;

    explicit MinimalSpanningTreeStatus(size_t vertex_count)
        : min_neighbor_weight(vertex_count + 1, INF),
          parent(vertex_count + 1, NOT_SET),
          visited(vertex_count + 1, false) {}
  };

  std::set<std::pair<Graph::Vertex, Graph::Vertex>> Prim_Get_MST(const WeightedGraphAdjMatrix &graph) {
    const size_t size = graph.GetVertexCount() + 1;
    MinimalSpanningTreeStatus mst_status = MinimalSpanningTreeStatus(graph.GetVertexCount());
    mst_status.min_neighbor_weight[1] = 0;
    for (int i = 0; i < size - 1; ++i) {
      Graph::Vertex vertex = mst_status.NOT_SET;
      for (Graph::Vertex vertex_with_min_edge = 1; vertex_with_min_edge < size; ++vertex_with_min_edge) {
        if (!mst_status.visited[vertex_with_min_edge] && (vertex == mst_status.NOT_SET ||
            mst_status.min_neighbor_weight[vertex] > mst_status.min_neighbor_weight[vertex_with_min_edge])) {
          vertex = vertex_with_min_edge;
        }
      }
      mst_status.visited[vertex] = true;
      if (mst_status.parent[vertex] != mst_status.NOT_SET) {
        mst_status.MST.insert(std::minmax(mst_status.parent[vertex], vertex));
      }
      for (const Graph::Vertex &neighbor : graph.GetAllNeighbors(vertex)) {
        if (mst_status.min_neighbor_weight[neighbor] > graph.GetWeight(neighbor, vertex)) {
          mst_status.min_neighbor_weight[neighbor] = graph.GetWeight(neighbor, vertex);
          mst_status.parent[neighbor] = vertex;
        }
      }
    }
    return mst_status.MST;
  }
}

int main() {
  size_t n, m;

  std::cin >> n >> m;
  WeightedGraphAdjMatrix weighted_graph_adj_matrix = WeightedGraphAdjMatrix(n, false);

  for (int i = 0; i < m; ++i) {
    Graph::Vertex start, finish;
    size_t weight;
    std::cin >> start >> finish >> weight;
    weighted_graph_adj_matrix.AddEdge(start, finish, weight);
  }
  auto MST = GraphProcessing::Prim_Get_MST(weighted_graph_adj_matrix);

  size_t MST_weight = 0;
  for (const auto &edge : MST) {
    MST_weight += weighted_graph_adj_matrix.GetWeight(edge.first, edge.second);
  }
  std::cout << MST_weight;

  return 0;
}