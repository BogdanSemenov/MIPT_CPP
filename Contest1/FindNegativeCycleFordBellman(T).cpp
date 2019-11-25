/* Дан ориентированный граф. Определить, есть ли в нем цикл отрицательного веса, и если да, то вывести его.

Input format

В первой строке содержится число N (1 ≤ N ≤ 100) – количество вершин графа.
 В следующих N строках находится по N чисел – матрица смежности графа. Веса ребер по модулю меньше 100000.
 Если ребра нет, соответствующее значение равно 100000.

Output format

В первой строке выведите "YES", если цикл существует, или "NO", в противном случае.
 При наличии цикла выведите во второй строке количество вершин в нем (считая одинаковые – первую и последнюю),
 а в третьей строке – вершины, входящие в этот цикл, в порядке обхода. Если циклов несколько,
 то выведите любой из них.
 */

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

  typedef std::vector<Graph::Vertex> Cycle;

  void Relax(const Graph &graph, std::vector<int> &distances, std::vector<Graph::Vertex> &predecessors) {
    auto edges = graph.GetEdges();
    for (auto edge : edges) {
      if (distances[edge.to] > distances[edge.from] + edge.weight) {
        distances[edge.to] = distances[edge.from] + edge.weight;
        predecessors[edge.to] = edge.from;
      }
    }
  }

  std::vector<int> FordBellmanForAllVertices(const Graph &graph, std::vector<Graph::Vertex> &predecessors) {
    const int INF = std::numeric_limits<int>::max() / 2;
    std::vector<int> distances(graph.GetVertexCount() + 1, INF);

    for (int i = 1; i < graph.GetVertexCount(); ++i) {
      Relax(graph, distances, predecessors);
    }
    return distances;
  }

  Cycle BuildNegativeCycle(const Graph &graph, std::vector<Graph::Vertex> &predecessors,
                                                const Graph::Vertex &vertex) {
    Graph::Vertex current_vertex = vertex;
    std::vector<bool> visited(graph.GetVertexCount() + 1, false);
    while (!visited[current_vertex]) {
      visited[current_vertex] = true;
      current_vertex = predecessors[current_vertex];
    }
    Cycle negative_cycle;
    negative_cycle.push_back(current_vertex);
    Graph::Vertex cycle_end = predecessors[current_vertex];
    while (cycle_end != current_vertex) {
      negative_cycle.push_back(cycle_end);
      cycle_end = predecessors[cycle_end];
    }
    negative_cycle.push_back(cycle_end);
    return negative_cycle;
  }

  Cycle GetNegativeCycle(const Graph &graph) {
    const int NOT_SET = 0;
    std::vector<Graph::Vertex> predecessors(graph.GetVertexCount() + 1, NOT_SET);
    std::vector<int> distances = FordBellmanForAllVertices(graph, predecessors);
    std::vector<int> distances_after_relax = distances;
    Relax(graph, distances_after_relax, predecessors);
    Graph::Vertex cycle_vertex = NOT_SET;
    for (size_t i = 1; i < graph.GetVertexCount() + 1; ++i) {
      if (distances[i] != distances_after_relax[i]) {
        cycle_vertex = i;
        break;
      }
    }
    if (cycle_vertex == NOT_SET) {
      return {};
    }
    Cycle negative_cycle = BuildNegativeCycle(graph, predecessors, cycle_vertex);

    return {negative_cycle.rbegin(), negative_cycle.rend()};
  }
}

int main() {
  size_t vertex_num;
  const size_t NO_EDGE = 100000;

  std::cin >> vertex_num;
  GraphAdjList graph_adj_list = GraphAdjList(vertex_num, true);

  for (size_t from = 1; from <= vertex_num; ++from) {
    for (size_t to = 1; to <= vertex_num; ++to) {
      int weight;
      std::cin >> weight;
      if (weight != NO_EDGE) {
        graph_adj_list.AddEdge(from, to, weight);
      }
    }
  }

  auto negative_cycle = GraphProcessing::GetNegativeCycle(graph_adj_list);

  if (negative_cycle.empty()) {
    std::cout << "NO" << std::endl;
  } else {
    std::cout << "YES" << std::endl;
    std::cout << negative_cycle.size() << std::endl;
    for (const auto vertex : negative_cycle) {
      std::cout << vertex << ' ';
    }
  }

  return 0;
}