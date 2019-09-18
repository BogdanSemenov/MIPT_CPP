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

  std::vector<std::vector<Graph::Vertex>> adj_list(n + 1);
  size_t input;
  for (int i = 1; i < n + 1; ++i) {
    for (int j = 1; j < n + 1; ++j) {
      std::cin >> input;
      if (input == 1) {
        adj_list[i].push_back(j);
      }
    }
  }

  GraphAdjList graph_adj_list = GraphAdjList(adj_list, false);
  std::cout << graph_adj_list.CountComponentVertices(vertex);

  return 0;
}



