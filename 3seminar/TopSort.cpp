/* Группа солдат-новобранцев прибыла в армейскую часть N666.
 После знакомства с прапорщиком стало очевидно, что от работ на кухне по очистке картофеля
 спасти солдат может только чудо.

Прапорщик, будучи не в состоянии запомнить фамилии, пронумеровал новобранцев от 1 до N.
 После этого он велел им построиться по росту (начиная с самого высокого).
 С этой несложной задачей могут справиться даже совсем необученные новобранцы, да вот беда,
 прапорщик уверил себя, что знает про некоторых солдат, кто из них кого выше,
 и это далеко не всегда соответствует истине.

После трех дней обучения новобранцам удалось выяснить, что знает (а точнее, думает, что знает)
 прапорщик. Помогите им, используя эти знания, построиться так, чтобы товарищ прапорщик остался
 доволен.

Входные данные

Сначала на вход программы поступают числа N и M (1 < N <= 100, 1 <= M <= 5000)
 – количество солдат в роте и количество пар солдат, про которых прапорщик знает,
 кто из них выше. Далее идут эти пары чисел A и B по одной на строке (1 <= A,B <= N),
 что означает, что, по мнению прапорщика, солдат A выше, чем B.
 Не гарантируется, что все пары чисел во входных данных различны.

Выходные данные

В первой строке выведите "Yes" (если можно построиться так, чтобы прапорщик остался доволен)
 или "No" (если нет). После ответа "Yes" на следующей строке выведите N чисел,
 разделенных пробелами, - одно из возможных построений.
 */

#include <iostream>
#include <vector>

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

namespace GraphProcessing {

  enum Colors {
    WHITE,
    GREY,
    BLACK
  };

  void DFS_TopSort(const Graph &graph, const Graph::Vertex &vertex, std::vector<bool> &used,
                   std::vector<Graph::Vertex> &topsort) {
    used[vertex] = true;
    for (Graph::Vertex u : graph.GetAllNeighbors(vertex)) {
      if (!used[u]) {
        DFS_TopSort(graph, u, used, topsort);
      }
    }
    topsort.push_back(vertex);
  }

  bool DFS_IsCyclic(const Graph &graph, const Graph::Vertex &vertex, std::vector<Colors> &visited) {
    visited[vertex] = GREY;
    for (Graph::Vertex u : graph.GetAllNeighbors(vertex)) {
      if (visited[u] == WHITE) {
        if (!DFS_IsCyclic(graph, u, visited)) {
          return false;
        }
      } else if (visited[u] == GREY) {
        return false;
      }
    }
    visited[vertex] = BLACK;
    return true;
  }

  bool IsCyclic(const Graph &graph) {
    std::vector<Colors> visited(graph.GetVertexCount() + 1, WHITE);
    for (Graph::Vertex vertex = 1; vertex < graph.GetVertexCount() + 1; ++vertex) {
      if (visited[vertex] == WHITE) {
        if (!DFS_IsCyclic(graph, vertex, visited)) {
          return true;
        }
      }
    }
    return false;
  }

  std::vector<Graph::Vertex> TopSort(const Graph &graph) {
    if (!graph.IsDirected() || IsCyclic(graph)) {
      return {};
    }
    std::vector<bool> used(graph.GetVertexCount() + 1, false);
    std::vector<Graph::Vertex> topsort;
    for (Graph::Vertex vertex = 1; vertex < graph.GetVertexCount() + 1; ++vertex) {
      if (!used[vertex]) {
        DFS_TopSort(graph, vertex, used, topsort);
      }
    }

    return {topsort.rbegin(), topsort.rend()};
  }
}

int main() {
  size_t n, m;
  std::cin >> n >> m;

  GraphAdjList graph_adj_list = GraphAdjList(n, true);
  for (int i = 0; i < m; ++i) {
    size_t first, second;
    std::cin >> first >> second;
    graph_adj_list.AddEdge(first, second);
  }

  auto topsort = GraphProcessing::TopSort(graph_adj_list);

  if (topsort.empty()) {
    std::cout << "No";
  } else {
    std::cout << "Yes" << std::endl;
    for (auto vertex : topsort) {
      std::cout << vertex << ' ';
    }
  }

  return 0;
}
