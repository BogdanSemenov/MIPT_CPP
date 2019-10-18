/* Дан неориентированный невзвешенный граф. Необходимо посчитать количество его компонент связности и вывести их.

 Input format

Во входном файле записано два числа N и M (1 ≤ N ≤ 100000, 0 ≤ M ≤ 100000).
 В следующих M строках записаны по два числа i и j (1 ≤ i, j ≤ N), которые означают, что вершины i и j соединены
 ребром.

Output format

В первой строчке выходного файла выведите количество компонент связности. Далее выведите сами компоненты
 связности в следующем формате: в первой строке количество вершин в компоненте, во второй — сами вершины в
 произвольном порядке.
*/


#include <iostream>
#include <vector>
#include <algorithm>

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

  void DFS(const Graph &graph, std::vector<Graph::Vertex> &component, std::vector<bool> &visited,
           const Graph::Vertex &vertex) {
    visited[vertex] = true;
    component.push_back(vertex);
    for (auto neighbor : graph.GetAllNeighbors(vertex)) {
      if (!visited[neighbor]) {
        DFS(graph, component, visited, neighbor);
      }
    }
  }

  std::vector<std::vector<Graph::Vertex>> GetConnectedComponents(const Graph &graph) {
    std::vector<std::vector<Graph::Vertex>> connected_components;
    std::vector<Graph::Vertex> component;
    std::vector<bool> visited(graph.GetVertexCount() + 1, false);
    for (Graph::Vertex vertex = 1; vertex < graph.GetVertexCount() + 1; ++vertex) {
      if (!visited[vertex]) {
        component.clear();
        DFS(graph, component, visited, vertex);
        connected_components.push_back(component);
      }
    }

    return connected_components;
  }
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  GraphAdjList graph_adj_list(n, false);

  for (int i = 0; i < m; ++i) {
    Graph::Vertex start, finish;
    std::cin >> start >> finish;
    graph_adj_list.AddEdge(start, finish);
  }

  auto connected_components = GraphProcessing::GetConnectedComponents(graph_adj_list);

  std::cout << connected_components.size() << std::endl;
  for (const auto &component : connected_components) {
    std::cout << component.size() << std::endl;
    for (auto vertex : component) {
      std::cout << vertex << ' ';
    }
    std::cout << std::endl;
  }

  return 0;
}
