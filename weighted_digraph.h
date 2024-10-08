#ifndef C___WEIGHTEDDIGRAPH_H
#define C___WEIGHTEDDIGRAPH_H

#include <list>
#include <vector>
#include <cstddef>

#include "digraph.h"



struct Edge
{
    int from;
    int to;
    double weight;

    Edge(int from_id, int to_id, double wgt) : from(from_id), to(to_id), weight(wgt)
    {
    }

    bool operator<(Edge const &other) const
    {
        return weight < other.weight;
    }
};

struct WeightedNode
{
    std::list<Edge> neighbours;

    WeightedNode() = default;

    WeightedNode(std::list<Edge> neighbours);

    WeightedNode(std::list<Edge> neighbours, int name);

    void add_edge(int from, int to, double weight);

    void give_name(int new_name);

    int deg() const;

    int name;


};

class WeightedDigraph
{
    friend class Network;
public:
    explicit WeightedDigraph(size_t num_nodes);

    void add_edge(int from, int to, double weight);

    void add_edge(Edge edge);

    int node_name(int node_id) const;

    std::list<Edge> adjList(int node_id) const;

    size_t num_nodes() const;

    int num_edges() const;

    int deg(int v) const;

    WeightedDigraph contract_set(std::vector<int> const & v, std::vector<Edge> & min_edges_in, std::vector<Edge> & min_edges_out) const;

    Edge min_ingoing_edge(int node_id) const;

    std::vector<Edge> find_cycle() const;

    WeightedDigraph modified_weights() const;

    void name_node(int node_id, int new_name);

    int node_name(int node_id);

    Digraph lose_weight() const;

private:
    int edges;
    std::vector<Edge> mins;
    bool dfs(int v, std::vector<bool> & visited, std::vector<bool> & possible, std::vector<Edge> & cycle) const;
    std::vector<WeightedNode> nodes;
};

#endif //C___WEIGHTEDDIGRAPH_H
