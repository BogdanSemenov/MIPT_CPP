#include <iostream>
#include <vector>

enum Colors {
  WHITE,
  GREY,
  BLACK
};

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

  virtual void AddEdge(const Vertex& start, const Vertex& finish) = 0;

};

class GraphAdjList : public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;

  bool DFS(const Vertex& vertex, std::vector<Colors>& visited, std::vector<Vertex>& topsort) const {
    visited[vertex] = GREY;
    for (Vertex u : adj_list_[vertex]) {
      if (visited[u] == WHITE) {
        if (!DFS(u, visited, topsort)) {
          return false;
        }
      }
      else if (visited[u] == GREY) {
        return false;
      }
    }
    visited[vertex] = BLACK;
    topsort.push_back(vertex);
    return true;
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

  std::vector<Vertex> TopSort() const {
    std::vector<Vertex> topsort;
    std::vector<Colors> visited(vertex_count_ + 1, WHITE);

    for (Vertex vertex = 1; vertex < vertex_count_ + 1; ++vertex) {
      if (visited[vertex] == WHITE) {
        bool is_acyclic = DFS(vertex, visited, topsort);
        if (!is_acyclic) {
          return {};
        }
      }
    }
    return { topsort.rbegin(), topsort.rend() };
  }

};

int main() {
  size_t n, m;
  std::cin >> n >> m;

  GraphAdjList graph_adj_list = GraphAdjList(n, true);
  for (int i = 0; i < m; ++i) {
    size_t first, second;
    std::cin >> first >> second;
    graph_adj_list.AddEdge(first, second);
  }

  auto topsort = graph_adj_list.TopSort();

  if (topsort.empty()) {
    std::cout << "No";
  }
  else {
    std::cout << "Yes" << std::endl;
    for (auto vertex : topsort) {
      std::cout << vertex << ' ';
    }
  }

  return 0;
}





