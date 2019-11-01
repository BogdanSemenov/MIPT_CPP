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

struct SquareMove {
  int dx;
  int dy;

  static std::vector<SquareMove> MakeSquareMoves() {
    return {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
  }
};

class Graph {
 protected:
  size_t vertex_count_;
  size_t edges_count_;
  bool is_directed_;

 public:
  struct Vertex {
    size_t row;
    size_t column;

    Vertex() : column(0), row(0) {}
    Vertex(size_t row, size_t column) : row(row), column(column) {}

    bool operator!=(const Vertex &other) {
      return column != other.column || row != other.row;
    }

    friend bool operator<(const Vertex &lhs, const Vertex &rhs) {
      return lhs.row < rhs.row ||
          (lhs.row == rhs.row && lhs.column < rhs.column);
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
void InitializeMap(size_t height, size_t width, std::map<Graph::Vertex, T> &map, const T &value) {
  for (size_t i = 1; i <= height; ++i) {
    for (size_t j = 1; j <= width; ++j) {
      map[{i, j}] = value;
    }
  }
}

namespace GraphProcessing {

  const int NOT_SET = -1;

  std::map<Graph::Vertex, int> BFS(const Graph &graph, const std::vector<Graph::Vertex> &start_vertices,
                                   std::map<Graph::Vertex, int> &min_distances) {
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

  std::vector<int> GetMinDistances(const Graph &graph, std::map<Graph::Vertex, int> &min_distances,
                                   const std::vector<Graph::Vertex> &start_vertices) {
    BFS(graph, start_vertices, min_distances);
    std::vector<int> distances;
    for (auto distance : min_distances) {
      distances.push_back(distance.second);
    }
    return distances;
  }
}

std::vector<int> GetMinDistancesForTable(const Graph &graph, const std::vector<Graph::Vertex> &start_vertices,
                                         const size_t height, const size_t width) {
  std::map<Graph::Vertex, int> min_distances;
  InitializeMap(height, width, min_distances, GraphProcessing::NOT_SET);
  return GraphProcessing::GetMinDistances(graph, min_distances, start_vertices);

}

bool IsValid(const Graph::Vertex &vertex, const size_t height, const size_t width) {
  return std::min(vertex.column, vertex.row) >= 1
      && vertex.column <= width && vertex.row <= height;
}

GraphAdjList MakeAdjList(size_t height, size_t width) {
  GraphAdjList graph_adj_list(height * width, true);
  auto square_moves = SquareMove::MakeSquareMoves();
  for (size_t i = 1; i <= height; ++i) {
    for (size_t j = 1; j <= width; ++j) {
      Graph::Vertex vertex{i, j};
      for (auto square_move : square_moves) {
        Graph::Vertex neighbor{i + square_move.dy, j + square_move.dx};
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

  std::vector<int> min_distances = GetMinDistancesForTable(graph_adj_list, start_vertices, n, m);
  for (size_t i = 0; i < n * m; ++i) {
    std::cout << min_distances[i] << ' ';
    if ((i + 1) % m == 0) {
      std::cout << std::endl;
    }
  }

  return 0;
}
