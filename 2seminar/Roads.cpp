#include <iostream>
#include <vector>

class Graph {
 protected:
  size_t vertex_count_;
  size_t edge_count_;
  bool is_directed_;

 public:
  typedef size_t Vertex;
  explicit Graph(size_t vertex_count, bool is_directed = false)
      : vertex_count_(vertex_count),
        is_directed_(is_directed),
        edge_count_(0) {}

  size_t getVertexCount() const {
    return vertex_count_;
  }

  size_t getEdgeCount() const {
    return edge_count_;
  }

  bool getDirection() const {
    return is_directed_;
  }

  virtual void AddEdge(const Vertex& start, const Vertex& finish) = 0;
};

class GraphAdjMatrix : public Graph {
 private:
  std::vector<std::vector<bool>> adj_matrix_;

 public:
  explicit GraphAdjMatrix(size_t vertex_count)
      : Graph(vertex_count),
        adj_matrix_(vertex_count_ + 1, std::vector<bool>(vertex_count_ + 1, false)) {}

  void AddEdge(const Vertex& start, const Vertex& finish) override {
    adj_matrix_[start][finish] = true;
    if (!is_directed_) {
      adj_matrix_[finish][start] = true;
    }
  }

  bool operator()(const Vertex& start, const Vertex& finish) const {
    return adj_matrix_[start][finish];
  }

};

namespace GraphProcessing {
void FillEdge(Graph& g) {
  int input;
  int vertex_count = g.getVertexCount();
  for (int i = 1; i < vertex_count + 1; ++i) {
    for (int j = 1; j < vertex_count + 1; ++j) {
      std::cin >> input;
      if (input == 1) {
        g.AddEdge(i, j);
      }
    }
  }
}

size_t CountRoads(GraphAdjMatrix& g) {
  size_t counter = 0;
  int vertex_count = g.getVertexCount();
  for (int i = 1; i < vertex_count + 1; ++i) {
    for (int j = 1; j < i; ++j) {
      if (g(i, j) == 1) {
        ++counter;
      }
    }
  }
  return counter;
}

}

int main() {
  int m;
  std::cin >> m;
  GraphAdjMatrix adj_matrix = GraphAdjMatrix(m);
  GraphProcessing::FillEdge(adj_matrix);
  std::cout << GraphProcessing::CountRoads(adj_matrix);

  return 0;
}