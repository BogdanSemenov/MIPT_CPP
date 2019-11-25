/*Дан ориентированный невзвешенный граф. Необходимо определить есть ли в нём циклы, и если есть,
 то вывести любой из них.

Input format

В первой строке входного файла находятся два натуральных числа N и M (1 ≤ N ≤ 100000, M ≤ 100000)
 - количество вершин и рёбер в графе соответственно. Далее в M строках перечислены рёбра графа.
 Каждое ребро задаётся парой чисел - номерами начальной и конечной вершин соответственно.

Output format

Если в графе нет цикла, то вывести "NO", иначе - "YES" и затем перечислить все вершины в порядке обхода цикла.
 */

#include <iostream>
#include <vector>
#include <algorithm>

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

  typedef std::vector<Graph::Vertex> Cycle;

  enum Colors {
    WHITE,
    GRAY,
    BLACK
  };

  const Cycle GetCycle(std::vector<Graph::Vertex> &predecessors,
                                      const Graph::Vertex &start,
                                      const Graph::Vertex &finish) {
    Cycle cycle;
    Graph::Vertex temp_start = start;
    Graph::Vertex temp_finish = finish;
    while (temp_finish != temp_start) {
      cycle.push_back(temp_finish);
      temp_finish = predecessors[temp_finish];
    }
    cycle.push_back(start);
    return cycle;
  }

  bool DFS(const Graph &graph, std::vector<Colors> &visited, Cycle &cycle,
           std::vector<Graph::Vertex> &predecessors,
           const Graph::Vertex &vertex) {
    visited[vertex] = GRAY;
    for (auto neighbor : graph.GetAllNeighbors(vertex)) {
      if (visited[neighbor] == WHITE) {
        predecessors[neighbor] = vertex;
        if (DFS(graph, visited, cycle, predecessors, neighbor)) {
          return true;
        }
      } else if (visited[neighbor] == GRAY) {
        cycle = GetCycle(predecessors, neighbor, vertex);
        return true;
      }
    }
    visited[vertex] = BLACK;
    return false;
  }

  Cycle FindCycle(const Graph &graph) {
    const size_t NOT_SET = 0;
    std::vector<Colors> visited(graph.GetVertexCount() + 1, WHITE);
    std::vector<Graph::Vertex> predecessors(graph.GetVertexCount() + 1, NOT_SET);
    Cycle cycle;
    for (Graph::Vertex vertex = 1; vertex < graph.GetVertexCount() + 1; ++vertex) {
      if (visited[vertex] == WHITE) {
        if (DFS(graph, visited, cycle, predecessors, vertex)) {
          return {cycle.rbegin(), cycle.rend()};
        }
      }
    }
    return {};
  }
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  GraphAdjList graph_adj_list(n, true);

  for (int i = 0; i < m; ++i) {
    Graph::Vertex start, finish;
    std::cin >> start >> finish;
    graph_adj_list.AddEdge(start, finish);
  }

  auto cycle = GraphProcessing::FindCycle(graph_adj_list);

  if (cycle.empty()) {
    std::cout << "NO";
  } else {
    std::cout << "YES" << std::endl;
    for (auto vertex : cycle) {
      std::cout << vertex << ' ';
    }
  }

  return 0;
}