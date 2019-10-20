/* Дана таблица, состоящая из N строк и M столбцов. В каждой клетке таблицы записано одно из чисел: 0 или 1.
 Расстоянием между клетками (x1, y1) и (x2, y2) назовем сумму |x1-x2|+|y1-y2|.
 Вам необходимо построить таблицу, в клетке (i, j) которой будет записано минимальное расстояние между
 клеткой (i, j) начальной таблицы и клеткой, в которой записана 1. Гарантируется, что хотя бы одна 1 в
 таблице есть.

Input format

В первой строке вводятся два натуральных числа N и M, не превосходящих 100. Далее идут N строк по M чисел
 — элементы таблицы.

Output format

Требуется вывести N строк по M чисел — элементы искомой таблицы.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

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

  std::vector<int> BFS(const Graph &graph, const std::vector<Graph::Vertex> &start_vertices) {
    const int DIST_NOT_SET = -1;
    std::vector<int> min_distances(graph.GetVertexCount() + 1, DIST_NOT_SET);
    std::queue<Graph::Vertex> queue;
    for (const Graph::Vertex vertex : start_vertices) {
      min_distances[vertex] = 0;
      queue.push(vertex);
    }
    while (!queue.empty()) {
      Graph::Vertex current_vertex = queue.front();
      queue.pop();
      for (auto neighbor : graph.GetAllNeighbors(current_vertex)) {
        if (min_distances[neighbor] == DIST_NOT_SET) {
          min_distances[neighbor] = min_distances[current_vertex] + 1;
          queue.push(neighbor);
        }
      }
    }
    return min_distances;
  }

  std::vector<int> GetMinDistances(const Graph &graph, const std::vector<Graph::Vertex> &start_vertices) {
    return BFS(graph, start_vertices);
  }
}

GraphAdjList MakeAdjList(size_t height, size_t width) {
  GraphAdjList graph_adj_list(width * height, false);
  for (int i = 1; i <= height; ++i) {
    for (int j = 1; j <= width - 1; ++j) {
      graph_adj_list.AddEdge((i - 1) * width + j, (i - 1) * width + j + 1);
    }
  }
  for (int i = 1; i <= height - 1; ++i) {
    for (int j = 1; j <= width; ++j) {
      graph_adj_list.AddEdge((i - 1) * width + j, i * width + j);
    }
  }
  return graph_adj_list;
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  GraphAdjList graph_adj_list = MakeAdjList(n, m);
  std::vector<Graph::Vertex> start_vertices;
  for (size_t i = 1; i <= n * m; ++i) {
    size_t input;
    std::cin >> input;
    if (input == 1) {
      start_vertices.push_back(i);
    }
  }

  std::vector<int> distances = GraphProcessing::GetMinDistances(graph_adj_list, start_vertices);
  for (size_t i = 1; i <= n * m; ++i) {
    std::cout << distances[i] << ' ';
    if (i % m == 0) {
      std::cout << std::endl;
    }
  }

  return 0;
}