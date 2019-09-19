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

  virtual void AddEdge(const Vertex& start, const Vertex& finish) = 0;
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

  std::vector<Vertex> FindMinPathVertices(const Vertex& start, const Vertex& finish) const {
    std::vector<Vertex> prev(vertex_count_ + 1, 0);

    bool path_exists = BFS(prev, start, finish);
    std::vector<Vertex> path;
    if (path_exists) {
      Vertex temp = finish;
      const size_t NOT_PREV_ELEMENT = 0;
      while (temp != NOT_PREV_ELEMENT) {
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

  GraphAdjList graph_adj_list = GraphAdjList(n, false);

  for (int i = 1; i < n + 1; ++i) {
    for (int j = 1; j < n + 1; ++j) {
      size_t input;
      std::cin >> input;
      if (input == 1) {
        graph_adj_list.AddEdge(i, j);
      }
    }
  }

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