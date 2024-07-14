#include "digraph.h"

Node::Node(std::list<int> neighbours_) : neighbours(std::move(neighbours_))
{

}

void Node::add_edge(int v)
{
  neighbours.push_back(v);
}

Digraph::Digraph(size_t num_nodes) : nodes(num_nodes)
{

}

void Digraph::add_edge(int from, int to)
{
  nodes[from].add_edge(to);
}

std::list<int> Digraph::adjList(int node_id) const
{
  return (nodes[node_id]).neighbours;
}

bool Digraph::isEdge(int from, int to) const
{
  return (std::find(adjList(from).begin(), adjList(from).end(), to) != adjList(from).end());
}

size_t Digraph::num_nodes() const
{
  return nodes.size();
}

Digraph Digraph::transpose() const
{
  Digraph G(num_nodes());
  for (int i = 0; i < num_nodes(); i++)
  {
    for (auto const& j: ((nodes[i]).neighbours))
    {
      G.add_edge(j,i);
    }
  }
  return G;
}
