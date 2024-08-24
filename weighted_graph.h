#ifndef C___WEIGHTEDGRAPH_H
#define C___WEIGHTEDGRAPH_H

#include <list>
#include <vector>
#include <algorithm>


struct Node
{
    std::list<std::pair<int, double> > neighbours;

    Node() = default;

    Node(std::list<std::pair<int, double> > neighbours);

    void add_edge(int node_id, double weight);

    int deg() const;
};

class WeightedGraph
{
public:
    WeightedGraph(size_t num_nodes);

    void add_edge(int from, int to, double weight);

    std::list<std::pair<int, double> > adjList(int node_id) const;

    size_t num_nodes() const;

    int num_edges() const;

    int deg(int v) const;

    std::pair<int, double> min_neighbour(int node_id) const;

    WeightedGraph remove_parallel() const;

private:
    std::vector<Node> nodes;
    int edges;
};

#endif //C___WEIGHTEDGRAPH_H
