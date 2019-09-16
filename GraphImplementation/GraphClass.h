#ifndef MIPT_GRAPHCLASS_H_
#define MIPT_GRAPHCLASS_H_

#include <vector>

class Graph { // multi pseudo directed(undirected) graph
 protected:
  size_t vertex_count_;
  size_t edge_count_;
  bool is_directed_;

 public:
  typedef size_t Vertex;
  explicit Graph(size_t vertex_count = 0, size_t edge_count = 0, bool is_directed = false);

  size_t GetVertexCount() const;

  size_t GetEdgeCount() const;

  bool GetDirection() const;

  virtual bool HasEdge(const Vertex& start, const Vertex& finish) const = 0;

  virtual const std::vector<Vertex> GetNeighbours(const Vertex& vertex) const = 0;

  virtual void AddEdge(const Vertex& start, const Vertex& finish) = 0;

  virtual void DeleteEdge(const Vertex& start, const Vertex& finish) = 0;

  virtual void Transpose() = 0;
};

class GraphAdjList: public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_list_;

 public:
  explicit GraphAdjList(const std::vector<std::vector<Vertex>>& graph);
  void AddEdge(const Vertex& start, const Vertex& finish) override;
  bool HasEdge(const Vertex& start, const Vertex& finish) const override;
  void DeleteEdge(const Vertex& start, const Vertex& finish) override;
  const std::vector<Vertex> GetNeighbours(const Vertex& vertex) const override;
  void Transpose() override;

};

class GraphAdjMatrix: public Graph {
 private:
  std::vector<std::vector<Vertex>> adj_matrix_;

 public:
  explicit GraphAdjMatrix(const std::vector<std::vector<Vertex>>& graph);
  void AddEdge(const Vertex& start, const Vertex& finish) override;
  bool HasEdge(const Vertex& start, const Vertex& finish) const override;
  void DeleteEdge(const Vertex& start, const Vertex& finish) override;
  const std::vector<Vertex> GetNeighbours(const Vertex& vertex) const override;
  void Transpose() override;

};

#endif MIPT_GRAPHCLASS_H_
