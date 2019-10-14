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

  virtual std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const = 0;

  virtual size_t GetWeight(const Vertex &start, const Vertex &finish) const = 0;
};

class GraphAdjList : public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;
  std::map<Edge, size_t> edge_weight_;

 public:
  explicit GraphAdjList(size_t vertex_count, bool is_directed)
      : Graph(vertex_count, is_directed),
        adj_list_(vertex_count + 1) {}

  void AddEdge(const Vertex &start, const Vertex &finish, size_t weight = 1) override {
    adj_list_[start].push_back(finish);
    edge_weight_[{start, finish}] = weight;
    if (!is_directed_) {
      edge_weight_[{finish, start}] = weight;
      adj_list_[finish].push_back(start);
    }
    ++edge_count_;
  }

  std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const override {
    return adj_list_[vertex];
  }

  size_t GetWeight(const Vertex &start, const Vertex &finish) const override {
    return edge_weight_.at({start, finish});
  }
};

namespace GraphProcessing {

  struct VerticesCondition {
    const size_t INF = std::numeric_limits<size_t>::max();
    const int NOT_SET = -1;
    std::set<Graph::Edge> MST;
    std::vector<size_t> min_weight;
    std::vector<Graph::Vertex> parent;
    std::vector<bool> visited;
    std::set<std::pair<size_t, Graph::Vertex>> edge_weight_queue;

    explicit VerticesCondition(size_t vertex_count)
        : min_weight(vertex_count + 1, INF),
          visited(vertex_count + 1, false),
          parent(vertex_count + 1, NOT_SET) {
      min_weight[1] = 0; // for vertex 1 weight = 0
      edge_weight_queue.insert({0, 1});
    }
  };

  std::set<Graph::Edge> Prim_Get_MST(const Graph &graph) {
    VerticesCondition vertices_condition = VerticesCondition(graph.GetVertexCount());
    while (!vertices_condition.edge_weight_queue.empty()) {
      auto[dist, vertex] = *vertices_condition.edge_weight_queue.begin();
      vertices_condition.edge_weight_queue.erase(vertices_condition.edge_weight_queue.begin());
      vertices_condition.visited[vertex] = true;
      if (vertices_condition.parent[vertex] != vertices_condition.NOT_SET) {
        vertices_condition.MST.insert(std::minmax(vertices_condition.parent[vertex], vertex));
      }
      for (const auto &neighbor : graph.GetAllNeighbors(vertex)) {
        if (!vertices_condition.visited[neighbor] && vertices_condition.min_weight[neighbor] >
        graph.GetWeight(neighbor, vertex)) {
          vertices_condition.edge_weight_queue.erase({vertices_condition.min_weight[neighbor], neighbor});
          vertices_condition.parent[neighbor] = vertex;
          vertices_condition.min_weight[neighbor] = graph.GetWeight(neighbor, vertex);
          vertices_condition.edge_weight_queue.insert({vertices_condition.min_weight[neighbor], neighbor});
        }
      }
    }
    return vertices_condition.MST;
  }
  // Time Complexity: O(E*logV). We can find the minimum edge in O(logV) time.
}

int main() {
  size_t n, m;

  std::cin >> n >> m;
  GraphAdjList graph_adj_list = GraphAdjList(n, false);

  for (int i = 0; i < m; ++i) {
    Graph::Vertex start, finish;
    size_t weight;
    std::cin >> start >> finish >> weight;
    graph_adj_list.AddEdge(start, finish, weight);
  }
  auto MST = GraphProcessing::Prim_Get_MST(graph_adj_list);

  size_t MST_weight = 0;
  for (const auto &edge : MST) {
    MST_weight += graph_adj_list.GetWeight(edge.first, edge.second);
  }
  std::cout << MST_weight;
  return 0;
}
