#ifndef C___WEIGHTEDDIGRAPH_H
#define C___WEIGHTEDDIGRAPH_H

#include <list>
#include <vector>
#include <algorithm>


struct Node {
  std::list<std::pair<int, double>> neighbours;
  Node() = default;
  Node(std::list<std::pair<int, double>> neighbours);
  void add_edge(int node_id, double weight);
  int deg() const;
};

class WeightedDigraph {
public:
  WeightedDigraph(size_t num_nodes);
  void add_edge(int from, int to, double weight);
  std::list<std::pair<int, double>> adjList(int node_id) const;
  size_t num_nodes() const;
  int num_edges() const;
  int deg(int v) const;
  std::pair<int,double> min_entering(int node_id) const;
 
private:
  std::vector<Node> nodes;
  int edges;
  std::vector<std::pair<int, double>> mins;
};

#endif //C___WEIGHTEDDIGRAPH_H


