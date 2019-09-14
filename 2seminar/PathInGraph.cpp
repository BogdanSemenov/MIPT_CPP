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
  explicit Graph(size_t vertex_count = 0, size_t edge_count = 0, bool is_directed = false)
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

  explicit GraphAdjList()
      : Graph() {}

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

int IncreaseFirstDigit(int number) {
  const size_t thousand = 1000;
  int temp = number;
  if (number / thousand != 9) {
    temp += thousand;
  }
  return temp;
}

int DecreaseLastDigit(int number) {
  const size_t ten = 10;
  int temp = number;
  if (number % ten != 1) {
    ++temp;
  }
  return temp;
}

int RightRotation(int number) {
  const size_t ten = 10;
  const size_t thousand = 1000;
  int temp = number / ten + thousand * (number / thousand);
  return temp;
}

int LeftRotation(int number) {
  const size_t ten = 10;
  const size_t thousand = 1000;
  int temp = number / thousand + ten * (number / ten);
  return temp;
}

void FillEdge(Graph& g) {
  int first_number, second_number;
  std::cin >> first_number >> second_number;

  std::vector<int> numbers;
  numbers.push_back(first_number);

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
}


int main() {
  GraphAdjList adj_list = GraphAdjList();
  GraphProcessing::FillEdge(adj_list);

  return 0;
}
