//
// Program to find Euler cycles in a directed graph.
// Author: Georgi Kocharyan

#include <iostream>
#include <list>
#include <ostream>

#include "digraph.h"

struct Edge
{
    int from;
    int to;

    Edge(int from_id, int to_id) : from(from_id), to(to_id)
    {
    }
};

std::list<Edge> euler_cycle(Digraph & G, int node)
{
    std::list<Edge> result;
    if (G.outdeg(node) == 0) {
        return result;
    }
    while (G.outdeg(node) != 0) {
        int neighbour = G.pop_edge(node);
        result.emplace_back(node, neighbour);
        node = neighbour;
    }
    auto itr = result.begin();
    for (auto const & edge : result) {
        std::list<Edge> small_euler = euler_cycle(G, edge.from);
        result.insert(itr, small_euler.begin(), small_euler.end());
        ++itr;
    }
    return result;
}

int main()
{
    constexpr int size = 5;
    Digraph G(size);
    G.add_edge(0,1);
    G.add_edge(1,2);
    G.add_edge(2,3);
    G.add_edge(3,4);
    G.add_edge(4,1);
    G.add_edge(1,4);
    G.add_edge(4,0);

    std::list<Edge> result = euler_cycle(G, 0);

    for (auto const & edge : result) {
        std::cout << edge.from << "-" << edge.to << std::endl;
    }
}
