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
  typedef std::pair<Graph::Vertex, Graph::Vertex> Edge;

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

  virtual void AddEdge(const Vertex &start, const Vertex &finish, size_t weight = 1) = 0;

  virtual std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const = 0;

  virtual size_t GetWeight(const Vertex &start, const Vertex &finish) const = 0;
};

class GraphAdjList : public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;
  std::map<Edge, size_t> edge_weight_;

 public:
  explicit GraphAdjList(size_t vertex_count, bool is_directed)
      : Graph(vertex_count, is_directed),
        adj_list_(vertex_count) {}

  void AddEdge(const Vertex &start, const Vertex &finish, size_t weight = 1) override {
    adj_list_[start].push_back(finish);
    edge_weight_[{start, finish}] = weight;
    if (!is_directed_) {
      edge_weight_[{finish, start}] = weight;
      adj_list_[finish].push_back(start);
    }
    ++edge_count_;
  }

  std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const override {
    return adj_list_[vertex];
  }

  size_t GetWeight(const Vertex &start, const Vertex &finish) const override {
    return edge_weight_.at({start, finish});
  }
};

namespace GraphProcessing {

  const int INF = std::numeric_limits<int>::max();

  std::vector<std::vector<int>> GetMinPath_Floyd(const Graph &graph) {
    const size_t vertex_num = graph.GetVertexCount();
    std::vector<std::vector<int>> distance(vertex_num, std::vector<int>(vertex_num));
    for (Graph::Vertex from = 0; from < vertex_num; ++from) {
      for (Graph::Vertex to = 0; to < vertex_num; ++to) {
        distance[from][to] = graph.GetWeight(from, to);
      }
    }
    for (Graph::Vertex k = 0; k < vertex_num; ++k) {
      for (Graph::Vertex i = 0; i < vertex_num; ++i) {
        for (Graph::Vertex j = 0; j < vertex_num; ++j) {
          if (distance[i][k] < INF && distance[k][j] < INF) {
            if (distance[i][j] > distance[i][k] + distance[k][j]) {
              distance[i][j] = distance[i][k] + distance[k][j];
            }
          }
        }
      }
    }
    return distance;
  }
}

int main() {
  size_t vertex_num;
  Graph::Vertex start, finish;
  const int NO_EDGE = -1;
  std::cin >> vertex_num >> start >> finish;
  GraphAdjList graph_adj_list(vertex_num, true);
  for (size_t i = 0; i < vertex_num; ++i) {
    for (size_t j = 0; j < vertex_num; ++j) {
      int weight;
      std::cin >> weight;
      if (weight != NO_EDGE) {
        graph_adj_list.AddEdge(i, j, weight);
      } else {
        graph_adj_list.AddEdge(i, j, GraphProcessing::INF);
      }
    }
  }

  auto distances = GraphProcessing::GetMinPath_Floyd(graph_adj_list);
  const int distance = distances[start - 1][finish - 1];
  std::cout << (distance == GraphProcessing::INF ? -1 : distance);

  return 0;
}