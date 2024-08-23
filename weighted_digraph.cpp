#include "weighted_digraph.h"
#include <unordered_map>
#include <iostream>

Node::Node(std::list<Edge> neighbours_) : neighbours(std::move(neighbours_))
{
  name = 0;
  marking = false;
}

Node::Node(std::list<Edge> neighbours_, int name) : neighbours(std::move(neighbours_)), name(name)
{
  marking = false;
}

void Node::add_edge(int node_id, double weight)
{
  neighbours.emplace_back(node_id,weight);
}

int Node::deg() const
{
  return neighbours.size();
}

void Node::mark()
{
  marking = true;
}

WeightedDigraph::WeightedDigraph(size_t num_nodes) : nodes(num_nodes)
{
  edges = 0;
  std::vector<Edge> v;
  v.reserve(num_nodes);
  for (int i = 0; i < num_nodes; i++) {
    v.emplace_back(0,i,std::numeric_limits<double>::infinity());
  }
  mins = v;
}


void WeightedDigraph::add_edge(Edge edge)
{
  add_edge(edge.from, edge.to, edge.weight);
}

void WeightedDigraph::add_edge(int from, int to, double weight)
{
  nodes[from].add_edge(to, weight);
  edges++;
  // check if weight of newly added edge is smaller than the min, and update accordingly
  // the entries of min should not be interpreted as Edges
  if (mins[to].weight > weight)
  {
    Edge v(from, to, weight);
    mins[to] = v;
  }
}


std::list<Edge> WeightedDigraph::adjList(int node_id) const
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

WeightedDigraph WeightedDigraph::contract_set(std::vector<int> v) {
  WeightedDigraph digraph(num_nodes()-v.size()+1);
  int count = 0;
  // marking is interpreted here as being within the contracted set: this is to minimise computation
  for (int i : v) {
    nodes[i].mark();
  }
  // want to store which nodes in the new graph the old nodes are mapped to
  std::vector<int> mappedto(num_nodes());
  for (int i = 0; i < num_nodes(); i++) {
    if (!nodes[i].marking) {
      digraph.nodes[count].name = i;
      mappedto[i] = count;
      count++;
    }
    else {
      // everything in the contracted set gets mapped to the final node.
      mappedto[i] = num_nodes()-v.size();
    }
  }
  for (int i = 0; i < num_nodes(); i++) {
    for (auto j : nodes[i].neighbours) {
      if (!nodes[i].marking) {
        digraph.add_edge(mappedto[i], mappedto[j.to], j.weight);
      }
      else {
        digraph.add_edge(num_nodes()-v.size(), mappedto[j.to], j.weight);
      }
    }
  }
  return digraph;
}

Edge WeightedDigraph::min_ingoing_edge(int node_id) const {
  return mins[node_id];
}




