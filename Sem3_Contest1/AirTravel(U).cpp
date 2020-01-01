/*Профессору Форду необходимо попасть на международную конференцию. Он хочет потратить на дорогу наименьшее
 количество денег, поэтому решил, что будет путешествовать исключительно ночными авиарейсами (чтобы не
 тратиться на ночевку в отелях), а днем будет осматривать достопримечательности тех городов, через которые
 он будет проезжать транзитом. Он внимательно изучил расписание авиаперелетов и составил набор подходящих
 авиарейсов, выяснив, что перелеты на выбранных направлениях совершаются каждую ночь и за одну ночь он не
 сможет совершить два перелета.

Теперь профессор хочет найти путь наименьшей стоимости, учитывая что до конференции осталось K ночей
 (то есть профессор может совершить не более K перелетов).

Input format

В первой строке находятся числа N (количество городов), M (количество авиарейсов), K (количество оставшихся
 ночей), S (номер города, в котором живет профессор), F (номер города, в котором проводится конференция).

Ограничения: 2 ≤ N ≤ 100, 1 ≤ M ≤ 10^5, 1 ≤ K ≤ 100, 1 ≤ S ≤ N, 1 ≤ F ≤ N.

Далее идет M строк, задающих расписание авиарейсов. i-я строка содержит три натуральных числа:
 S_i, F_i и P_i, где S_i - номер города, из которого вылетает i-й рейс, F_i - номер города, в который
 прилетает i-й рейс, P_i - стоимость перелета i-м рейсом. 1 ≤ S_i ≤ N, 1 ≤ F_i ≤ N, 1 ≤ P_i ≤ 10^6.

Output format

Выведите одно число - минимальную стоимость пути, подходящего для профессора. Если профессор не сможет
 за K ночей добраться до конференции, выведите число -1.

 Input	   Output
4 5 2 1 4    4
1 2 1
2 3 1
3 4 1
1 3 3
1 4 5
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
  };

  explicit Graph(size_t vertex_count, bool is_directed)
      : vertex_count_(vertex_count),
        is_directed_(is_directed),
        edge_count_(0) {}

  size_t GetVertexCount() const {
    return vertex_count_;
  }

  virtual void AddEdge(const Vertex &start, const Vertex &finish, int weight = 1) = 0;

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

  void AddEdge(const Vertex &start, const Vertex &finish, int weight = 1) override {
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

  int GetMinPathOfFixedLength(const Graph &graph, const Graph::Vertex &start, const Graph::Vertex &finish,
                              size_t path_length) {
    const int INF = std::numeric_limits<int>::max();
    std::vector<int> distances(graph.GetVertexCount() + 1, INF);
    std::vector<int> updated_distances = distances;
    distances[start] = 0;
    updated_distances[start] = 0;
    for (size_t i = 0; i < path_length; ++i) {
      auto edges = graph.GetEdges();
      for (auto &edge : edges) {
        if (distances[edge.from] != INF && distances[edge.to] > distances[edge.from] + edge.weight) {
          updated_distances[edge.to] = distances[edge.from] + edge.weight;
        }
      }
      distances = updated_distances;
    }
    if (distances[finish] == INF) {
      return -1;
    }
    return distances[finish];
  }
}

int main() {
  size_t n, m, path_length, start, finish;

  std::cin >> n >> m >> path_length >> start >> finish;
  GraphAdjList graph_adj_list = GraphAdjList(n, true);

  for (int i = 0; i < m; ++i) {
    Graph::Vertex from, to;
    int weight;
    std::cin >> from >> to >> weight;
    graph_adj_list.AddEdge(from, to, weight);
  }

  std::cout << GraphProcessing::GetMinPathOfFixedLength(graph_adj_list, start, finish, path_length);

  return 0;
}