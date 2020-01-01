/* Вам дан неориентированный граф. Найдите кратчайший путь от вершины a до вершины b.

Input format

В первой строке входного файла идут целые числа n и m (1 ≤ n ≤ 50000, 1 ≤ m ≤ 100000) количества
 вершин и рёбер соответственно. Во второй строке идут целые числа a и b - стартовая и конечная вершины
 соответственно. Далее идут m строк, описывающих рёбра.

Output format

Если пути между a и b нет, выведите единственное число -1. Иначе выведите в первой строке
 число l - длину кратчайшего пути между этими двумя вершинами в рёбрах, а во второй строке выведите l + 1
 число - вершины этого пути.
*/


#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

class Graph {
 protected:
  size_t vertex_count_;
  size_t edges_count_;
  bool is_directed_;

 public:
  typedef size_t Vertex;

  explicit Graph(size_t vertex_count, bool is_directed)
      : vertex_count_(vertex_count),
        is_directed_(is_directed),
        edges_count_(0) {}

  virtual void AddEdge(const Vertex &start, const Vertex &finish) = 0;

  size_t GetVertexCount() const {
    return vertex_count_;
  }

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
  }

  std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const override {
    return adj_list_[vertex];
  }
};

namespace GraphProcessing {

  bool BFS(const Graph &graph, std::vector<Graph::Vertex> &predecessors, const Graph::Vertex &start,
      const Graph::Vertex &finish) {
    const int DIST_NOT_SET = -1;
    std::vector<size_t> dist(graph.GetVertexCount() + 1, DIST_NOT_SET);
    std::queue<Graph::Vertex> queue;
    queue.push(start);
    dist[start] = 0;

    while (!queue.empty()) {
      Graph::Vertex vertex = queue.front();
      queue.pop();
      for (auto neighbor : graph.GetAllNeighbors(vertex)) {
        if (dist[neighbor] == DIST_NOT_SET) {
          dist[neighbor] = dist[vertex] + 1;
          predecessors[neighbor] = vertex;
          queue.push(neighbor);
        }
      }
    }
      return dist[finish] != DIST_NOT_SET;
  }

  std::vector<Graph::Vertex> GetMinPath(const Graph &graph, const Graph::Vertex &start,
      const Graph::Vertex &finish) {
    const int PREV_NOT_SET = 0;
    std::vector<Graph::Vertex> predecessors(graph.GetVertexCount() + 1, PREV_NOT_SET);
    bool path_exists = BFS(graph, predecessors, start, finish);

    std::vector<Graph::Vertex> min_path;
    if (path_exists) {
      Graph::Vertex vertex = finish;
      while (vertex != PREV_NOT_SET) {
        min_path.push_back(vertex);
        vertex = predecessors[vertex];
      }
    }

    return {min_path.rbegin(), min_path.rend()};
  }
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  GraphAdjList graph_adj_list(n, false);

  Graph::Vertex start, finish;
  std::cin >> start >> finish;

  for (int i = 0; i < m; ++i) {
    Graph::Vertex first, second;
    std::cin >> first >> second;
    graph_adj_list.AddEdge(first, second);
  }

  auto min_path = GraphProcessing::GetMinPath(graph_adj_list, start, finish);
  if (min_path.empty()) {
    std::cout << -1;
  } else {
    std::cout << min_path.size() - 1 << std::endl;
    for (auto vertex : min_path) {
      std::cout << vertex << ' ';
    }
  }

  return 0;
}
