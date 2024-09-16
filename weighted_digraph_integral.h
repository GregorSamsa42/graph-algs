//
// Created by gregorsamsa on 15.09.24.
//

#ifndef WEIGHTED_DIGRAPH_INTEGRAL_H
#define WEIGHTED_DIGRAPH_INTEGRAL_H
#include <list>
#include <vector>
#include <cstddef>


struct Edge
{
    int from;
    int to;
    int weight;

    Edge(int to_id, int wgt) : to(to_id), weight(wgt)
    {
    }

    Edge(int from_id, int to_id, int wgt) : from(from_id), to(to_id), weight(wgt)
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

    void add_edge(int from, int to, int weight);

    void give_name(int new_name);

    int deg() const;

    int name;


};

class WeightedDigraphIntegral
{
public:
    explicit WeightedDigraphIntegral(size_t num_nodes);

    void add_edge(int from, int to, int weight);

    void add_edge(Edge edge);

    int node_name(int node_id) const;

    std::list<Edge> adjList(int node_id) const;

    size_t num_nodes() const;

    int num_edges() const;

    int deg(int v) const;

    WeightedDigraphIntegral contract_set(std::vector<int> const & v, std::vector<Edge> & min_edges_in, std::vector<Edge> & min_edges_out) const;

    Edge min_ingoing_edge(int node_id) const;

    std::vector<Edge> find_cycle() const;

    WeightedDigraphIntegral modified_weights() const;

    void name_node(int node_id, int new_name);

    int node_name(int node_id);

    int max;

private:
    int edges;
    std::vector<Edge> mins;
    bool dfs(int v, std::vector<bool> & visited, std::vector<bool> & possible, std::vector<Edge> & cycle) const;
    std::vector<WeightedNode> nodes;
};
#endif //WEIGHTED_DIGRAPH_INTEGRAL_H
