/*От вас требуется определить вес минимального остовного дерева для неориентированного взвешенного связного графа.

Входные данные

В первой строке входного файла находятся числа N и M (1 ≤ N ≤ 100; 1 ≤ M ≤ 6000),
 где N - количество вершин в графе, а M - количество рёбер. В каждой из последующих M строк записано по тройке
 чисел A, B, C, где A и B - номера вершин, соединённых ребром, а C - вес ребра (натуральное число,
 не превышающее 30000).

Выходные данные

В выходной файл выведите одно число - искомый вес. */

#include <iostream>
#include <vector>
#include <set>
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

class WeightedGraphAdjList : public GraphAdjList {
 private:
  std::map<std::pair<Graph::Vertex, Graph::Vertex>, size_t> edge_weight_;

 public:
  explicit WeightedGraphAdjList(size_t vertex_count, bool is_directed)
      : GraphAdjList(vertex_count, is_directed) {}

  void AddEdge(const Vertex &start, const Vertex &finish, size_t weight) {
    GraphAdjList::AddEdge(start, finish);
    edge_weight_[{start, finish}] = weight;
    if (!is_directed_) {
      edge_weight_[{finish, start}] = weight;
    }
  }

  size_t GetWeight(const Vertex &start, const Vertex &finish) const {
    return edge_weight_.at({start, finish});
  }
};

namespace GraphProcessing {

  struct MinimalSpanningTreeStatus {
    const size_t INF = std::numeric_limits<size_t>::max();
    const int NOT_SET = -1;
    std::set<std::pair<Graph::Vertex, Graph::Vertex>> MST;
    std::vector<size_t> min_neighbor_weight;
    std::vector<Graph::Vertex> parent;
    std::set<std::pair<size_t, Graph::Vertex>> edge_weight_queue;

    explicit MinimalSpanningTreeStatus(size_t vertex_count)
        : min_neighbor_weight(vertex_count + 1, INF),
          parent(vertex_count + 1, NOT_SET) {}
  };

  std::set<std::pair<Graph::Vertex, Graph::Vertex>> Prim_Get_MST(const WeightedGraphAdjList &graph) {
    MinimalSpanningTreeStatus mst_status = MinimalSpanningTreeStatus(graph.GetVertexCount());
    mst_status.min_neighbor_weight[1] = 0;
    mst_status.edge_weight_queue.insert({0, 1});
    while (!mst_status.edge_weight_queue.empty()) {
      auto[dist, vertex] = *mst_status.edge_weight_queue.begin();
      mst_status.edge_weight_queue.erase(mst_status.edge_weight_queue.begin());
      if (mst_status.parent[vertex] != mst_status.NOT_SET) {
        mst_status.MST.insert(std::minmax(mst_status.parent[vertex], vertex));
      }
      for (const auto &neighbor : graph.GetAllNeighbors(vertex)) {
        if (mst_status.min_neighbor_weight[neighbor] > graph.GetWeight(neighbor, vertex)) {
          mst_status.edge_weight_queue.erase({mst_status.min_neighbor_weight[neighbor], neighbor});
          mst_status.parent[neighbor] = vertex;
          mst_status.min_neighbor_weight[neighbor] = graph.GetWeight(neighbor, vertex);
          mst_status.edge_weight_queue.insert({mst_status.min_neighbor_weight[neighbor], neighbor});
        }
      }
    }
    return mst_status.MST;
  }
}

int main() {
  size_t n, m;

  std::cin >> n >> m;
  WeightedGraphAdjList graph_adj_list = WeightedGraphAdjList(n, false);

  for (int i = 0; i < m; ++i) {
    Graph::Vertex start, finish;
    size_t weight;
    std::cin >> start >> finish >> weight;
    graph_adj_list.AddEdge(start, finish, weight);
  }
  auto MST = GraphProcessing::Prim_Get_MST(graph_adj_list);

  size_t MST_weight = 0;
  for (const auto &edge : MST) {
    MST_weight += graph_adj_list.GetWeight(edge.first, edge.second);
  }
  std::cout << MST_weight;
  return 0;
}