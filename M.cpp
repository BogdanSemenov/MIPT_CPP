#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

////////////////////////////// class FlowNetwork //////////////////////////////

class FlowNetwork {
 public:
  typedef size_t Vertex;
  typedef int64_t Flow;

  struct FlowEdge {
    Vertex from;
    Vertex to;
    size_t flow;
    size_t capacity;

    explicit FlowEdge() = default;
    explicit FlowEdge(const Vertex &from, const Vertex &to, const Flow &capacity);
    Flow ResidualCapacity() const;
  };

  FlowNetwork(size_t vertex_count, size_t edges_count, const Vertex &source, const Vertex &termination);
  Flow GetMaxFlow_FordFulkerson();
  Flow PushFlow(const Vertex &vertex, const Flow &flow_upper_bound);
  void AddEdge(const Vertex &from, const Vertex &to, const Flow &capacity, size_t index);
  Vertex GetSource() const;
  Vertex GetTermination() const;
  size_t GetVertexCount() const;
  FlowEdge &GetBackEdge(size_t index);
  std::vector<size_t> GetEdgesIndicesIncidentOn(const Vertex &vertex) const;

 private:
  std::vector<FlowEdge> index_edge_;
  std::vector<std::vector<size_t>> adj_list_;
  std::vector<bool> visited_;
  size_t vertex_count_;
  size_t edge_count_;
  Vertex source_, termination_;
  Flow DFS(const Vertex &vertex, const Flow &flow_upper_bound);
};

////////////////////////////// class FlowNetwork //////////////////////////////

FlowNetwork::FlowNetwork(size_t vertex_count, size_t edges_count, const Vertex &source, const Vertex &termination)
    : vertex_count_(vertex_count),
      edge_count_(edges_count),
      source_(source),
      index_edge_(2 * edges_count),
      adj_list_(vertex_count),
      termination_(termination),
      visited_(vertex_count, false) {}

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

////////////////////////////// FordFulkerson Algorithm //////////////////////////////

FlowNetwork::Flow FlowNetwork::GetMaxFlow_FordFulkerson() {
  const Flow INF = std::numeric_limits<Flow>::max();
  Flow max_flow = 0, blocking_flow = INF;
  while (blocking_flow != 0) {
    blocking_flow = PushFlow(source_, INF);
    max_flow += blocking_flow;
  }
  return max_flow;
}

FlowNetwork::Flow FlowNetwork::PushFlow(const Vertex &vertex, const Flow &flow_upper_bound) {
  visited_.assign(vertex_count_, false);
  return DFS(vertex, flow_upper_bound);
}

FlowNetwork::Flow FlowNetwork::DFS(const Vertex &vertex, const Flow &flow_upper_bound) {
  if (vertex == termination_) {
    return flow_upper_bound;
  }
  visited_[vertex] = true;
  for (size_t index : GetEdgesIndicesIncidentOn(vertex)) {
    FlowEdge &flow_edge = index_edge_[index];
    Vertex adj_vertex = flow_edge.to;
    if (!visited_[adj_vertex] && flow_edge.ResidualCapacity() > 0) {
      Flow delta = DFS(adj_vertex, std::min(flow_upper_bound, flow_edge.ResidualCapacity()));
      if (delta > 0) {
        flow_edge.flow += delta;
        FlowEdge &back_flow_edge = GetBackEdge(index);
        back_flow_edge.flow -= delta;
        return delta;
      }
    }
  }
  return 0;
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

////////////////////////////////////////////////////////////////////////////////

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t vertex_count, edge_count;
  std::cin >> vertex_count >> edge_count;
  FlowNetwork flow_network(vertex_count, edge_count, 0, vertex_count - 1);
  for (size_t i = 0; i < edge_count; ++i) {
    FlowNetwork::Vertex from, to;
    FlowNetwork::Flow capacity;
    std::cin >> from >> to >> capacity;
    flow_network.AddEdge(from - 1, to - 1, capacity, i);
  }

  std::cout << flow_network.GetMaxFlow_FordFulkerson();

  return 0;
}