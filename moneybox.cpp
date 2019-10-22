/* У Васи есть N свинок-копилок, свинки занумерованы числами от 1 до N.
 * Каждая копилка может быть открыта единственным соответствующим ей ключом или разбита.

Вася положил ключи в некоторые из копилок (он помнит, какой ключ лежит в какой из копилок).
 Теперь Вася собрался купить машину, а для этого ему нужно достать деньги из всех копилок.
 При этом он хочет разбить как можно меньшее количество копилок (ведь ему еще нужно копить деньги на квартиру,
 дачу, вертолет…). Помогите Васе определить, какое минимальное количество копилок нужно разбить.

Input format

В первой строке содержится число N — количество свинок-копилок (1 ≤ N ≤ 100).
 Далее идет N строк с описанием того, где лежит ключ от какой копилки: в i-ой из этих строк записан номер
 копилки, в которой находится ключ от i-ой копилки.

Output format

Выведите единственное число: минимальное количество копилок, которые необходимо разбить.
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
};

namespace GraphProcessing {

  void DFS_GraphSort(const Graph &graph, std::vector<bool> &visited, std::vector<Graph::Vertex> &sorted_graph,
                   const Graph::Vertex &vertex) {
    visited[vertex] = true;
    for (auto neighbor : graph.GetAllNeighbors(vertex)) {
      if (!visited[neighbor]) {
        DFS_GraphSort(graph, visited, sorted_graph, neighbor);
      }
    }
  
    sorted_graph.push_back(vertex);
  }
  
  void DFS(const Graph &graph, std::vector<bool> &visited, const Graph::Vertex &vertex) {
    visited[vertex] = true;
    for (auto neighbor : graph.GetAllNeighbors(vertex)) {
      if (!visited[neighbor]) {
        DFS(graph, visited, neighbor);
      }
    }
  }
  
  std::vector<Graph::Vertex> GraphSort(const Graph &graph) {
    if (!graph.IsDirected()) {
      return {};
    }
    std::vector<bool> visited(graph.GetVertexCount() + 1, false);
    std::vector<Graph::Vertex> sorted_graph;
    for (Graph::Vertex vertex = 1; vertex < graph.GetVertexCount() + 1; ++vertex) {
      if (!visited[vertex]) {
        DFS_GraphSort(graph, visited, sorted_graph, vertex);
      }
    }
  
    return {sorted_graph.rbegin(), sorted_graph.rend()};
  }
  
  size_t CountComponentsOfSortedGraph(const Graph &graph) {
    std::vector<bool> visited(graph.GetVertexCount() + 1, false);
    size_t dfs_visiting_counter = 0;
    for (Graph::Vertex vertex : GraphSort(graph)) {
      if (!visited[vertex]) {
        DFS(graph, visited, vertex);
        ++dfs_visiting_counter;
      }
    }
    return dfs_visiting_counter;
  }
}

int main() {
  size_t n;
  std::cin >> n;
  GraphAdjList graph_adj_list(n, true);
  for (Graph::Vertex finish = 1; finish <= n; ++finish) {
    Graph::Vertex start;
    std::cin >> start;
    graph_adj_list.AddEdge(start, finish);
  }

  std::cout << GraphProcessing::CountComponentsOfSortedGraph(graph_adj_list) << std::endl;

  return 0;
}
