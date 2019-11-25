/*Дан ориентированный невзвешенный граф. Необходимо его топологически отсортировать.

Input format

В первой строке входного файла даны два целых числа N и M (1 ≤ N ≤ 100000, 0 ≤ M ≤ 100000) -
 количества вершин и рёбер в графе соответственно. Далее в M строках перечислены рёбра графа.
 Каждое ребро задаётся парой чисел - номерами начальной и конечной вершин соответственно.

Output format

Вывести любую топологическую сортировку графа в виде последовательности номеров вершин.
 Если граф невозможно топологически отсортировать, вывести -1.
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

enum Colors {
  WHITE,
  GRAY,
  BLACK
};

bool DFS_IsCyclic(const Graph &graph, std::vector<Colors> &visited, const Graph::Vertex &vertex) {
  visited[vertex] = GRAY;
  for (auto neighbor : graph.GetAllNeighbors(vertex)) {
    if (visited[neighbor] == WHITE) {
      if (DFS_IsCyclic(graph, visited, neighbor)) {
        return true;
      }
    } else if (visited[neighbor] == GRAY) {
      return true;
    }
  }
  visited[vertex] = BLACK;
  return false;
}

void DFS_TopSort(const Graph &graph, std::vector<bool> &visited, std::vector<Graph::Vertex> &topsort,
                 const Graph::Vertex &vertex) {
  visited[vertex] = true;
  for (auto neighbor : graph.GetAllNeighbors(vertex)) {
    if (!visited[neighbor]) {
      DFS_TopSort(graph, visited, topsort, neighbor);
    }
  }

  topsort.push_back(vertex);
}

bool IsCyclic(const Graph &graph) {
  std::vector<Colors> visited(graph.GetVertexCount() + 1, WHITE);
  for (Graph::Vertex vertex = 1; vertex < graph.GetVertexCount() + 1; ++vertex) {
    if (visited[vertex] == WHITE) {
      if (DFS_IsCyclic(graph, visited, vertex)) {
        return true;
      }
    }
  }

  return false;
}

std::vector<Graph::Vertex> TopSort(const Graph &graph) {
  if (!graph.IsDirected() || IsCyclic(graph)) {
    return {};
  }
  std::vector<bool> visited(graph.GetVertexCount() + 1, false);
  std::vector<Graph::Vertex> topsort;
  for (Graph::Vertex vertex = 1; vertex < graph.GetVertexCount() + 1; ++vertex) {
    if (!visited[vertex]) {
      DFS_TopSort(graph, visited, topsort, vertex);
    }
  }

  return {topsort.rbegin(), topsort.rend()};
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

  auto topsort = GraphProcessing::TopSort(graph_adj_list);

  if (topsort.empty()) {
    std::cout << -1;
  } else {
    for (auto vertex : topsort) {
      std::cout << vertex << ' ';
    }
  }

  return 0;
}