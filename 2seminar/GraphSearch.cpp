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

  size_t GetVertexCount() const {
    return vertex_count_;
  }

  size_t GetEdgeCount() const {
    return edge_count_;
  }

  bool GetDirection() const {
    return is_directed_;
  }

  virtual void AddEdge(const Vertex& start, const Vertex& finish) = 0;

};

class GraphAdjList : public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;

  void DFS(const Vertex& vertex, std::vector<bool>& used) const {
    used[vertex] = true;
    for (auto u : adj_list_[vertex]) {
      if (!used[u]) {
        DFS(u, used);
      }
    }
  }

 public:
  explicit GraphAdjList(size_t vertex_count, bool is_directed)
      : Graph(vertex_count, is_directed),
        adj_list_(vertex_count + 1) {}

  void AddEdge(const Vertex& start, const Vertex& finish) override {
    adj_list_[start].push_back(finish);
    if (!is_directed_) {
      adj_list_[finish].push_back(start);
    }
    ++edge_count_;
  }

  size_t CountComponentVertices(const Vertex& vertex) const {
    std::vector<bool> used(vertex_count_ + 1, false);
    DFS(vertex, used);
    size_t component_vertices_cnt = 0;
    for (Vertex v = 1; v < vertex_count_ + 1; v++) {
      if (used[v]) {
        ++component_vertices_cnt;
      }
    }
    return component_vertices_cnt;
  }
};

int main() {
  size_t n;
  Graph::Vertex vertex;
  std::cin >> n >> vertex;

  GraphAdjList graph_adj_list = GraphAdjList(n, false);
  size_t input;
  for (int i = 1; i < n + 1; ++i) {
    for (int j = 1; j < n + 1; ++j) {
      std::cin >> input;
      if (input == 1) {
        graph_adj_list.AddEdge(i, j);
      }
    }
  }

  std::cout << graph_adj_list.CountComponentVertices(vertex);

  return 0;
}
