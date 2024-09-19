#include "network.h"

#include <limits>


Network::Network(int source, int sink, int num_nodes) : WeightedDigraph(num_nodes), source(source), sink(sink), nodes(num_nodes)
{
    edges = 0;
    std::vector<Edge> v;
    v.reserve(num_nodes);
    for (int i = 0; i < num_nodes; i++) {
        v.emplace_back(0, i, std::numeric_limits<double>::infinity());
    }
    mins = v;
}

std::list<NetworkEdge> Network::adjList(int node_id) const
{
    return (nodes[node_id]).neighbours;
}

void Network::unmark()
{
    for (auto & node : nodes) {
        for (auto & edge : node.neighbours) {
            edge.marking = false;
        }
    }
}

void Network::add_edge(int from, int to, double weight)
{
    nodes[from].add_edge(from, to, weight);
    edges++;
    // check if weight of newly added edge is smaller than the min, and update accordingly
    // the entries of min should not be interpreted as Edges
    if (mins[to].weight > weight) {
        const Edge v(from, to, weight);
        mins[to] = v;
    }
};


NetworkEdge::NetworkEdge(Edge edge) : Edge(edge)
{
    flow = 0;
    marking = false;
}

NetworkEdge::NetworkEdge(int from_id, int to_id, double wgt) : Edge(from_id, to_id, wgt)
{
    flow = 0;
    marking = false;
}

void NetworkEdge::pump(double additional_flow)
{
    flow += additional_flow;
}

double NetworkEdge::rest_capacity() const
{
    return weight - flow;
}

void NetworkEdge::mark()
{
    marking = true;
}

void NetworkWeightedNode::add_edge(int from, int to, double weight)
{
    neighbours.emplace_back(from, to, weight);
}

