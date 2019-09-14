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
  explicit Graph(size_t vertex_count, size_t edge_count = 0, bool is_directed = false)
      : vertex_count_(vertex_count),
        is_directed_(is_directed),
        edge_count_(edge_count) {}

  size_t getVertexCount() const {
    return vertex_count_;
  }

  size_t getEdgeCount() const {
    return edge_count_;
  }

  bool getDirection() const {
    return is_directed_;
  }

  virtual void AddEdge(const Vertex& start, const Vertex& finish) = 0;

};

class GraphAdjList : public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;

 public:
  explicit GraphAdjList(size_t vertex_count)
      : Graph(vertex_count),
        adj_list_(vertex_count + 1) {}

  void AddEdge(const Vertex& start, const Vertex& finish) override {
    adj_list_[start].push_back(finish);
    if (!is_directed_) {
      adj_list_[finish].push_back(start);
    }
    ++edge_count_;
  }

  std::vector<Vertex> operator[](size_t input) const {
    return adj_list_[input];
  }
};

namespace GraphProcessing {
void FillEdge(Graph& g) {
  int n;
  size_t vertex_count = g.getVertexCount();
  for (int i = 1; i < vertex_count + 1; ++i) {
    for (int j = 1; j < vertex_count + 1; ++j) {
      std::cin >> n;
      if (n == 1) {
        g.AddEdge(i, j);
      }
    }
  }
}

bool BFS(const GraphAdjList& g, std::vector<int>& prev, const Graph::Vertex& start_vertex,
         const Graph::Vertex& finish_vertex) {
  std::vector<int> dist(g.getVertexCount() + 1, -1);
  std::queue<Graph::Vertex> q;
  Graph::Vertex curr = 0;

  dist[start_vertex] = 0;
  q.push(start_vertex);
  while (!q.empty()) {
    curr = q.front();
    q.pop();
    for (auto u : g[curr]) {
      if (dist[u] == -1) {
        dist[u] = dist[curr] + 1;
        q.push(u);
        prev[u] = curr;
      }
    }
  }
  return dist[finish_vertex] != -1;
}

std::vector<Graph::Vertex> FindPath(const GraphAdjList& g) {
  Graph::Vertex start_vertex, finish_vertex;
  std::cin >> start_vertex >> finish_vertex;
  std::vector<int> prev(g.getVertexCount() + 1, 0);

  bool is_path_existed = BFS(g, prev, start_vertex, finish_vertex);
  std::vector<Graph::Vertex> path;
  if (is_path_existed) {
    while (finish_vertex != start_vertex) {
      path.push_back(finish_vertex);
      finish_vertex = prev[finish_vertex];
    }
    path.push_back(start_vertex);
  }
  return path;
}

}


int main() {
  int m;
  std::cin >> m;

  GraphAdjList adj_list = GraphAdjList(m);
  GraphProcessing::FillEdge(adj_list);

  auto path = GraphProcessing::FindPath(adj_list);
  if (path.empty()) {
    std::cout << -1;
  }
  else {
    size_t size = path.size() - 1;
    std::cout << size << std::endl;
    if (size != 0) {
      for (auto it = path.rbegin(); it < path.rend(); ++it) {
        std::cout << *it << ' ';
      }
    }
  }
  return 0;
}