/*
 Вам задан ориентированный граф с N вершинами и M ребрами (1 ≤ N ≤ 20000, 1 ≤ M ≤ 200000).
 Найдите компоненты сильной связности заданного графа и топологически отсортируйте его конденсацию.

Input format

Граф задан во входном файле следующим образом: первая строка содержит числа N и M.
 Каждая из следующих M строк содержит описание ребра — два целых числа из диапазона от 1 до N
 — номера начала и конца ребра.

Output format

На первой строке выведите число K — количество компонент сильной связности в заданном графе.
 На следующей строке выведите N чисел — для каждой вершины выведите номер компоненты сильной связности,
 которой принадлежит эта вершина. Компоненты сильной связности должны быть занумерованы таким образом,
 чтобы для любого ребра номер компоненты сильной связности его начала не превышал номера компоненты сильной
 связности его конца.
 */

#include <set>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

class Graph {
 protected:
  size_t vertex_count_;
  size_t edges_count_;
  bool is_directed_;

 public:
  typedef size_t Vertex;

  explicit Graph(size_t vertex_count, bool is_directed)
      : vertex_count_(vertex_count),
        is_directed_(is_directed),
        edges_count_(0) {}

  virtual void AddEdge(const Vertex &start, const Vertex &finish) = 0;

  size_t GetVertexCount() const {
    return vertex_count_;
  }

  bool IsDirected() const {
    return is_directed_;
  }

  virtual std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const = 0;

  virtual std::shared_ptr<Graph> Transpose() const = 0;
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
  }

  std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const override {
    return adj_list_[vertex];
  }

  std::shared_ptr<Graph> Transpose() const override {
    auto graph_adj_list = std::make_shared<GraphAdjList>(vertex_count_, true);
    for (Vertex vertex = 1; vertex < vertex_count_ + 1; ++vertex) {
      for (auto neighbor : adj_list_[vertex]) {
        graph_adj_list->AddEdge(neighbor, vertex);
      }
    }
    return graph_adj_list;
  }
};

namespace GraphProcessing {

  struct StronglyConnectedComponents {
    std::vector<Graph::Vertex> components;
    size_t scc_count;
    explicit StronglyConnectedComponents(size_t num_vertices) : components(num_vertices + 1), scc_count(0) {}
  };

  void DFS_TopSort(const Graph &graph, std::vector<bool> &visited, std::vector<Graph::Vertex> &order,
                   const Graph::Vertex &vertex) {
    visited[vertex] = true;
    for (auto neighbor : graph.GetAllNeighbors(vertex)) {
      if (!visited[neighbor]) {
        DFS_TopSort(graph, visited, order, neighbor);
      }
    }
    order.push_back(vertex);
  }

  void DFS_GetSCC(const Graph &graph, StronglyConnectedComponents &strongly_connected_components,
                  std::vector<bool> &visited, const Graph::Vertex &vertex) {
    visited[vertex] = true;
    strongly_connected_components.components[vertex] = strongly_connected_components.scc_count;
    for (auto neighbor : graph.GetAllNeighbors(vertex)) {
      if (!visited[neighbor]) {
        DFS_GetSCC(graph, strongly_connected_components, visited, neighbor);
      }
    }
  }

  std::vector<Graph::Vertex> TopSort(const Graph &graph) {
    std::vector<bool> visited(graph.GetVertexCount() + 1, false);
    std::vector<Graph::Vertex> order;
    for (Graph::Vertex vertex = 1; vertex < graph.GetVertexCount() + 1; ++vertex) {
      if (!visited[vertex]) {
        DFS_TopSort(graph, visited, order, vertex);
      }
    }
    return {order.rbegin(), order.rend()};
  }

  StronglyConnectedComponents GetSCC(const Graph &transposed_graph, const std::vector<Graph::Vertex> &top_sort) {
    const size_t vertex_count = transposed_graph.GetVertexCount();
    std::vector<bool> visited(vertex_count + 1, false);
    StronglyConnectedComponents strongly_connected_components(vertex_count);
    for (Graph::Vertex vertex : top_sort) {
      if (!visited[vertex]) {
        ++strongly_connected_components.scc_count;
        DFS_GetSCC(transposed_graph, strongly_connected_components, visited, vertex);
      }
    }
    return strongly_connected_components;
  }

  StronglyConnectedComponents CondenseGraph(const Graph &graph) {
    std::vector<Graph::Vertex> top_sort = TopSort(graph);
    auto transposed_graph_ptr = graph.Transpose();

    return GetSCC(*transposed_graph_ptr, top_sort);
  }
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  GraphAdjList graph_adj_list(n, true);

  for (int i = 0; i < m; ++i) {
    Graph::Vertex start, finish;
    std::cin >> start >> finish;
    graph_adj_list.AddEdge(start, finish);
  }

  auto condense_graph = GraphProcessing::CondenseGraph(graph_adj_list);
  std::cout << condense_graph.scc_count << std::endl;
  for (int i = 1; i < condense_graph.components.size(); ++i) {
    std::cout << condense_graph.components[i] << ' ';
  }

  return 0;
}
