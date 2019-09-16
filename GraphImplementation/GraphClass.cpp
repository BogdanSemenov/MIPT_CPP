#include "GraphClass.h"
#include <algorithm>
#include <iostream>

Graph::Graph(size_t vertex_count, size_t edge_count, bool is_directed)
    : vertex_count_(vertex_count), edge_count_(edge_count), is_directed_(is_directed) {}

size_t Graph::GetVertexCount() const {
  return vertex_count_;
}

size_t Graph::GetEdgeCount() const {
  return edge_count_;
}

bool Graph::GetDirection() const {
  return is_directed_;
}

GraphAdjMatrix::GraphAdjMatrix(const std::vector<std::vector<Vertex>>& graph)
    : Graph(graph.size()), adj_matrix_(graph) {
  for (int i = 1; i < vertex_count_ + 1; ++i) {
    for (int j = 1; j < vertex_count_ + 1; ++j) {
      if (graph[i][j] > 0) {
        edge_count_ += graph[i][j];
      }
    }
  }
}

bool GraphAdjMatrix::HasEdge(const Vertex& start, const Vertex& finish) const {
  return adj_matrix_[start][finish] > 0;
}

void GraphAdjMatrix::AddEdge(const Vertex& start, const Vertex& finish) {
  ++adj_matrix_[start][finish];
  if (!is_directed_) {
    ++adj_matrix_[finish][start];
  }
  ++edge_count_;
}

void GraphAdjMatrix::DeleteEdge(const Vertex& start, const Vertex& finish) {
  if (HasEdge(start, finish)) {
    --adj_matrix_[start][finish];
    if (!is_directed_) {
      --adj_matrix_[finish][start];
    }
    --edge_count_;
  }
}

const std::vector<Graph::Vertex> GraphAdjMatrix::GetNeighbours(const Vertex& vertex) const {
  std::vector<Vertex> result;
  for (int i = 1; i < vertex_count_ + 1; ++i) {
    if (HasEdge(vertex, i)) {
      result.push_back(i);
    }
  }
  return result;
}

void GraphAdjMatrix::Transpose() {
  for (int i = 1; i < vertex_count_ + 1; ++i) {
    for (int j = i; j < vertex_count_ + 1; ++j) {
      std::swap(adj_matrix_[i][j], adj_matrix_[j][i]);
    }
  }
}


GraphAdjList::GraphAdjList(const std::vector<std::vector<Vertex>>& graph)
    : Graph(graph.size()), adj_list_(graph) {
  for (int i = 1; i < vertex_count_ + 1; ++i) {
    edge_count_ += graph[i].size();
  }
}

bool GraphAdjList::HasEdge(const Vertex& start, const Vertex& finish) const {
  auto begin = adj_list_[start].begin();
  auto end = adj_list_[start].end();
  auto it = std::find(begin, end, finish);
  return it != end;
}

void GraphAdjList::AddEdge(const Vertex& start, const Vertex& finish) {
  adj_list_[start].push_back(finish);
  if (!is_directed_) {
    adj_list_[finish].push_back(start);
  }
  ++edge_count_;
}

void GraphAdjList::DeleteEdge(const Vertex& start, const Vertex& finish) {
  if (HasEdge(start, finish)) {
    adj_list_[start].erase(std::find(adj_list_[start].begin(),
                                    adj_list_[start].end(), finish));
    if (!is_directed_) {
      adj_list_[finish].erase(std::find(adj_list_[finish].begin(),
                                       adj_list_[finish].end(), start));
    }

    --edge_count_;
  }
}

const std::vector<Graph::Vertex> GraphAdjList::GetNeighbours(const Vertex& vertex) const {
  return adj_list_[vertex];
}

void GraphAdjList::Transpose() {
  std::vector<std::vector<Vertex>> temp(vertex_count_ + 1);
  for (int i = 1; i < vertex_count_ + 1; ++i) {
    size_t size = adj_list_[i].size();
    for (int j = 1; j < size; ++j) {
      temp[adj_list_[i][j]].push_back(i);
    }
  }
  adj_list_ = temp;
}
