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

  bool GetDirection() const {
    return is_directed_;
  }

  virtual size_t GetVertexDeg(const Vertex& vertex) const = 0;
};

class GraphAdjList : public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;

 public:
  explicit GraphAdjList(const std::vector<std::vector<Vertex>>& graph, bool is_directed)
      : Graph(graph.size(), is_directed),
        adj_list_(graph) {
    for (int i = 1; i < vertex_count_ + 1; ++i) {
      edge_count_ += graph[i].size();
    }
    if (!is_directed_) {
      edge_count_ /= 2;
    }
  }

  size_t GetVertexDeg(const Vertex& vertex) const override {
    return adj_list_[vertex].size();
  }
};

int main() {
  size_t n, m;
  std::cin >> n >> m;

  std::vector<std::vector<Graph::Vertex>> adj_list(n + 1);
  Graph::Vertex first, second;
  for(int i = 0; i < m; ++i) {
    std::cin >> first >> second;
    adj_list[first].push_back(second);
    adj_list[second].push_back(first);
  }

  GraphAdjList graph_adj_list = GraphAdjList(adj_list, false);

  for (int i = 1; i < n + 1; ++i) {
    std::cout << graph_adj_list.GetVertexDeg(i) << ' ';
  }

  return 0;
}
