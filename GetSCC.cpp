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
    auto graph_adj_list = std::make_shared<GraphAdjList>(vertex_count_, is_directed_);
    std::vector<std::vector<Vertex>> temp(vertex_count_ + 1);

    for (Vertex vertex = 1; vertex < vertex_count_ + 1; ++vertex) {
      const size_t size = adj_list_[vertex].size();
      for (auto neighbor : adj_list_[vertex]) {
        temp[neighbor].push_back(vertex);
      }
    }
    graph_adj_list->adj_list_ = temp;

    return graph_adj_list;
  }
};

namespace GraphProcessing {

void DFS_EnumerateVertices(const Graph &graph, std::vector<bool> &visited, std::vector<Graph::Vertex> &order,
    const Graph::Vertex &vertex) {
  visited[vertex] = true;
  for (auto neighbor : graph.GetAllNeighbors(vertex)) {
    if (!visited[neighbor]) {
      DFS_EnumerateVertices(graph, visited, order, neighbor);
    }
  }
  order.push_back(vertex);
}

void DFS_GetSCC(const Graph &graph, std::vector<bool> &visited, std::set<Graph::Vertex> &component,
const Graph::Vertex &vertex) {
  visited[vertex] = true;
  component.insert(vertex);
  for (auto neighbor : graph.GetAllNeighbors(vertex)) {
    if (!visited[neighbor]) {
      DFS_GetSCC(graph, visited, component, neighbor);
    }
  }
}

std::vector<Graph::Vertex> EnumerateVertices(const Graph &graph) {
  std::vector<bool> visited(graph.GetVertexCount() + 1, false);
  std::vector<Graph::Vertex> order;

  for (Graph::Vertex vertex = 1; vertex < graph.GetVertexCount() + 1; ++vertex) {
    if (!visited[vertex]) {
      DFS_EnumerateVertices(graph, visited, order, vertex);
    }
  }

  return {order.rbegin(), order.rend()};
}

std::vector<std::set<Graph::Vertex>> GetSCC(const Graph &graph) {
  std::vector<bool> visited(graph.GetVertexCount() + 1, false);
  std::vector<Graph::Vertex> descending_order_vertices = EnumerateVertices(graph);
  auto transposed_graph_ptr = graph.Transpose();
  std::set<Graph::Vertex> component;
  std::vector<std::set<Graph::Vertex>> strongly_connected_components;
  for (Graph::Vertex vertex : descending_order_vertices) {
    if (!visited[vertex]) {
      component.clear();
      DFS_GetSCC(*transposed_graph_ptr, visited, component, vertex);
      strongly_connected_components.push_back(component);
    }
  }
  return strongly_connected_components;
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

  auto strongly_connected_components = GraphProcessing::GetSCC(graph_adj_list);
  const size_t scc_cnt = strongly_connected_components.size();
  const size_t vertex_count = graph_adj_list.GetVertexCount() + 1;

  std::cout << scc_cnt << std::endl;
  for (Graph::Vertex vertex = 1; vertex < vertex_count; ++vertex) {
    for (int i = 0; i < scc_cnt; ++i) {
      if (strongly_connected_components[i].find(vertex) != strongly_connected_components[i].end()) {
        std::cout << i + 1 << ' ';
      }
    }
  }

  return 0;
}