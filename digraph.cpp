#include "digraph.h"

Node::Node(std::list<int> neighbours_) : neighbours(std::move(neighbours_))
{
}

void Node::add_edge(int v)
{
    neighbours.push_back(v);
}

int Node::outdeg() const
{
    return neighbours.size();
}

Digraph::Digraph(size_t num_nodes) : nodes(num_nodes)
{
}

void Digraph::add_edge(const int from, const int to)
{
    nodes[from].add_edge(to);
}

std::list<int> Digraph::adjList(const int node_id) const
{
    return (nodes[node_id]).neighbours;
}

bool Digraph::isEdge(const int from, const int to) const
{
    return (std::find(adjList(from).begin(), adjList(from).end(), to) != adjList(from).end());
}

int Digraph::outdeg(int v) const
{
    return nodes[v].outdeg();
}

std::vector<int> Digraph::indegrees() const
{
    std::vector<int> indegs(num_nodes(), 0);
    for (int i = 0; i < num_nodes(); i++) {
        for (auto const &j: ((nodes[i]).neighbours)) {
            indegs[j]++;
        }
    }
    return indegs;
}

int Digraph::pop_edge(const int v)
{
    const int result = adjList(v).front();
    (nodes[v].neighbours).pop_front();
    return result;
}

size_t Digraph::num_nodes() const
{
    return nodes.size();
}

Digraph Digraph::transpose() const
{
    Digraph G(num_nodes());
    for (int i = 0; i < num_nodes(); i++) {
        for (auto const &j: ((nodes[i]).neighbours)) {
            G.add_edge(j, i);
        }
    }
    return G;
}
