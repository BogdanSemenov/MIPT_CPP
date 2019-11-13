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

  typedef std::vector<std::vector<Graph::Vertex>> StronglyConnectedComponents;

  void DFS_TopSortedOrder(const Graph &graph, std::vector<bool> &visited, std::vector<Graph::Vertex> &topsorted_order,
                   const Graph::Vertex &vertex) {
    visited[vertex] = true;
    for (auto neighbor : graph.GetAllNeighbors(vertex)) {
      if (!visited[neighbor]) {
        DFS_TopSortedOrder(graph, visited, topsorted_order, neighbor);
      }
    }
    topsorted_order.push_back(vertex);
  }

  void DFS_GetSCC(const Graph &graph, std::vector<Graph::Vertex> &current_component,
                  std::vector<bool> &visited, const Graph::Vertex &vertex) {
    visited[vertex] = true;
    current_component.push_back(vertex);
    for (auto neighbor : graph.GetAllNeighbors(vertex)) {
      if (!visited[neighbor]) {
        DFS_GetSCC(graph, current_component, visited, neighbor);
      }
    }
  }

  std::vector<Graph::Vertex> GetTopSortedOrder(const Graph &graph) {
    std::vector<bool> visited(graph.GetVertexCount() + 1, false);
    std::vector<Graph::Vertex> topsorted_order;
    for (Graph::Vertex vertex = 1; vertex < graph.GetVertexCount() + 1; ++vertex) {
      if (!visited[vertex]) {
        DFS_TopSortedOrder(graph, visited, topsorted_order, vertex);
      }
    }
    return {topsorted_order.rbegin(), topsorted_order.rend()};
  }

  StronglyConnectedComponents GetSCC(const Graph &transposed_graph, const std::vector<Graph::Vertex> &topsorted_order) {
    const size_t vertex_count = transposed_graph.GetVertexCount();
    std::vector<bool> visited(vertex_count + 1, false);
    StronglyConnectedComponents scc;
    std::vector<Graph::Vertex> current_component;
    for (Graph::Vertex vertex : topsorted_order) {
      if (!visited[vertex]) {
        DFS_GetSCC(transposed_graph, current_component, visited, vertex);
        scc.push_back(current_component);
        current_component.clear();
      }
    }
    return scc;
  }

  StronglyConnectedComponents CondenseGraph(const Graph &graph) {
    std::vector<Graph::Vertex> topsorted_order = GetTopSortedOrder(graph);
    auto transposed_graph_ptr = graph.Transpose();
    return GetSCC(*transposed_graph_ptr, topsorted_order);
  }
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  GraphAdjList graph_adj_list(n, true);

  for (size_t i = 0; i < m; ++i) {
    Graph::Vertex start, finish;
    std::cin >> start >> finish;
    graph_adj_list.AddEdge(start, finish);
  }

  auto condense_graph = GraphProcessing::CondenseGraph(graph_adj_list);
  std::vector<size_t> vertex_position(n);
  for (size_t i = 0; i < condense_graph.size(); ++i) {
    for (Graph::Vertex vertex : condense_graph[i]) {
      vertex_position[vertex - 1] = i + 1;
    }
  }
  std::cout << condense_graph.size() << std::endl;
  for (auto position : vertex_position) {
    std::cout << position << ' ';
  }

  return 0;
}
