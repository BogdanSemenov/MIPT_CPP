/*Напишите программу, которая будет находить расстояния в неориентированном взвешенном графе с
 неотрицательными длинами ребер, от указанной вершины до всех остальных. Программа должна работать
 быстро для больших разреженных графов.

Input format

В первой строке входных данных задано число NUM — количество различных запусков алгоритма Дейкстры
 (на разных графах). Далее следуют NUM блоков, каждый из которых имеет следующую структуру.

Первая строка блока содержит два числа N и M, разделенные пробелом — количество вершин и количество
 ребер графа. Далее следуют M строк, каждая из которых содержит по три целых числа, разделенные пробелами.
 Первые два из них в пределах от 0 до N–1 каждое и обозначают концы соответствующего ребра, третье —
 в пределах от 0 до 20000 и обозначает длину этого ребра. Далее, в последней строке блока, записанное
 единственное число от 0 до N–1 — вершина, расстояния от которой надо искать.

Количество различных графов в одном тесте NUM не превышает 5. Количество вершин не превышает 60000,
 рёбер — 200000.

Output format

Выведите на стандартный выход (экран) NUM строк, в каждой из которых по N_i чисел, разделенных
 пробелами — расстояния от указанной начальной вершины взвешенного неориентированного графа до
 его 0-й, 1-й, 2-й и т. д. вершин (допускается лишний пробел после последнего числа). Если некоторая
 вершина недостижима от указанной начальной, вместо расстояния выводите число 2009000999
 (гарантировано, что все реальные расстояния меньше).

Examples
Input       Output
1        18 0 5 2 8
5 7
1 2 5
1 3 2
2 3 4
2 4 3
3 4 6
0 3 20
0 4 10
1
*/

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <map>
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

  const size_t INF = std::numeric_limits<size_t>::max();

  struct Distance_Vertex {
    size_t distance;
    Graph::Vertex vertex;

    Distance_Vertex(size_t distance, Graph::Vertex vertex)
    : distance(distance),
      vertex(vertex) {}

    bool operator<(const Distance_Vertex &other) const {
      return distance < other.distance;
    }

    bool operator>(const Distance_Vertex &other) const {
      return distance > other.distance;
    }
  };

  struct DijkstraInfo {
    std::vector<size_t> min_distance;
    std::vector<bool> visited;
    std::priority_queue<Distance_Vertex, std::vector<Distance_Vertex>, std::greater<>> priority_queue;

    explicit DijkstraInfo(size_t vertex_count, const Graph::Vertex &start)
        : min_distance(vertex_count, INF),
          visited(vertex_count, false) {
      min_distance[start] = 0;
      priority_queue.push({0, start});
    }
  };

  void Relax(const Graph &graph, DijkstraInfo &dijkstra_info, const Graph::Vertex &neighbor,
             const Graph::Vertex &vertex) {
    size_t weight = graph.GetWeight(vertex, neighbor);
    if (dijkstra_info.min_distance[neighbor] > weight + dijkstra_info.min_distance[vertex]) {
      dijkstra_info.min_distance[neighbor] = weight + dijkstra_info.min_distance[vertex];
      dijkstra_info.priority_queue.push({dijkstra_info.min_distance[neighbor], neighbor});
    }
  }

  std::vector<size_t> GetMinDistances_Dijkstra(const Graph &graph, const Graph::Vertex &start) {
    DijkstraInfo dijkstra_info(graph.GetVertexCount(), start);
    while (!dijkstra_info.priority_queue.empty()) {
      Graph::Vertex vertex = dijkstra_info.priority_queue.top().vertex;
      dijkstra_info.priority_queue.pop();
      if (dijkstra_info.visited[vertex]) {
        continue;
      }
      dijkstra_info.visited[vertex] = true;
      for (const auto &neighbor : graph.GetAllNeighbors(vertex)) {
        Relax(graph, dijkstra_info, neighbor, vertex);
      }
    }
    return dijkstra_info.min_distance;
  }
}

int main() {
  size_t num;
  std::cin >> num;
  size_t NOT_REACHABLE = 2009000999;
  for (size_t i = 0; i < num; ++i) {
    size_t vertices_num, edges_num;
    std::cin >> vertices_num >> edges_num;
    GraphAdjList graph_adj_list = GraphAdjList(vertices_num, false);

    for (size_t j = 0; j < edges_num; ++j) {
      Graph::Vertex start, finish;
      size_t weight;
      std::cin >> start >> finish >> weight;
      graph_adj_list.AddEdge(start, finish, weight);
    }
    Graph::Vertex vertex;
    std::cin >> vertex;
    auto min_distance = GraphProcessing::GetMinDistances_Dijkstra(graph_adj_list, vertex);
    for (auto dist : min_distance) {
      if (dist == GraphProcessing::INF) {
        std::cout << NOT_REACHABLE << ' ';
      } else {
        std::cout << dist << ' ';
      }
    }
    std::cout << std::endl;
  }

  return 0;
}
