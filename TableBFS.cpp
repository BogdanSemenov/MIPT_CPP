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
#include <map>

class Graph {
 protected:
  size_t vertex_count_;
  size_t edges_count_;
  bool is_directed_;

 public:
  struct Vertex {
    std::pair<size_t, size_t> vertex;

    Vertex() = default;
    Vertex(size_t first, size_t second) : vertex{first, second} {}
    explicit Vertex(size_t default_value) : vertex{default_value, default_value} {}

    bool operator!=(const Vertex &other) {
      return vertex != other.vertex;
    }

    friend bool operator<(const Vertex &lhs, const Vertex &rhs) {
      return lhs.vertex.first < rhs.vertex.first ||
          (lhs.vertex.first == rhs.vertex.first && lhs.vertex.second < rhs.vertex.second);
    }
  };

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
  std::map<Vertex, std::vector<Vertex>> adj_list_;

 public:
  explicit GraphAdjList(size_t vertex_count, bool is_directed)
      : Graph(vertex_count, is_directed) {}

  void AddEdge(const Vertex &start, const Vertex &finish) override {
    adj_list_[start].push_back(finish);
    if (!is_directed_) {
      adj_list_[finish].push_back(start);
    }
  }

  std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const override {
    return adj_list_.at(vertex);
  }
};

template<typename T>
void InitializeMap(size_t height, size_t vertex_num, std::map<Graph::Vertex, T> &map, const T &value) {
  for (size_t i = 1; i < height + 1; ++i) {
    for (size_t j = 1; j < vertex_num / height + 1; ++j) {
      map[{i, j}] = value;
    }
  }
}

namespace GraphProcessing {

  std::map<Graph::Vertex, int> BFS(const Graph &graph, const std::vector<Graph::Vertex> &start_vertices,
                                   size_t height) {
    const int NOT_SET = -1;
    std::map<Graph::Vertex, int> min_distances;
    InitializeMap(height, graph.GetVertexCount(), min_distances, NOT_SET);
    std::queue<Graph::Vertex> queue;
    for (const Graph::Vertex vertex : start_vertices) {
      min_distances[vertex] = 0;
      queue.push(vertex);
    }
    while (!queue.empty()) {
      Graph::Vertex vertex = queue.front();
      queue.pop();
      for (Graph::Vertex neighbor : graph.GetAllNeighbors(vertex)) {
        if (min_distances[neighbor] == NOT_SET) {
          min_distances[neighbor] = min_distances[vertex] + 1;
          queue.push(neighbor);
        }
      }
    }
    return min_distances;
  }

  std::vector<int> GetMinDistances(const Graph &graph, const std::vector<Graph::Vertex> &start_vertices,
                                   size_t height) {
    auto vertices_min_distances = BFS(graph, start_vertices, height);
    std::vector<int> distances;
    for (auto distance : vertices_min_distances) {
      distances.push_back(distance.second);
    }
    return distances;
  }
}

bool IsValid(const Graph::Vertex &vertex, size_t height, size_t width) {
  return std::min(vertex.vertex.first, vertex.vertex.second) >= 1
      && vertex.vertex.first <= height && vertex.vertex.second <= width;
}

GraphAdjList MakeAdjList(size_t height, size_t width) {
  GraphAdjList graph_adj_list(height * width, true);
  std::vector<int> delta_x = {0, 0, 1, -1};
  std::vector<int> delta_y = {1, -1, 0, 0};
  for (size_t i = 1; i <= height; ++i) {
    for (size_t j = 1; j <= width; ++j) {
      Graph::Vertex vertex{i, j};
      for (size_t k = 0; k < delta_x.size(); ++k) {
        Graph::Vertex neighbor{i + delta_x[k], j + delta_y[k]};
        if (IsValid(neighbor, height, width)) {
          graph_adj_list.AddEdge(vertex, neighbor);
        }
      }
    }
  }
  return graph_adj_list;
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  GraphAdjList graph_adj_list = MakeAdjList(n, m);
  std::vector<Graph::Vertex> start_vertices;
  for (size_t i = 1; i <= n; ++i) {
    for (size_t j = 1; j <= m; ++j) {
      size_t input;
      std::cin >> input;
      if (input == 1) {
        start_vertices.emplace_back(i, j);
      }
    }
  }

  std::vector<int> min_distances = GraphProcessing::GetMinDistances(graph_adj_list, start_vertices, n);
  for (size_t i = 0; i < n * m; ++i) {
    std::cout << min_distances[i] << ' ';
    if ((i + 1) % m == 0) {
      std::cout << std::endl;
    }
  }

  return 0;
}
