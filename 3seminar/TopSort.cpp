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

  virtual void AddEdge(const Vertex &start, const Vertex &finish) = 0;

  virtual std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const = 0;

};

class GraphAdjList : public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;

 public:
  explicit GraphAdjList(size_t vertex_count, bool is_directed)
      : Graph(vertex_count, is_directed),
        adj_list_(vertex_count + 1) {}

  void AddEdge(const Vertex &start, const Vertex &finish) override {
    adj_list_[start].push_back(finish);
    if (!is_directed_) {
      adj_list_[finish].push_back(start);
    }
    ++edge_count_;
  }

  std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const override {
    return adj_list_[vertex];
  }
};

namespace GraphProcessing {

enum Colors {
  WHITE,
  GREY,
  BLACK
};

bool DFS(const Graph &graph, const Graph::Vertex &vertex, std::vector<Colors> &visited,
         std::vector<Graph::Vertex> &topsort) {
  visited[vertex] = GREY;
  for (Graph::Vertex u : graph.GetAllNeighbors(vertex)) {
    if (visited[u] == WHITE) {
      if (!DFS(graph, u, visited, topsort)) {
        return false;
      }
    } else if (visited[u] == GREY) {
      return false;
    }
  }
  visited[vertex] = BLACK;
  topsort.push_back(vertex);
  return true;
}

std::vector<Graph::Vertex> TopSort(const Graph &graph) {
  if (!graph.IsDirected()) {
    return {};
  }
  std::vector<Graph::Vertex> topsort;
  std::vector<Colors> visited(graph.GetVertexCount() + 1, WHITE);

  for (Graph::Vertex vertex = 1; vertex < graph.GetVertexCount() + 1; ++vertex) {
    if (visited[vertex] == WHITE) {
      bool is_acyclic = DFS(graph, vertex, visited, topsort);
      if (!is_acyclic) {
        return {};
      }
    }
  }
  return {topsort.rbegin(), topsort.rend()};
}
}

int main() {
  size_t n, m;
  std::cin >> n >> m;

  GraphAdjList graph_adj_list = GraphAdjList(n, true);
  for (int i = 0; i < m; ++i) {
    size_t first, second;
    std::cin >> first >> second;
    graph_adj_list.AddEdge(first, second);
  }

  auto topsort = GraphProcessing::TopSort(graph_adj_list);

  if (topsort.empty()) {
    std::cout << "No";
  } else {
    std::cout << "Yes" << std::endl;
    for (auto vertex : topsort) {
      std::cout << vertex << ' ';
    }
  }

  return 0;
}
