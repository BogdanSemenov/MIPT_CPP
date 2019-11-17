/*Байтазар работает в отделе транспортировки байтландской нефтяной компании Байтойл.
 В его задачу входит планирование поставок бензина на автозаправочные станции.

Всего в Байтландии m автомагистралей с двусторонним движением и n перекрёстков
 (обозначенных номерами от 1 до n). Каждая автомагистраль соединяет некоторые два перекрёстка.
 Никакая автомагистраль не имеет общих точек с внутренними точками другой автомагистрали.
 На некоторых перекрёстках размещены заправочные станции, принадлежащие Байтойл.

В распоряжении компании Байтойл имеются бензовозы различной вместимости. Каждый бензовоз
 вне зависимости от вместимости потребляет 1 литр бензина на километр пути. Водители не могут
 заправляться бензином из цистерны, которую везут, но могут бесплатно заправляться на любой
 заправочной станции Байтойл.

Байтазару требуется отвечать на следующие запросы: может ли бензовоз с бензобаком вместомости
 b литров доехать от заправочной станции, расположенной на перекрёстке x, до заправочной станции,
 расположенной на перекрёстке y? Напомним, что бензовоз с бензобаком ёмкостью b не может проехать
 более b километров без заправки.

Напишите программу, которая бы автоматизировала ответы на подобные запросы.

Input format
Первая строка входного файла содержит три целых числа n, s и m (2 ≤ s ≤ n ≤ 200000, 1 ≤ m ≤ 200000)
 - количество перекрёстков, количество заправочных станций Байтойл и количество дорог в Байтландии.
 Вторая строка содержит s попарно различных целых чисел c1, c2, ..., cs (1 ≤ ci ≤ n) - номера
 перекрёстков, на которых расположены заправки Байтойл.

Следующие m строк задают дороги в Байтландии. i-я из этих строк содержит три целых числа ui,
 vi и di (1 ≤ ui,vi ≤ n, ui != vi, 1 ≤ di ≤ 10000), обозначающие, что i-я дорога имеет длину
 di километров и соединяет перекрёстки ui и vi. Каждая пара перекрёстков соединена не более,
 чем одной дорогой.

Следующая строка содержит одно целое число q (1 ≤ q ≤ 200000) - количество запросов.
 Последующие q строк задают запросы: i-я из этих строк содержит три целых числа xi, yi и bi
 (1 ≤ xi,yi ≤ n, xi != yi, 1 ≤ bi ≤ 2*10^9) --- исходную точку маршрута, конечную точку маршрута
 и ёмкость бака бензовоза. Гарантируется, что на обоих перекрёстка xi и yi размещены заправочные
 станции Байтойл.

Output format
Выведите q строк, i-я из которых должна содержать ответ на i-й запрос: TAK, в случае,
 если бензовоз с баком вместимостью bi сможет доехать от перекрёстка xi до перекрёстка yi,
 и NIE в противном случае.

Input        Output
6 4 5         TAK
1 5 2 6       TAK
1 3 1         TAK
2 3 2         NIE
3 4 3
4 5 5
6 4 5
4
1 2 4
2 6 9
1 5 9
6 5 8
*/

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <limits>
#include <string>
#include <unordered_map>

class Graph {
 protected:
  size_t vertex_count_;
  bool is_directed_;
  size_t edge_count_;

 public:
  typedef size_t Vertex;

  struct Edge {
    Vertex from;
    Vertex to;
    size_t weight;

    bool operator<(const Edge &edge) const {
      return weight < edge.weight || (weight == edge.weight && from < edge.from) ||
          (weight == edge.weight && from == edge.from && to < edge.to);
    }

    bool operator==(const Edge &edge) const {
      return weight == edge.weight && edge.to == to && edge.from == from;
    }
  };

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
        adj_list_(vertex_count + 1) {}

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

template<typename T>
class DSU {
 private:
  std::unordered_map<T, size_t> rank_;
  std::unordered_map<T, T> predecessors_;

  T FindSet(const T &x) {
    if (x == predecessors_[x]) {
      return x;
    }
    return predecessors_[x] = FindSet(predecessors_[x]);
  }

 public:
  explicit DSU(const std::vector<T> &elements) {
    for (int i = 0; i < elements.size(); ++i) {
      predecessors_[elements[i]] = elements[i];
      rank_[elements[i]] = 0;
    }
  }

  void Union(const T &x, const T &y) {
    T x_predecessor = FindSet(x);
    T y_predecessor = FindSet(y);

    if (x_predecessor == y_predecessor) {
      return;
    }
    if (rank_[x_predecessor] < rank_[y_predecessor]) {
      std::swap(x_predecessor, y_predecessor);
    }
    predecessors_[y_predecessor] = x_predecessor;
    if (rank_[x_predecessor] == rank_[y_predecessor]) {
      ++rank_[x_predecessor];
    }
  }

  bool InSameSet(const T &x, const T &y) {
    return FindSet(x) == FindSet(y);
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

  typedef std::priority_queue<Distance_Vertex, std::vector<Distance_Vertex>, std::greater<Distance_Vertex>> PriorityQueue;

  void Relax(const Graph &graph, std::vector<size_t> &min_distance, const Graph::Vertex &neighbor,
             const Graph::Vertex &vertex, PriorityQueue &priority_queue) {
    size_t weight = graph.GetWeight(vertex, neighbor);
    if (min_distance[neighbor] > weight + min_distance[vertex]) {
      min_distance[neighbor] = weight + min_distance[vertex];
      priority_queue.push({min_distance[neighbor], neighbor});
    }
  }

  std::vector<size_t> GetMinDistances_Dijkstra(const Graph &graph, const std::vector<Graph::Vertex> &starts) {
    const size_t vertex_count = graph.GetVertexCount();
    std::vector<size_t> min_distance(vertex_count, INF);
    std::vector<bool> visited(vertex_count, false);
    PriorityQueue priority_queue;
    for (auto start : starts) {
      min_distance[start] = 0;
      priority_queue.push({0, start});
    }
    while (!priority_queue.empty()) {
      Graph::Vertex vertex = priority_queue.top().vertex;
      priority_queue.pop();
      if (visited[vertex]) {
        continue;
      }
      visited[vertex] = true;
      for (const auto &neighbor : graph.GetAllNeighbors(vertex)) {
        Relax(graph, min_distance, neighbor, vertex, priority_queue);
      }
    }
    return min_distance;
  }
}

struct PathInfo {
  Graph::Edge edge;
  int id;
  bool is_graph_edge;

  bool operator<(const PathInfo &path_info) const {
    return edge.weight < path_info.edge.weight || (edge.weight == path_info.edge.weight && id < path_info.id);
  }

  bool operator==(const PathInfo &path_info) const {
    return edge.weight == path_info.edge.weight && id == path_info.id;
  }

  bool operator>(const PathInfo &path_info) const {
    return !(*this < path_info || *this == path_info);
  }
};

std::vector<Graph::Vertex> GenerateVerticesVector(size_t vector_size) {
  std::vector<Graph::Vertex> vertices(vector_size);
  Graph::Vertex vertex(1);
  std::generate(vertices.begin(), vertices.end(), [&vertex] {
    return vertex++;
  });
  return vertices;
}

std::vector<bool> PossibilityToMoveFromStartToFinish(const Graph &graph, const std::vector<Graph::Vertex> &gas_stations,
                                                     const size_t quires_num, const std::vector<Graph::Edge> &edges,
                                                     std::priority_queue<PathInfo, std::vector<PathInfo>,
                                                                         std::greater<PathInfo>> &path_info) {
  int NOT_SET = -1;
  auto min_distances = GraphProcessing::GetMinDistances_Dijkstra(graph, gas_stations);
  for (const auto &edge : edges) {
    path_info.push({{edge.from, edge.to, edge.weight + min_distances[edge.from] + min_distances[edge.to]}, NOT_SET,
                    true});
  }

  DSU<Graph::Vertex> dsu(GenerateVerticesVector(graph.GetVertexCount()));
  std::vector<bool> possibility_to_move_from_start_to_finish(quires_num);
  while (!path_info.empty()) {
    auto edge_info = path_info.top();
    path_info.pop();
    if (!edge_info.is_graph_edge) {
      possibility_to_move_from_start_to_finish[edge_info.id] = dsu.InSameSet(edge_info.edge.to, edge_info.edge.from);
    } else {
      dsu.Union(edge_info.edge.to, edge_info.edge.from);
    }
  }
  return possibility_to_move_from_start_to_finish;
}

int main() {
  size_t vertex_num, gas_station_num, edges_num;
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cin >> vertex_num >> gas_station_num >> edges_num;

  std::vector<Graph::Vertex> gas_stations(gas_station_num);
  std::priority_queue<PathInfo, std::vector<PathInfo>, std::greater<PathInfo>> path_info;
  std::vector<Graph::Edge> edges;
  for (size_t i = 0; i < gas_station_num; ++i) {
    std::cin >> gas_stations[i];
    --gas_stations[i];
  }

  GraphAdjList graph_adj_list(vertex_num, false);
  for (size_t i = 0; i < edges_num; ++i) {
    Graph::Vertex start, finish;
    size_t weight;
    std::cin >> start >> finish >> weight;
    --start;
    --finish;
    graph_adj_list.AddEdge(start, finish, weight);
    edges.push_back({start, finish, weight});
  }

  size_t queries_num;
  std::cin >> queries_num;
  for (size_t i = 0; i < queries_num; ++i) {
    Graph::Vertex start, finish;
    size_t weight;
    std::cin >> start >> finish >> weight;
    path_info.push({{start - 1, finish - 1, weight}, static_cast<int>(i), false});
  }

  std::vector<bool> result =
      PossibilityToMoveFromStartToFinish(graph_adj_list, gas_stations, queries_num, edges, path_info);
  for (const auto &res : result) {
    std::cout << (res ? "TAK" : "NIE") << std::endl;
  }

  return 0;
}
