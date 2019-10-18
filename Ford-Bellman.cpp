#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <limits>

class Graph {
 protected:
  size_t vertex_count_;
  size_t edge_count_;
  bool is_directed_;

 public:
  typedef size_t Vertex;

  struct Edge {
    Vertex from;
    Vertex to;
    int weight;

    Edge(const Vertex &from, const Vertex &to, int weight) :
        from(from), to(to), weight(weight) {}

    bool operator<(const Edge &other) const {
      return weight < other.weight;
    }
  };

  explicit Graph(size_t vertex_count, bool is_directed)
      : vertex_count_(vertex_count),
        is_directed_(is_directed),
        edge_count_(0) {}

  size_t GetVertexCount() const {
    return vertex_count_;
  }

  virtual void AddEdge(const Vertex &start, const Vertex &finish, int weight) = 0;

  virtual std::vector<Edge> GetEdges() const = 0;
};

class GraphAdjList : public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;
  std::vector<Edge> edges_;

 public:
  explicit GraphAdjList(size_t vertex_count, bool is_directed)
      : Graph(vertex_count, is_directed),
        adj_list_(vertex_count + 1) {}

  void AddEdge(const Vertex &start, const Vertex &finish, int weight) override {
    adj_list_[start].push_back(finish);
    edges_.emplace_back(start, finish, weight);
    if (!is_directed_) {
      adj_list_[finish].push_back(start);
      edges_.emplace_back(finish, start, weight);
    }
    ++edge_count_;
  }

  std::vector<Edge> GetEdges() const override {
    return edges_;
  }
};


namespace GraphProcessing {

std::vector<size_t> Ford_Bellman(const Graph &graph, const Graph::Vertex &vertex) {
    const size_t MAX = 30000;
    std::vector<size_t> distances(graph.GetVertexCount() + 1, MAX);
    distances[vertex] = 0;

    for (int i = 1; i <= graph.GetVertexCount(); ++i) {
      auto edges = graph.GetEdges();
      for (auto edge : edges) {
        if (distances[edge.from] != MAX && distances[edge.to] > distances[edge.from] + edge.weight) {
          distances[edge.to] = distances[edge.from] + edge.weight;
        }
      }
    }

  return distances;
  }
}

int main() {
  size_t n, m;

  std::cin >> n >> m;
  GraphAdjList graph_adj_list = GraphAdjList(n, true);

  for (int i = 0; i < m; ++i) {
    Graph::Vertex start, finish;
    size_t weight;
    std::cin >> start >> finish >> weight;
    graph_adj_list.AddEdge(start, finish, weight);
  }

  auto distances = GraphProcessing::Ford_Bellman(graph_adj_list, 1);

  for (int i = 1; i < distances.size(); ++i) {
    std::cout << distances[i] << ' ';
  }

  return 0;
}