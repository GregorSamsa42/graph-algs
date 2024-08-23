#include "weighted_digraph.h"
#include <unordered_map>
#include <iostream>

Node::Node(std::list<std::pair<int, double>> neighbours_) : neighbours(std::move(neighbours_))
{

}

void Node::add_edge(int node_id, double weight)
{
  neighbours.push_back(std::make_pair(node_id,weight));
}

int Node::deg() const
{
  return neighbours.size();
}

WeightedDigraph::WeightedDigraph(size_t num_nodes) : nodes(num_nodes)
{
  edges = 0;
  std::vector<std::pair<int, double>> v(num_nodes, std::make_pair(0,std::numeric_limits<double>::infinity())); 
  mins = v;
}

void WeightedDigraph::add_edge(int from, int to, double weight)
{
  nodes[from].add_edge(to, weight);
  edges++;
  if (mins[to].second > weight)
  {
    mins[to] = std::make_pair(from, weight);
  }
}

std::list<std::pair<int, double>> WeightedDigraph::adjList(int node_id) const
{
  return (nodes[node_id]).neighbours;
}

int WeightedDigraph::deg(int node_id) const
{
  return nodes[node_id].deg();
}

size_t WeightedDigraph::num_nodes() const
{
  return nodes.size();
}

int WeightedDigraph::num_edges() const
{
  return edges;
}

std::pair<int,double> WeightedDigraph::min_entering(int node_id) const
{
  return mins[node_id];
}

