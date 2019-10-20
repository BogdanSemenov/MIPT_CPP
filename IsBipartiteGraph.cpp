/*Во время контрольной работы профессор Флойд заметил, что некоторые студенты обмениваются записками.
Сначала он хотел поставить им всем двойки, но в тот день профессор был добрым, а потому решил разделить
студентов на две группы: списывающих и дающих списывать, и поставить двойки только первым.

У профессора записаны все пары студентов, обменявшихся записками. Требуется определить, сможет ли он разделить
 студентов на две группы так, чтобы любой обмен записками осуществлялся от студента одной группы студенту другой
 группы.

Input format

В первой строке находятся два числа N и M — количество студентов и количество пар студентов,
 обменивающихся записками (1 ≤ N ≤ 100, 0 ≤ M ≤ N*(N−1)/2). Далее в M строках расположены описания пар
 студентов: два числа, соответствующие номерам студентов, обменивающихся записками (нумерация студентов идёт с 1).
 Каждая пара студентов перечислена не более одного раза.

Output format

Необходимо вывести ответ на задачу профессора Флойда. Если возможно разделить студентов на две группы
 — выведите «YES»; иначе выведите «NO».
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

  enum Group {
    FIRST,
    SECOND,
    NOT_SET
  };

  bool DFS(const Graph &graph, std::vector<Group> &group_status, const Graph::Vertex &vertex) {
    Group neighbor_group = (group_status[vertex] == FIRST ? SECOND : FIRST);
    for (auto neighbor : graph.GetAllNeighbors(vertex)) {
      if (group_status[neighbor] == group_status[vertex]) {
        return false;
      } else if (group_status[neighbor] == NOT_SET) {
        group_status[neighbor] = neighbor_group;
        if (!DFS(graph, group_status, neighbor)) {
          return false;
        }
      }
    }
    return true;
  }

  bool IsBipartiteGraph(const Graph &graph) {
    std::vector<Group> group_status(graph.GetVertexCount() + 1, NOT_SET);
    for (Graph::Vertex vertex = 1; vertex < graph.GetVertexCount() + 1; ++vertex) {
      if (group_status[vertex] == NOT_SET) {
        group_status[vertex] = FIRST;
        if (!DFS(graph, group_status, vertex)) {
          return false;
        }
      }
    }
    return true;
  }

}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  GraphAdjList graph_adj_list(n, false);

  for (int i = 0; i < m; ++i) {
    Graph::Vertex start, finish;
    std::cin >> start >> finish;
    graph_adj_list.AddEdge(start, finish);
  }

  if (GraphProcessing::IsBipartiteGraph(graph_adj_list)) {
    std::cout << "YES";
  } else {
    std::cout << "NO";
  }

  return 0;
}
