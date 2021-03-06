/* Дан неориентированный граф. Требуется найти все точки сочленения в нём.

Входные данные

Первая строка входного файла содержит два натуральных числа n и m — количества вершин
 и рёбер графа соответственно (1 <= n <= 20000 , 1 <= m <= 200000 ).

Следующие m строк содержат описание рёбер по одному на строке.
 Ребро номер i описывается двумя натуральными числами b_i, e_i — номерами концов
 ребра (1 <= b_i, e_i <= n).

Выходные данные

Первая строка выходного файла должна содержать одно натуральное число b
 — количество точек сочленения в заданном графе.
 На следующей строке выведите b целых чисел — номера вершин, которые являются точками сочленения,
 в возрастающем порядке.
*/

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <string>

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

  virtual std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const = 0;

};

class GraphAdjList : public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;

 public:
  explicit GraphAdjList(size_t vertex_count, bool is_directed)
      : Graph(vertex_count, is_directed),
        adj_list_(vertex_count + 1) {}

  void AddEdge(const Vertex &start, const Vertex &finish) override {
    adj_list_[start].push_back(finish);
    if (!is_directed_) {
      adj_list_[finish].push_back(start);
    }
    ++edge_count_;
  }

  std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const override {
    return adj_list_[vertex];
  }
};

namespace GraphProcessing {

  struct VerticesCondition {
    std::vector<bool> visited;
    std::vector<size_t> time_in;
    std::vector<size_t> time_up;
    size_t time;
    std::set<Graph::Vertex> cut_vertices;

    explicit VerticesCondition(size_t vertex_count)
    :   visited(vertex_count + 1, false),
        time_in(vertex_count + 1, 0),
        time_up(vertex_count + 1, 0),
        time(0) {}
  };

  void DFS(const Graph &graph, const Graph::Vertex &vertex,
           VerticesCondition &vertices_condition, const Graph::Vertex &predecessor) {
    const bool is_root = (predecessor == vertex);
    vertices_condition.time_in[vertex] = vertices_condition.time_up[vertex] = ++vertices_condition.time;
    vertices_condition.visited[vertex] = true;
    size_t children = 0;
    for (Graph::Vertex u : graph.GetAllNeighbors(vertex)) {
      if (u == predecessor) {
        continue;
      }
      if (vertices_condition.visited[u]) {
        vertices_condition.time_up[vertex] = std::min(vertices_condition.time_up[vertex],
            vertices_condition.time_in[u]);
      } else {
        DFS(graph, u, vertices_condition, vertex);
        ++children;
        vertices_condition.time_up[vertex] = std::min(vertices_condition.time_up[vertex],
            vertices_condition.time_up[u]);
        if (!is_root && vertices_condition.time_up[u] >= vertices_condition.time_in[vertex]) {
          vertices_condition.cut_vertices.insert(vertex);
        }
      }
    }
    if (is_root && children >= 2) {
      vertices_condition.cut_vertices.insert(vertex);
    }
  }

  std::set<Graph::Vertex> GetCutVertices(const Graph &graph) {
    const size_t vertex_count = graph.GetVertexCount();
    VerticesCondition vertices_condition = VerticesCondition(vertex_count);
    for (Graph::Vertex vertex = 1; vertex < vertex_count + 1; ++vertex) {
      if (!vertices_condition.visited[vertex]) {
        DFS(graph, vertex, vertices_condition, vertex);
      }
    }
    return vertices_condition.cut_vertices;
  }
}

int main() {
  size_t n, m;

  std::cin >> n >> m;
  GraphAdjList graph_adj_list = GraphAdjList(n, false);

  for (int i = 0; i < m; ++i) {
    Graph::Vertex start, finish;
    std::cin >> start >> finish;
    graph_adj_list.AddEdge(start, finish);
  }

  auto cut_vertices = GraphProcessing::GetCutVertices(graph_adj_list);
  std::cout << cut_vertices.size() << std::endl;
  for (auto cut_vertex : cut_vertices) {
    std::cout << cut_vertex << std::endl;
  }

  return 0;
}
