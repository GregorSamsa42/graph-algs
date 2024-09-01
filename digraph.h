#ifndef C___DIGRAPH_H
#define C___DIGRAPH_H

#include <list>
#include <vector>
#include <algorithm>


struct Node
{
    std::list<int> neighbours;

    Node() = default;

    Node(std::list<int> neighbours);

    void add_edge(int v);

    int outdeg() const;
};

class Digraph
{
public:
    Digraph(size_t num_nodes);

    void add_edge(int from, int to);

    std::list<int> adjList(int node_id) const;

    size_t num_nodes() const;

    Digraph transpose() const;

    bool isEdge(int from, int to) const;

    int outdeg(int v) const;

    std::vector<int> indegrees() const;

    int pop_edge(int v);

private:
    std::vector<Node> nodes;
};

#endif //C___DIGRAPH_H
