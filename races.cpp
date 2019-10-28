#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

class Graph {
 protected:
  int vertex_count_;
  int edges_count_;
  bool is_directed_;

 public:
  struct Vertex {
    int row;
    int column;

    Vertex() : column(0), row(0) {}
    Vertex(int row, int column) : row(row), column(column) {}

    bool operator!=(const Vertex &other) {
      return column != other.column || row != other.row;
    }

    friend bool operator<(const Vertex &lhs, const Vertex &rhs) {
      return lhs.column < rhs.column ||
          (lhs.column == rhs.column && lhs.row < rhs.row);
    }
  };

  explicit Graph(int vertex_count, bool is_directed)
      : vertex_count_(vertex_count),
        is_directed_(is_directed),
        edges_count_(0) {}

  virtual void AddEdge(const Vertex &start, const Vertex &finish) = 0;

  int GetVertexCount() const {
    return vertex_count_;
  }

  virtual std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const = 0;
};

class GraphAdjList : public Graph {
 private:
  std::map<Vertex, std::vector<Vertex>> adj_list_;

 public:
  explicit GraphAdjList(int vertex_count, bool is_directed)
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
void InitializeMap(int n, int m, std::map<Graph::Vertex, T> &map, const T &value) {
  for (int i = 1; i < n + 1; ++i) {
    for (int j = 1; j < m + 1; ++j) {
      map[{i, j}] = value;
    }
  }
}

namespace GraphProcessing {

  const int NOT_SET = -1;

  void BFS(const Graph &graph, const Graph::Vertex &start, std::map<Graph::Vertex, int> &min_distances) {
    std::queue<Graph::Vertex> queue;
    queue.push(start);
    min_distances[start] = 0;
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
  }

  int GetMinDistanceToFinishVertex(const Graph &graph, std::map<Graph::Vertex, int> &min_distances,
                                   const Graph::Vertex &start, const Graph::Vertex &finish) {
    BFS(graph, start, min_distances);
    return min_distances[finish];
  }
}

int GetMinDistanceToFinishVertexForTable(const Graph &graph, const Graph::Vertex &start, const Graph::Vertex &finish,
                                         const size_t height, const size_t width) {
  std::map<Graph::Vertex, int> min_distances;
  InitializeMap(height, width, min_distances, GraphProcessing::NOT_SET);
  return GraphProcessing::GetMinDistanceToFinishVertex(graph, min_distances, start, finish);
}

void MoveLeft(int x_coord, int y_coord, int &delta_left, const std::vector<std::vector<int>> &matrix) {
  for (int k = x_coord; k > 0 && matrix[y_coord][k] != 0; --k) {
    --delta_left;
  }
}

void MoveRight(int x_coord, int y_coord, int &delta_right, const std::vector<std::vector<int>> &matrix) {
  for (int k = x_coord; k < matrix[0].size() && matrix[y_coord][k] != 0; ++k) {
    ++delta_right;
  }
}

void MoveUp(int x_coord, int y_coord, int &delta_up, const std::vector<std::vector<int>> &matrix) {
  for (int k = y_coord; k > 0 && matrix[k][x_coord] != 0; --k) {
    --delta_up;
  }
}

void MoveDown(int x_coord, int y_coord, int &delta_down, const std::vector<std::vector<int>> &matrix) {
  for (int k = y_coord; k < matrix.size() && matrix[k][x_coord] != 0; ++k) {
    ++delta_down;
  }
}

void MakeMove(int x_coord, int y_coord, Graph::Vertex &left, Graph::Vertex &right,
              Graph::Vertex &up, Graph::Vertex &down, const std::vector<std::vector<int>> &matrix) {
  MoveLeft(x_coord, y_coord, left.column, matrix);
  MoveRight(x_coord, y_coord, right.column, matrix);
  MoveUp(x_coord, y_coord, up.row, matrix);
  MoveDown(x_coord, y_coord, down.row, matrix);
}

std::vector<Graph::Vertex> MakeNeighbours(int x_coord, int y_coord, Graph::Vertex &left, Graph::Vertex &right,
                                          Graph::Vertex &up, Graph::Vertex &down) {
  std::vector<Graph::Vertex> neighbors;
  left.column = x_coord + left.column / 2;
  neighbors.push_back(left);
  right.column = x_coord + right.column / 2;
  neighbors.push_back(right);
  up.row = y_coord + up.row / 2;
  neighbors.push_back(up);
  down.row = y_coord + down.row / 2;
  neighbors.push_back(down);
  return neighbors;
}

GraphAdjList MakeAdjList(const std::vector<std::vector<int>> &matrix) {
  const int height = matrix.size();
  const int width = matrix[0].size();
  GraphAdjList graph_adj_list = GraphAdjList(height * width, true);
  for (int i = 1; i < height; ++i) {
    for (int j = 1; j < width; ++j) {
      Graph::Vertex vertex = {i, j};
      if (matrix[i][j] == 1) {
        Graph::Vertex left, right, up, down;
        left.row = right.row = i;
        up.column = down.column = j;
        MakeMove(j, i, left, right, up, down, matrix);
        std::vector<Graph::Vertex> neighbors = MakeNeighbours(j, i, left, right, up, down);
        for (auto neighbor : neighbors) {
          graph_adj_list.AddEdge(vertex, neighbor);
        }
      }
    }
  }
  return graph_adj_list;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<std::vector<int>> matrix(n + 1, std::vector<int>(m + 1, 0));

  char input;
  Graph::Vertex start, finish;
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      std::cin >> input;
      if (input == 'S') {
        start = {i, j};
      } else if (input == 'T') {
        finish = {i, j};
      }
      if (input != '#') {
        matrix[i][j] = 1;
      }
    }
  }
  GraphAdjList graph_adj_list = MakeAdjList(matrix);
  std::cout << GetMinDistanceToFinishVertexForTable(graph_adj_list, start, finish, n, m);

  return 0;
}
