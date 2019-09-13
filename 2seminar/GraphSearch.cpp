#include <iostream>
#include <vector>

class Graph {
 protected:
  size_t vertex_count_;
  size_t edge_count_;
  bool is_directed_;

 public:
  typedef size_t Vertex;
  explicit Graph(size_t vertex_count, size_t edge_count = 0, bool is_directed = false)
      : vertex_count_(vertex_count),
        is_directed_(is_directed),
        edge_count_(edge_count) {}

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

class GraphAdjList : public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;

 public:
  explicit GraphAdjList(size_t vertex_count)
      : Graph(vertex_count),
        adj_list_(vertex_count + 1) {}

  void AddEdge(const Vertex& start, const Vertex& finish) override {
    adj_list_[start].push_back(finish);
    if (!is_directed_) {
      adj_list_[finish].push_back(start);
    }
    ++edge_count_;
  }

  std::vector<Vertex> operator[](size_t input) const {
    return adj_list_[input];
  }
};

namespace GraphProcessing {
void FillEdge(Graph& g) {
  int n;
  size_t vertex_count = g.getVertexCount();
  for (int i = 1; i < vertex_count + 1; ++i) {
    for (int j = 1; j < vertex_count + 1; ++j) {
      std::cin >> n;
      if (n == 1) {
        g.AddEdge(i, j);
      }
    }
  }
}

void DFS_visit(int vertex, const GraphAdjList& g, std::vector<bool>& used) {
  used[vertex] = true;
  for (auto i : g[vertex]) {
    if (!used[i]) {
      DFS_visit(i, g, used);
    }
  }
}

size_t CountVertex(const GraphAdjList& g, size_t vertex) {
  std::vector<bool> used(g.getVertexCount() + 1, false);
  DFS_visit(vertex, g, used);
  size_t counter = 0;
  for (auto i : used) {
    counter += i;
  }
  return counter;
}

}

int main() {
  int m, vertex;
  std::cin >> m >> vertex;

  GraphAdjList adj_list = GraphAdjList(m);
  GraphProcessing::FillEdge(adj_list);
  std::cout << GraphProcessing::CountVertex(adj_list, vertex);

  return 0;
}



