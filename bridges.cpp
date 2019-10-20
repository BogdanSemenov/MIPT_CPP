/* Дан неориентированный граф. Требуется найти все мосты в нём.

Входные данные

Первая строка входного файла содержит два натуральных числа n и m — количества вершин
 и рёбер графа соответственно (1 ≤ n ≤ 20 000, 1 ≤ m ≤ 200 000).

Следующие m строк содержат описание рёбер по одному на строке.
 Ребро номер i описывается двумя натуральными числами b_i, e_i — номерами концов ребра
 (1 ≤ b_i, e_i ≤ n).

Выходные данные

Первая строка выходного файла должна содержать одно натуральное число b — количество мостов
 в заданном графе. На следующей строке выведите b целых чисел — номера рёбер,
 которые являются мостами, в возрастающем порядке. Рёбра нумеруются с единицы в том порядке,
 в котором они заданы во входном файле.
 */

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <string>

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

  struct GraphStatus {
    std::vector<bool> visited;
    std::vector<size_t> time_in;
    std::vector<size_t> time_up;
    size_t time;
    std::set<Graph::Edge> bridges;

    explicit GraphStatus(size_t vertex_count) :
        visited(vertex_count + 1, false),
        time_in(vertex_count + 1, 0),
        time_up(vertex_count + 1, 0),
        time(0) {}
  };

  void DFS(const Graph &graph, const Graph::Vertex &vertex, GraphStatus &graph_status,
           const std::map<Graph::Edge, int> &edge_index, const Graph::Vertex &predecessor) {
    graph_status.time_in[vertex] = graph_status.time_up[vertex] = ++graph_status.time;
    graph_status.visited[vertex] = true;
    for (Graph::Vertex u : graph.GetAllNeighbors(vertex)) {
      if (u == predecessor) {
        continue;
      }
      if (graph_status.visited[u]) {
        graph_status.time_up[vertex] = std::min(graph_status.time_up[vertex],
                                                graph_status.time_in[u]);
      } else {
        DFS(graph, u, graph_status, edge_index, vertex);
        graph_status.time_up[vertex] = std::min(graph_status.time_up[vertex],
                                                graph_status.time_up[u]);
        if (graph_status.time_up[u] > graph_status.time_in[vertex] && edge_index.at(std::minmax(vertex, u)) != -1) {
          graph_status.bridges.insert(std::minmax(vertex, u));
        }
      }
    }
  }

  std::set<Graph::Edge> GetBridges(const Graph &graph, const std::map<Graph::Edge, int> &edge_index) {
    const size_t vertex_count = graph.GetVertexCount();
    GraphStatus graph_status = GraphStatus(vertex_count);
    for (Graph::Vertex vertex = 1; vertex < vertex_count + 1; ++vertex) {
      if (!graph_status.visited[vertex]) {
        DFS(graph, vertex, graph_status, edge_index, vertex);
      }
    }
    return graph_status.bridges;
  }
}

int main() {
  size_t n, m;

  std::cin >> n >> m;
  GraphAdjList graph_adj_list = GraphAdjList(n, false);
  std::vector<Graph::Edge> edges;
  std::map<Graph::Edge, int> edge_index;

  for (int i = 1; i < m + 1; ++i) {
    Graph::Vertex from, to;
    std::cin >> from >> to;
    graph_adj_list.AddEdge(from, to);
    if (edge_index[std::minmax(from, to)] == 0) {
      edge_index[std::minmax(from, to)] = i;
    } else {
      edge_index[std::minmax(from, to)] = -1;
    }
    edges.emplace_back(std::minmax(from, to));
  }

  auto bridges = GraphProcessing::GetBridges(graph_adj_list, edge_index);
  std::cout << bridges.size() << std::endl;
  for (int i = 0; i < edges.size(); ++i) {
    if (bridges.find(edges[i]) != bridges.end()) {
      std::cout << i + 1 << ' ';
    }
  }

  return 0;
}
