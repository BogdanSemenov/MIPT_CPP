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
  typedef std::pair<Graph::Vertex, Graph::Vertex> Edge;

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

  virtual void AddEdge(const Vertex &start, const Vertex &finish, size_t weight = 1) = 0;

  virtual bool HasEdge(const Vertex &start, const Vertex &finish) const = 0;

  virtual size_t GetWeight(const Vertex &start, const Vertex &finish) const = 0;

  virtual std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const = 0;
};

class GraphAdjMatrix : public Graph {
 private:
  std::vector<std::vector<int>> adj_matrix_;

 public:
  explicit GraphAdjMatrix(size_t vertex_count, bool is_directed)
      : Graph(vertex_count, is_directed),
        adj_matrix_(vertex_count + 1, std::vector<int>(vertex_count + 1, 0)) {}

  void AddEdge(const Vertex &start, const Vertex &finish, size_t weight = 1) override {
    adj_matrix_[start][finish] = weight;
    if (!is_directed_) {
      adj_matrix_[finish][start] = weight;
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

  size_t GetWeight(const Vertex &start, const Vertex &finish) const override {
    return adj_matrix_[start][finish];
  }
};

namespace GraphProcessing {

  struct VerticesCondition {
    const size_t INF = std::numeric_limits<size_t>::max();
    const int NOT_SET = -1;
    std::vector<Graph::Edge> MST;
    std::vector<size_t> min_weight;
    std::vector<Graph::Vertex> parent;
    std::vector<bool> visited;

    explicit VerticesCondition(size_t vertex_count)
        : min_weight(vertex_count + 1, INF),
          visited(vertex_count + 1, false),
          parent(vertex_count + 1, NOT_SET) {
      min_weight[1] = 0; // for vertex 1 weight = 0
    }
  };

  std::vector<Graph::Edge> Prim_Get_MST(const Graph &graph) {
    const size_t vertices_cnt = graph.GetVertexCount() + 1;
    VerticesCondition vertices_condition = VerticesCondition(graph.GetVertexCount());
    for (int i = 0; i < vertices_cnt - 1; ++i) {
      Graph::Vertex vertex_with_min_edge = vertices_condition.NOT_SET;
      for (Graph::Vertex vertex = 1; vertex < vertices_cnt; ++vertex) {
        if (!vertices_condition.visited[vertex] && (vertex_with_min_edge == vertices_condition.NOT_SET ||
            vertices_condition.min_weight[vertex_with_min_edge] > vertices_condition.min_weight[vertex])) {
          vertex_with_min_edge = vertex;
        }
      }
      vertices_condition.visited[vertex_with_min_edge] = true;
      if (vertices_condition.parent[vertex_with_min_edge] != vertices_condition.NOT_SET) {
        vertices_condition.MST.emplace_back(std::minmax(vertices_condition.parent[vertex_with_min_edge],
            vertex_with_min_edge));
      }
      for (const Graph::Vertex &neighbor : graph.GetAllNeighbors(vertex_with_min_edge)) {
        if (vertices_condition.min_weight[neighbor] > graph.GetWeight(neighbor, vertex_with_min_edge)) {
          vertices_condition.min_weight[neighbor] = graph.GetWeight(neighbor, vertex_with_min_edge);
          vertices_condition.parent[neighbor] = vertex_with_min_edge;
        }
      }
    }
    return vertices_condition.MST;
  }
}

int main() {
  size_t n, m;

  std::cin >> n >> m;
  GraphAdjMatrix graph_adj_matrix = GraphAdjMatrix(n, false);

  for (int i = 0; i < m; ++i) {
    Graph::Vertex start, finish;
    size_t weight;
    std::cin >> start >> finish >> weight;
    graph_adj_matrix.AddEdge(start, finish, weight);
  }
  auto MST = GraphProcessing::Prim_Get_MST(graph_adj_matrix);

  size_t MST_weight = 0;
  for (const auto &edge : MST) {
    MST_weight += graph_adj_matrix.GetWeight(edge.first, edge.second);
  }
  std::cout << MST_weight;

  return 0;
}
