#include "weighted_graph.h"
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

WeightedGraph::WeightedGraph(size_t num_nodes) : nodes(num_nodes)
{

}

void WeightedGraph::add_edge(int from, int to, double weight)
{
  nodes[from].add_edge(to, weight);
  nodes[to].add_edge(from, weight);
}

std::list<std::pair<int, double>> WeightedGraph::adjList(int node_id) const
{
  return (nodes[node_id]).neighbours;
}

int WeightedGraph::deg(int node_id) const
{
  return nodes[node_id].deg();
}

size_t WeightedGraph::num_nodes() const
{
  return nodes.size();
}

std::pair<int,double> WeightedGraph::min_neighbour(int node_id) const
{
  if (adjList(node_id).empty())
  {
    return std::make_pair(0,0);
  }
  int result = (adjList(node_id).front()).first;
  int current_min = (adjList(node_id).front()).second;
  for (const auto& i : adjList(node_id))
  {
    if (current_min > i.second)
    {
      current_min = i.second;
      result = i.first;
    }
  }
  return std::make_pair(result,current_min);
}

// removes duplicate edges, leaving only the lightest edge (guarantees m = O(n^2))

WeightedGraph WeightedGraph::remove_parallel() const
{
  WeightedGraph G(num_nodes());
  for (int i = 0; i < num_nodes(); i++)
  {
    std::unordered_map<int, double> lightestEdges;
    for (const auto& j: adjList(i))
    {
      int to = j.first;
      double weight = j.second;
      if (lightestEdges.find(to) == lightestEdges.end() || weight < lightestEdges[to])
      {
         lightestEdges[to] = weight;
      }
    }
    for (const auto& [to, weight] : lightestEdges)
      {
        G.nodes[i].add_edge(to, weight);
      }
  }
  return G;
}

