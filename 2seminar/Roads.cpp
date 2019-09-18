#include <iostream>
#include <vector>

class Graph {
 protected:
  size_t vertex_count_;
  size_t edge_count_;
  bool is_directed_;

 public:
  typedef size_t Vertex;
  explicit Graph(size_t vertex_count, bool is_directed)
      : vertex_count_(vertex_count),
        is_directed_(is_directed),
        edge_count_(0) {}

  size_t GetVertexCount() const {
    return vertex_count_;
  }

  size_t GetEdgeCount() const {
    return edge_count_;
  }

  bool IsDirected() const {
    return is_directed_;
  }
};

class GraphAdjMatrix : public Graph {
 private:
  std::vector<std::vector<size_t>> adj_matrix_;

 public:
  explicit GraphAdjMatrix(const std::vector<std::vector<size_t>>& graph, bool is_directed)
      : Graph(graph.size() - 1, is_directed),
        adj_matrix_(graph) {
    for (int i = 1; i < vertex_count_ + 1; ++i) {
      for (int j = 1; j < vertex_count_ + 1; ++j) {
        edge_count_ += adj_matrix_[i][j];
      }
    }
    if (!is_directed) {
      edge_count_ /= 2;
    }
  }
};

int main() {
  int n;
  std::cin >> n;

  std::vector<std::vector<size_t>> adj_matrix(n + 1, std::vector<size_t>(n + 1));
  size_t input;
  for (int i = 1; i < n + 1; ++i) {
    for (int j = 1; j < n + 1; ++j) {
      std::cin >> input;
      adj_matrix[i][j] = input;
    }
  }
  GraphAdjMatrix graph_adj_matrix = GraphAdjMatrix(adj_matrix, false);
  std::cout << graph_adj_matrix.GetEdgeCount();

  return 0;
}