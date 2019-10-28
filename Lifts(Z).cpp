/*Чтобы поднять в свой офис на N-м этаже небоскреба новый сейф, Вите опять пришлось прибегнуть
 к помощи грузчиков. Но за это время система оплаты изменилась. Теперь за подъем по лестнице
 на один этаж требуется заплатить U рублей, за спуск по лестнице на один этаж — D рублей, за внос
 в лифт — I рублей, за вынос из лифта — J рублей.

В офисе имеется L лифтов, каждый из которых останавливается лишь на определенных этажах.

Помогите Вите разработать маршрут подъема сейфа с первого этажа, стоимость которого наименьшая.

Input format

В первой строке входного файла записаны целые числа N, U, D, I, J, L. Каждая из следующих
 L строк описывает соответствующий лифт. Она начинается с числа K_i — количества этажей, на которых
 останавливается i-й лифт, за которым следует K_i натуральных чисел — этажи, на которых
 останавливается этот лифт (этажи для каждого лифта задаются в возрастающем порядке). 0 ≤ U ≤ 1000,
 0 ≤ D ≤ 1000, 0 ≤ I ≤ 1000, 0 ≤ J ≤ 1000, 0 ≤ L ≤ 500, 1 ≤ N ≤ 1000000, 2 ≤ K_i ≤ 1000,
 K_1+K_2+…+K_L ≤ 100000. Количество этажей в небоскребе не превосходит 1000000.

Output format

В выходной файл выведите одно число — минимальную стоимость подъема сейфа.


Input	         Output
10 1 1 1 1 1       7
2 3 7

10 1 1 3 2 1       9
2 3 7

20 100 0 1 1 2    804
2 5 7
2 8 17

*/


#include <iostream>
#include <vector>
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

  virtual void AddEdge(const Vertex &start, const Vertex &finish, int weight = 1) = 0;

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

  typedef std::priority_queue<Distance_Vertex, std::vector<Distance_Vertex>,
                              std::greater<>> PriorityQueue;

  void Relax(const int weight, std::vector<size_t> &min_distance, const Graph::Vertex &neighbor,
             const Graph::Vertex &vertex, PriorityQueue &priority_queue) {
    if (min_distance[neighbor] > weight + min_distance[vertex]) {
      min_distance[neighbor] = weight + min_distance[vertex];
      priority_queue.push({min_distance[neighbor], neighbor});
    }
  }
}

std::vector<size_t> GetMinCosts(const Graph &graph, const size_t stairs_up_cost,
                                const size_t stairs_down_cost, const Graph::Vertex &start,
                                const size_t limitation) {
  const size_t vertex_count = graph.GetVertexCount();
  std::vector<size_t> min_cost(vertex_count, GraphProcessing::INF);
  std::vector<bool> visited(vertex_count, false);
  GraphProcessing::PriorityQueue priority_queue;
  min_cost[start] = 0;
  priority_queue.push({0, start});
  while (!priority_queue.empty()) {
    Graph::Vertex vertex = priority_queue.top().vertex;
    priority_queue.pop();
    if (visited[vertex]) {
      continue;
    }
    visited[vertex] = true;
    for (const auto &neighbor : graph.GetAllNeighbors(vertex)) {
      int weight = graph.GetWeight(vertex, neighbor);
      GraphProcessing::Relax(weight, min_cost, neighbor, vertex, priority_queue);
    }
    if (vertex > 0 && vertex < limitation) {
      GraphProcessing::Relax(stairs_down_cost, min_cost, vertex - 1, vertex, priority_queue);
    }
    if (vertex < limitation - 1)
      GraphProcessing::Relax(stairs_up_cost, min_cost, vertex + 1, vertex, priority_queue);
  }
  return min_cost;
}

int main() {
  const size_t MAX_FLOOR_NUM = 1000000;
  const size_t start_floor = 0;
  size_t finish_floor, stairs_up_cost, stairs_down_cost, lift_up_cost, lift_down_cost, lifts_num;
  std::cin >> finish_floor >> stairs_up_cost >> stairs_down_cost >> lift_up_cost >> lift_down_cost >> lifts_num;
  GraphAdjList graph_adj_list(MAX_FLOOR_NUM + lifts_num, true);

  for (size_t i = 0; i < lifts_num; ++i) {
    Graph::Vertex lift_stop_floors_cnt;
    std::cin >> lift_stop_floors_cnt;
    for (size_t j = 0; j < lift_stop_floors_cnt; ++j) {
      Graph::Vertex floor;
      std::cin >> floor;
      graph_adj_list.AddEdge(floor - 1, MAX_FLOOR_NUM + i, lift_up_cost);
      graph_adj_list.AddEdge(MAX_FLOOR_NUM + i, floor - 1, lift_down_cost);
   }
  }

  auto distance = GetMinCosts(graph_adj_list, stairs_up_cost, stairs_down_cost, start_floor, MAX_FLOOR_NUM);
  std::cout << distance[finish_floor - 1];

  return 0;
}