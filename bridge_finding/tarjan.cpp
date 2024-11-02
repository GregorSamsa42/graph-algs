// Implementation of Tarjan's bridge finding algorithm
// Author: Georgi Kocharyan

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <functional>

#include "digraph.h"

using UndirectedGraph = Digraph<Edge>;

std::vector<Edge> tarjan(const UndirectedGraph & G)
{
    std::vector<Edge> bridges(G.num_edges());
}

int main() {
    const int size = 8;
    UndirectedGraph G(size);
    G.add_edge(3, 4);
    G.add_edge(2, 3);
    G.add_edge(4,2);
    G.add_edge(1, 2);
    G.add_edge(0,1);

    std::vector<Edge> edges = tarjan(G);

    return 0;

}
