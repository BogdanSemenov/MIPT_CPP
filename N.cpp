#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <memory>
#include <limits>
#include <exception>

////////////////////////////// class FlowNetwork //////////////////////////////

class FlowNetwork {
 private:
  typedef size_t Vertex;
  typedef int64_t Flow;

  struct FlowEdge {
    Vertex from;
    Vertex to;
    Flow flow;
    Flow capacity;

    explicit FlowEdge() = default;
    explicit FlowEdge(const Vertex &from, const Vertex &to, const Flow &capacity);
    Flow ResidualCapacity() const;
    Vertex Other(const Vertex &vertex) const;
  };

  struct FlowEdgePair {
    FlowEdge *forward_edge;
    FlowEdge *back_edge;
  };

  std::vector<FlowEdge> index_edge_;
  std::vector<std::vector<size_t>> adj_list_;
  std::vector<bool> visited_;
  size_t vertex_count_;
  size_t edge_count_;
  Vertex source_, termination_;
  std::vector<FlowEdgePair> predecessors_;
  bool HasIncreasingPath();
    
 public:

  FlowNetwork::Flow GetMaxFlow_EdmondsKarp();
  FlowNetwork(size_t vertex_count, size_t edges_count, const Vertex &source, const Vertex &termination);
  void AddEdge(const Vertex &from, const Vertex &to, const Flow &capacity, size_t index);
  Vertex GetSource() const;
  Vertex GetTermination() const;
  size_t GetVertexCount() const;
  FlowEdge &GetBackEdge(size_t index);
  std::vector<size_t> GetEdgesIndicesIncidentOn(const Vertex &vertex) const;
};


////////////////////////////// class FlowNetwork //////////////////////////////

FlowNetwork::FlowNetwork(size_t vertex_count, size_t edges_count, const Vertex &source, const Vertex &termination)
    : vertex_count_(vertex_count),
      edge_count_(edges_count),
      adj_list_(vertex_count),
      source_(source),
      index_edge_(2 * edges_count),
      visited_(vertex_count, false),
      predecessors_(vertex_count),
      termination_(termination) {}

void FlowNetwork::AddEdge(const Vertex &from, const Vertex &to, const Flow &capacity, size_t index) {
  index_edge_[2 * index] = FlowEdge(from, to, capacity);
  index_edge_[2 * index + 1] = FlowEdge(to, from, 0);
  adj_list_[from].push_back(2 * index);
  adj_list_[to].push_back(2 * index + 1);
}

std::vector<size_t> FlowNetwork::GetEdgesIndicesIncidentOn(const FlowNetwork::Vertex &vertex) const {
  return adj_list_[vertex];
}

FlowNetwork::Vertex FlowNetwork::GetSource() const {
  return source_;
}

FlowNetwork::Vertex FlowNetwork::GetTermination() const {
  return termination_;
}

size_t FlowNetwork::GetVertexCount() const {
  return vertex_count_;
}

FlowNetwork::FlowEdge &FlowNetwork::GetBackEdge(size_t index) {
  return index_edge_[index ^ 1];
}

////////////////////////////// struct FlowEdge //////////////////////////////

FlowNetwork::FlowEdge::FlowEdge(const Vertex &from, const Vertex &to, const Flow &capacity)
    : from(from),
      to(to),
      capacity(capacity),
      flow(0) {}


FlowNetwork::Flow FlowNetwork::FlowEdge::ResidualCapacity() const {
  return capacity - flow;
}

FlowNetwork::Vertex FlowNetwork::FlowEdge::Other(const FlowNetwork::Vertex &vertex) const {
  return (vertex == from) ? to : from;
}

////////////////////////////// EdmondsKarpAlgorithm //////////////////////////////

FlowNetwork::Flow FlowNetwork::GetMaxFlow_EdmondsKarp() {
  const Flow INF = std::numeric_limits<Flow>::max();
  const auto NOT_SET = new FlowEdge(0, 0, 0);
  predecessors_.assign(vertex_count_, {NOT_SET, NOT_SET});
  Flow max_flow = 0;
  while (HasIncreasingPath()) {
    Flow blocking_flow = INF;
    for (Vertex vertex = termination_; vertex != source_; vertex = predecessors_[vertex].forward_edge->Other(vertex)) {
      blocking_flow = std::min(blocking_flow, predecessors_[vertex].forward_edge->ResidualCapacity());
    }
    for (Vertex vertex = termination_; vertex != source_; vertex = predecessors_[vertex].forward_edge->Other(vertex)) {
      predecessors_[vertex].forward_edge->flow += blocking_flow;
      predecessors_[vertex].back_edge->flow -= blocking_flow;
    }
    predecessors_.assign(vertex_count_, {NOT_SET, NOT_SET});
    max_flow += blocking_flow;
  }
  return max_flow;
}

bool FlowNetwork::HasIncreasingPath() {
  visited_.assign(vertex_count_, false);
  std::queue<Vertex> queue;
  queue.push(source_);
  visited_[source_] = true;
  while (!queue.empty() && !visited_[termination_]) {
    Vertex vertex = queue.front();
    queue.pop();
    for (size_t index : adj_list_[vertex]) {
      FlowEdge &flow_edge = index_edge_[index];
      Vertex adj_vertex = flow_edge.Other(vertex);
      if (flow_edge.ResidualCapacity() > 0) {
        if (!visited_[adj_vertex]) {
          predecessors_[adj_vertex] = {&flow_edge, &GetBackEdge(index)};
          visited_[adj_vertex] = true;
          queue.push(adj_vertex);
        }
      }
    }
  }
  return visited_[termination_];
}

////////////////////////////////////////////////////////////////////////////////

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t vertex_count, edge_count;
  std::cin >> vertex_count >> edge_count;
  FlowNetwork flow_network(vertex_count, edge_count, 0, vertex_count - 1);
  for (size_t i = 0; i < edge_count; ++i) {
    size_t from, to;
    int64_t capacity;
    std::cin >> from >> to >> capacity;
    flow_network.AddEdge(from - 1, to - 1, capacity, i);
  }

  std::cout << flow_network.GetMaxFlow_EdmondsKarp();

  return 0;
}
