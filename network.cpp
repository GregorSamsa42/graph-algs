#include "network.h"


Network::Network(int source, int sink, int num_nodes) : WeightedDigraph(num_nodes), source(source), sink(sink) {};


NetworkEdge::NetworkEdge(Edge edge) : Edge(edge)
{
    flow = 0;
}

void NetworkEdge::pump(double additional_flow)
{
    flow += additional_flow;
}

double NetworkEdge::rest_capacity() const
{
    return weight - flow;
}
