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

  bool BFS(std::vector<Vertex>& prev, const Vertex& start, const Vertex& finish) const {
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
    return dist[finish] != -1;
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

    bool path_exists = BFS(prev, start, finish);
    std::vector<Vertex> path;
    if (path_exists) {
      Vertex temp = finish;
      while (temp != 0) {
        path.push_back(temp);
        temp = prev[temp];
      }
    }
    return { path.rbegin(), path.rend() };
  }
};



int main() {
  size_t n;
  Graph::Vertex start, finish;
  std::cin >> n;

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
  std::cin >> start >> finish;
  auto min_path_vertices = graph_adj_list.FindMinPathVertices(start, finish);

  if (min_path_vertices.empty()) {
    std::cout << -1;
  }
  else {
    size_t size = min_path_vertices.size() - 1;
    std::cout << size << std::endl;
    if (size != 0) {
      for (auto i : min_path_vertices) {
        std::cout << i << ' ';
      }
    }
  }

  return 0;
}