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

  virtual void AddEdge(const Vertex& start, const Vertex& finish) = 0;
};

class GraphAdjList : public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;

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

  size_t GetVertexDeg(const Vertex& vertex) const override {
    return adj_list_[vertex].size();
  }
};

int main() {
  size_t n, m;
  std::cin >> n >> m;

  GraphAdjList graph_adj_list = GraphAdjList(n, false);

  for(int i = 0; i < m; ++i) {
    Graph::Vertex first, second;
    std::cin >> first >> second;
    graph_adj_list.AddEdge(first, second);
  }

  for (int i = 1; i < n + 1; ++i) {
    std::cout << graph_adj_list.GetVertexDeg(i) << ' ';
  }

  return 0;
}
