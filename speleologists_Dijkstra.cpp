/* Однажды группа спелеологов отправилась покорять африканские пещеры. Углубившись достаточно далеко,
 они обнаружили что-то странное - пахло дымом. "Неужели мы подобрались к вулкану?" - удивленно
 спросил новичок. "Нет. Около вулкана пахнет серой", - ответил капитан. "Кто у нас занимался тушением
 костров?". Все неуверенно пожали плечами. "Итак, друзья, мы в ловушке".

Перед капитаном команды спелеологов возникла непосильная задача: как же вывести свою группу из пещеры,
 не сгорев? Пещера в терминах спелеологов состоит из множества "тоннелей" и "станций". Каждый тоннель
 соединяет какую-нибудь пару различных станций и имеет свою длину. У капитана имеется карта пещеры, и
 он знает, где они разводили костры, и где находится выход из пещеры.

Каждую секунду дым распространяется на 1 метр. Таким образом, тоннель длины "k", смежный
 с уже задымленной станцией, заполнится дымом за "k" секунд, а также заполнится дымом и соответствующая
 станция. Если спелеологи находятся на станции, где есть дым, они задыхаются. Спелеологи бегут со
 скоростью 1 метр в секунду. Изначально дым есть только в тех станциях, где спелеологи разводили костры.

Перемещаться по тоннелям можно в обе стороны. Если спелеологи прибывают на станцию одновременно с дымом,
 то они задыхаются (это верно и для той станции, где расположен выход). Гарантируется, что сейчас
 спелеологи находятся на станции, где нет дыма.

Input format

В первой строке входных данных содержится три целых числа N, M и K - количество станций, тоннелей
 и станций с кострами соответственно (2 ≤ N ≤ 2*105, 0 ≤ M ≤ 2*105, 1 ≤ K < N).

Во второй строке содержится K различных чисел ai - номера станций, в которых спелеологи разводили костры
 (1 ≤ ai ≤ N).

Следующие M строк описывают тоннели. Каждое описание состоит из трех чисел xi, yi и li,
 обозначающих номера станций, которые соединяет i-ый тоннель, и его длину в метрах
 (1 ≤ xi, yi ≤ N, 1 ≤ li ≤ 109$, xi != yi). В последней строке входных данных содержится 2 числа
 S и F - номер станции, на которой сейчас находятся спелеологи и номер станции, в которой находится
 выход из пещеры (1 ≤ S, F ≤ N).

Output format

Выведите единственное число - минимальное количество секунд, которое требуется спелеологам для того,
 чтобы выбраться из пещеры, либо "-1", если спелеологам не удастся выбраться.

Input    Output
6 5 1    23
6
1 2 20
2 3 2
3 4 1
2 5 1
5 6 3
4 1

6 6 2   -1
4 1
4 1 1
6 5 1
2 4 1
1 5 2
5 6 2
5 1 2
3 6

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

  virtual bool HasEdge(const Vertex& start, const Vertex& finish) const = 0;

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

  bool HasEdge(const Vertex& start, const Vertex& finish) const override {
    auto begin = adj_list_[start].begin();
    auto end = adj_list_[start].end();
    auto it = std::find(begin, end, finish);
    return it != end;
  }

  size_t GetWeight(const Vertex &start, const Vertex &finish) const override {
    return edge_weight_.at({start, finish});
  }
};

namespace GraphProcessing {

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

  typedef std::priority_queue<Distance_Vertex, std::vector<Distance_Vertex>, std::greater<>> PriorityQueue;

  void Relax(const Graph &graph, std::vector<size_t> &min_distance, const Graph::Vertex &neighbor,
             const Graph::Vertex &vertex, PriorityQueue &priority_queue) {
    size_t weight = graph.GetWeight(vertex, neighbor);
    if (min_distance[neighbor] > weight + min_distance[vertex]) {
      min_distance[neighbor] = weight + min_distance[vertex];
      priority_queue.push({min_distance[neighbor], neighbor});
    }
  }

  std::vector<size_t> GetMinDistances_Dijkstra(const Graph &graph, const std::vector<Graph::Vertex> &starts) {
    const size_t INF = std::numeric_limits<size_t>::max();
    const size_t vertex_count = graph.GetVertexCount();
    std::vector<size_t> min_distance(vertex_count + 1, INF);
    std::vector<bool> visited(vertex_count + 1, false);
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

int main() {
  size_t vertex_num, edges_num, fires_num;
  std::cin >> vertex_num >> edges_num >> fires_num;
  std::vector<Graph::Vertex> fires(fires_num);
  for (size_t i = 0; i < fires_num; ++i) {
    std::cin >> fires[i];
  }

  GraphAdjList graph_adj_list(vertex_num, false);
  for (size_t i = 0; i < edges_num; ++i) {
    Graph::Vertex start, finish;
    size_t weight;
    std::cin >> start >> finish >> weight;
    bool has_edge = graph_adj_list.HasEdge(start, finish);
    if (!has_edge || (weight < graph_adj_list.GetWeight(start, finish))) {
      graph_adj_list.AddEdge(start, finish, weight);
    }
  }
  auto fire_time = GraphProcessing::GetMinDistances_Dijkstra(graph_adj_list, fires);
  Graph::Vertex start, finish;
  std::cin >> start >> finish;
  auto speleologists_time = GraphProcessing::GetMinDistances_Dijkstra(graph_adj_list, {start});
  if (speleologists_time[finish] >= fire_time[finish]) {
    std::cout << -1;
  } else {
    std::cout << speleologists_time[finish];
  }
  return 0;
}
