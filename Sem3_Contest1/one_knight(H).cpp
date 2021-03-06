/* На шахматной доске NxN в клетке (x1, y1) стоит голодный шахматный конь.
 Он хочет попасть в клетку (x2, y2), где растет вкусная шахматная трава.
 Какое наименьшее количество ходов он должен для этого сделать?

Input format

На вход программы поступает пять чисел: N, x1, y1, x2, y2 (5 ≤ N ≤ 20, 1 ≤ x1, y1, x2, y2 ≤ N).
 Левая верхняя клетка доски имеет координаты (1, 1), правая нижняя — (N, N).

Output format

В первой строке выведите единственное число K — наименьшее необходимое число ходов коня.
 В каждой из следующих K+1 строк должно быть записано 2 числа — координаты очередной клетки в пути коня.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

struct KnightMove {
  int dx;
  int dy;

  static std::vector<KnightMove> MakeKnightMoves() {
    return {{2, 1}, {2, -1}, {-2, 1}, {-2 ,-1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
  }
};

class Graph {
 protected:
  size_t vertex_count_;
  size_t edges_count_;
  bool is_directed_;

 public:
  struct Vertex {
    size_t column;
    size_t row;

    Vertex() = default;
    Vertex(size_t column, size_t row) : column(column), row(row) {}
    explicit Vertex(size_t default_value) : column(default_value), row(default_value) {}

    bool operator!=(const Vertex &other) {
      return column != other.column || row != other.row;
    }

    friend bool operator<(const Vertex &lhs, const Vertex &rhs) {
      return lhs.column < rhs.column ||
          (lhs.column == rhs.column && lhs.row < rhs.row);
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
void InitializeMap(size_t vertex_count, std::map<Graph::Vertex, T> &map, const T &value) {
  for (size_t i = 1; i < vertex_count + 1; ++i) {
    for (size_t j = 1; j < vertex_count + 1; ++j) {
      map[{i, j}] = value;
    }
  }
}

namespace GraphProcessing {

  void BFS(const Graph &graph, std::map<Graph::Vertex, Graph::Vertex> &predecessors,
           const Graph::Vertex &start) {
    const int NOT_SET = -1;
    std::map<Graph::Vertex, int> dist;
    InitializeMap(graph.GetVertexCount(), dist, NOT_SET);
    std::queue<Graph::Vertex> queue;
    queue.push(start);
    dist[start] = 0;
    while (!queue.empty()) {
      Graph::Vertex vertex = queue.front();
      queue.pop();
      for (Graph::Vertex neighbor : graph.GetAllNeighbors(vertex)) {
        if (dist[neighbor] == NOT_SET) {
          dist[neighbor] = dist[vertex] + 1;
          queue.push(neighbor);
          predecessors[neighbor] = vertex;
        }
      }
    }
  }

  std::vector<Graph::Vertex> GetMinPath(const Graph &graph, const Graph::Vertex &start,
                                        const Graph::Vertex &finish) {
    const Graph::Vertex NOT_SET(0);
    std::map<Graph::Vertex, Graph::Vertex> predecessors;
    InitializeMap(graph.GetVertexCount(), predecessors, NOT_SET);
    BFS(graph, predecessors, start);
    std::vector<Graph::Vertex> min_path;
    Graph::Vertex vertex = finish;
    while (vertex != NOT_SET) {
      min_path.push_back(vertex);
      vertex = predecessors[vertex];
    }
    return {min_path.rbegin(), min_path.rend()};
  }
}

bool IsValid(const Graph::Vertex &vertex, size_t table_size) {
  return std::min(vertex.column, vertex.row) >= 1
      && std::max(vertex.column, vertex.row) <= table_size;
}

GraphAdjList MakeAdjList(size_t vertex_num) {
  GraphAdjList graph_adj_list(vertex_num, true);
  auto knight_moves = KnightMove::MakeKnightMoves();
  for (size_t i = 1; i <= vertex_num; ++i) {
    for (size_t j = 1; j <= vertex_num; ++j) {
      Graph::Vertex vertex{i, j};
      for (auto knight_move : knight_moves) {
        Graph::Vertex neighbor{i + knight_move.dx, j + knight_move.dy};
        if (IsValid(neighbor, vertex_num)) {
          graph_adj_list.AddEdge(vertex, neighbor);
        }
      }
    }
  }
  return graph_adj_list;
}

int main() {
  size_t n;
  std::cin >> n;
  GraphAdjList graph_adj_list = MakeAdjList(n);

  size_t x1, y1, x2, y2;
  std::cin >> x1 >> y1 >> x2 >> y2;

  Graph::Vertex start = {x1, y1};
  Graph::Vertex finish = {x2, y2};

  auto min_path = GraphProcessing::GetMinPath(graph_adj_list, start, finish);
  if (min_path.empty()) {
    std::cout << -1;
  } else {
    std::cout << min_path.size() - 1 << std::endl;
    for (auto vertex : min_path) {
      std::cout << vertex.column << ' ' << vertex.row << std::endl;
    }
  }

  return 0;
}
