#include <iostream>
#include <vector>
#include <queue>

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

  void BFS(const Graph &graph, std::vector<Graph::Vertex> &prev, const Graph::Vertex &start) {
    const int UNVISITED_VERTEX = -1;
    std::queue<Graph::Vertex> q;
    std::vector<int> dist(graph.GetVertexCount() + 1, UNVISITED_VERTEX);
    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
      Graph::Vertex current_vertex = q.front();
      q.pop();
      for (auto u : graph.GetAllNeighbors(current_vertex)) {
        if (dist[u] == UNVISITED_VERTEX) {
          dist[u] = dist[current_vertex] + 1;
          q.push(u);
          prev[u] = current_vertex;
        }
      }
    }
  }

  std::vector<Graph::Vertex> FindMinPathVertices(const Graph &graph, const Graph::Vertex &start,
                                                 const Graph::Vertex &finish) {
    const size_t NOT_SET = 0;
    std::vector<int> dist(graph.GetVertexCount() + 1, -1);
    std::vector<Graph::Vertex> prev(graph.GetVertexCount() + 1, NOT_SET);

    BFS(graph, prev, start);
    std::vector<Graph::Vertex> path;
    Graph::Vertex temp = finish;
    while (temp != NOT_SET) {
      path.push_back(temp);
      temp = prev[temp];
    }
    return {path.rbegin(), path.rend()};
  }
}

Graph::Vertex IncreaseFirstDigit(Graph::Vertex vertex) {
  return vertex += 1000;
}

Graph::Vertex DecreaseLastDigit(Graph::Vertex vertex) {
  return --vertex;
}

Graph::Vertex LeftRotation(const Graph::Vertex &vertex) {
  return vertex / 1000 + 10 * (vertex % 1000);
}

Graph::Vertex RightRotation(const Graph::Vertex &vertex) {
  return vertex / 10 + 1000 * (vertex % 10);
}

std::vector<Graph::Vertex> GetNextVariants(const Graph::Vertex &vertex) {
  std::vector<Graph::Vertex> next_variants;
  next_variants.push_back(LeftRotation(vertex));
  next_variants.push_back(RightRotation(vertex));
  if (vertex % 10 != 1) {
    next_variants.push_back(DecreaseLastDigit(vertex));
  }
  if (vertex / 1000 != 9) {
    next_variants.push_back(IncreaseFirstDigit(vertex));
  }

  return next_variants;
}

GraphAdjList MakeAdjList() {
  const size_t MAX_SIZE = 10000;
  const Graph::Vertex MIN_VERTEX = 1000;

  GraphAdjList graph_adj_list = GraphAdjList(MAX_SIZE, true);
  for (Graph::Vertex vertex = MIN_VERTEX; vertex < MAX_SIZE; ++vertex) {
    auto next_variants = GetNextVariants(vertex);
    for (Graph::Vertex neighbor : next_variants) {
      graph_adj_list.AddEdge(vertex, neighbor);
    }
  }

  return graph_adj_list;
}

int main() {

  Graph::Vertex first, second;
  std::cin >> first >> second;

  GraphAdjList graph_adj_list = MakeAdjList();

  auto min_path_vertices = GraphProcessing::FindMinPathVertices(graph_adj_list, first, second);

  for (auto i : min_path_vertices) {
    std::cout << i << std::endl;
  }

  return 0;
}