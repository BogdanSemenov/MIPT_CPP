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

  void BFS(std::vector<Vertex>& prev, const Vertex& start, const Vertex& finish) const {
    std::vector<int> dist(vertex_count_ + 1, -1);
    std::queue<Vertex> q;
    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
      Vertex curr = q.front();
      q.pop();
      for (auto u : adj_list_[curr]) {
        if (dist[u] == -1) {
          dist[u] = dist[curr] + 1;
          q.push(u);
          prev[u] = curr;
        }
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

  std::vector<Vertex> FindMinPathVertices(const Vertex& start, const Vertex& finish) const {
    std::vector<Vertex> prev(vertex_count_ + 1, 0);

    BFS(prev, start, finish);
    std::vector<Vertex> path;
    Vertex temp = finish;
    while (temp != 0) {
      path.push_back(temp);
      temp = prev[temp];
    }
    return { path.rbegin(), path.rend() };
  }
};

std::vector<Graph::Vertex> GetNextVariants(const Graph::Vertex& vertex) {
  std::vector<Graph::Vertex> next_variants;
  size_t temp = vertex;
  next_variants.push_back(vertex / 10 + 1000 * (vertex % 10));
  next_variants.push_back(vertex / 1000 + 10 * (vertex % 1000));
  if (vertex % 10 != 1) {
    next_variants.push_back(--temp);
  }
  temp = vertex;
  if (vertex / 1000 != 9) {
    temp += 1000;
    next_variants.push_back(temp);
  }

  return next_variants;
}

void FillAdjList(std::vector<std::vector<Graph::Vertex>>& adj_list, const Graph::Vertex& first,
                 const Graph::Vertex& second, const size_t MAX_SIZE) {
  std::vector<bool> visited(MAX_SIZE, false);
  std::queue<Graph::Vertex> q;
  q.push(first);
  while (!visited[second]) {
    size_t curr = q.front();
    q.pop();
    auto next_variants = GetNextVariants(curr);
    visited[curr] = true;
    adj_list[curr] = next_variants;
    for (auto neighbor : adj_list[curr]) {
      if (!visited[neighbor]) {
        next_variants = GetNextVariants(neighbor);
        visited[neighbor] = true;
        adj_list[neighbor] = next_variants;
        q.push(neighbor);
      }
    }
  }
}

int main() {
  const size_t MAX_SIZE = 10000;
  Graph::Vertex first, second;
  std::cin >> first >> second;

  std::vector<std::vector<Graph::Vertex>> adj_list(MAX_SIZE);

  FillAdjList(adj_list, first, second, MAX_SIZE);

  GraphAdjList graph_adj_list = GraphAdjList(adj_list, false);

  auto min_path_vertices = graph_adj_list.FindMinPathVertices(first, second);

  for (auto i : min_path_vertices) {
    std::cout << i << std::endl;
  }

  return 0;
}