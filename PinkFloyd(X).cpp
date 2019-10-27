/*Группа Pink Floyd собирается дать новый концертный тур по всему миру. По предыдущему опыту
 группа знает, что солист Роджер Уотерс постоянно нервничает при перелетах. На некоторых маршрутах
 он теряет вес от волнения, а на других — много ест и набирает вес.

Известно, что чем больше весит Роджер, тем лучше выступает группа, поэтому требуется спланировать
 перелеты так, чтобы вес Роджера на каждом концерте был максимально возможным. Группа должна посещать
 города в том же порядке, в котором она дает концерты. При этом между концертами группа может посещать
 промежуточные города.

Input format

Первая строка входного файла содержит три натуральных числа n, m и k — количество городов в мире,
 количество рейсов и количество концертов, которые должна дать группа соответственно (n ≤ 100, m ≤ 10^4,
 2 ≤ k ≤ 10^4). Города пронумерованы числами от 1 до n. Следующие m строк содержат описание рейсов,
 по одному на строке. Рейс номер i описывается тремя числами b_i, e_i и w_i — номер начального и
 конечного города рейса и предполагаемое изменение веса Роджера в миллиграммах (1 ≤ b_i, e_i ≤ n,
 −10^5 ≤ w_i ≤ 10^5). Последняя строка содержит числа a_1, a_2, ..., a_k — номера городов, в которых
 проводятся концерты. В начале концертного тура группа находится в городе a_1. Гарантируется, что
 группа может дать все концерты.

Output format

Первая строка выходного файла должна содержать число s — количество рейсов, которые должна
 сделать группа. Вторая строка должна содержать s чисел — номера используемых рейсов. Если
 существует такая последовательность маршрутов между концертами, что Роджер будет набирать вес
 неограниченно, то первая строка выходного файла должна содержать строку “infinitely kind”.
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

  virtual void AddEdge(const Vertex &start, const Vertex &finish, int weight = 1) = 0;

  virtual std::vector<Vertex> GetAllNeighbors(const Vertex &vertex) const = 0;

  virtual bool HasEdge(const Vertex &start, const Vertex &finish) const = 0;

  virtual int GetWeight(const Vertex &start, const Vertex &finish) const = 0;
};

class GraphAdjList : public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;
  std::map<Edge, size_t> edge_weight_;

 public:
  explicit GraphAdjList(size_t vertex_count, bool is_directed)
      : Graph(vertex_count, is_directed),
        adj_list_(vertex_count) {}

  void AddEdge(const Vertex &start, const Vertex &finish, int weight = 1) override {
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

  bool HasEdge(const Vertex &start, const Vertex &finish) const override {
    auto begin = adj_list_[start].begin();
    auto end = adj_list_[start].end();
    auto it = std::find(begin, end, finish);
    return it != end;
  }

  int GetWeight(const Vertex &start, const Vertex &finish) const override {
    return edge_weight_.at({start, finish});
  }
};

namespace GraphProcessing {

  const int INF = std::numeric_limits<int>::max();
  const int PREV_NOT_SET = -1;
  const int DIST_NOT_SET = 0;

  void Floyd(const Graph &graph, std::vector<std::vector<int>> &distance,
             std::vector<std::vector<Graph::Vertex>> &predecessor) {
    const size_t vertex_num = graph.GetVertexCount();
    for (size_t from = 0; from < vertex_num; ++from) {
      for (size_t to = 0; to < vertex_num; ++to) {
        if (graph.HasEdge(from, to)) {
          distance[from][to] = graph.GetWeight(from, to);
        } else {
          if (from != to) {
            distance[from][to] = INF;
          }
        }
      }
    }
    for (size_t k = 0; k < vertex_num; ++k) {
      for (size_t i = 0; i < vertex_num; ++i) {
        for (size_t j = 0; j < vertex_num; ++j) {
          if (distance[i][k] != INF && distance[k][j] != INF) {
            if (distance[i][j] > distance[i][k] + distance[k][j]) {
              distance[i][j] = distance[i][k] + distance[k][j];
              predecessor[i][j] = k;
            }
          }
        }
      }
    }
  }

  void BuildPath(const Graph &graph, std::vector<std::vector<Graph::Vertex>> &predecessor, std::vector<int> &path,
                 Graph::Vertex start, Graph::Vertex finish) {
    if (predecessor[start][finish] == PREV_NOT_SET) {
      path.push_back(start);
      return;
    }
    BuildPath(graph, predecessor, path, start, predecessor[start][finish]);
    BuildPath(graph, predecessor, path, predecessor[start][finish], finish);
  }

  std::vector<int> GetMinPath(const Graph &graph, const std::vector<Graph::Vertex> &concerts) {
    const size_t vertex_num = graph.GetVertexCount();
    std::vector<std::vector<int>> distance(vertex_num, std::vector<int>(vertex_num, DIST_NOT_SET));
    std::vector<std::vector<Graph::Vertex>> predecessor(vertex_num, std::vector<Graph::Vertex>(vertex_num, PREV_NOT_SET));
    Floyd(graph, distance, predecessor);
    std::vector<int> path;
    for (size_t i = 0; i < concerts.size() - 1; ++i) {
      Graph::Vertex first = concerts[i];
      Graph::Vertex second = concerts[i + 1];
      for (int j = 0; j < vertex_num; ++j) {
        if (distance[j][j] < 0 && distance[first][j] != INF && distance[j][second] != INF) {
          return {};
        }
      }
      BuildPath(graph, predecessor, path, first, second);
    }
    path.push_back(concerts.back());
    return path;
  }
}

int main() {
  size_t vertex_num, edge_num, concerts_num;
  std::cin >> vertex_num >> edge_num >> concerts_num;
  std::vector<Graph::Vertex> concerts(concerts_num);
  std::map<Graph::Edge, size_t> edge_flight_num;
  GraphAdjList graph_adj_list(vertex_num, true);
  for (size_t i = 0; i < edge_num; ++i) {
    Graph::Vertex start, finish;
    int weight;
    std::cin >> start >> finish >> weight;
    graph_adj_list.AddEdge(start - 1, finish - 1, -weight);
    edge_flight_num[{start - 1, finish - 1}] = i + 1;
  }

  for (size_t i = 0; i < concerts_num; ++i) {
    std::cin >> concerts[i];
    --concerts[i];
  }
  auto path = GraphProcessing::GetMinPath(graph_adj_list, concerts);
  if (path.empty()) {
    std::cout << "infinitely kind";
  } else {
    std::vector<size_t> flight_num;
    for (size_t i = 0; i < path.size() - 1; ++i) {
      if (path[i] == path[i + 1]) {
        continue;
      }
      flight_num.push_back(edge_flight_num[{path[i], path[i + 1]}]);
    }
    std::cout << flight_num.size() << std::endl;
    for (auto ans : flight_num) {
      std::cout << ans << ' ';
    }
  }

  return 0;
}