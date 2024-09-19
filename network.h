//
// Created by gregorsamsa on 19.09.24.
//

#ifndef NETWORK_H
#define NETWORK_H
#include "weighted_digraph.h"

struct NetworkEdge : Edge
{
    double flow;
    NetworkEdge(Edge edge); // flow always initialised to zero
    void pump(double additional_flow);
    double rest_capacity() const;
    // want to sort edges based on rest capacity, with the maximal ones coming first
    bool operator<(NetworkEdge const &other) const
    {
        return rest_capacity() > other.rest_capacity();
    }
};

class Network : public WeightedDigraph
{
    public:
    int source;
    int sink;
    Network(int source, int sink, int num_nodes);
    std::list<Edge> adjList(int node_id) const;
};

#endif //NETWORK_H
