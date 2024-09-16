// Implementation of Karp's algorithm (1978) for finding a minimum mean cycle.
// Author: Georgi Kocharyan
#include <algorithm>
#include <iostream>
#include <limits>
#include <ostream>

#include "weighted_digraph.h"
#include "digraph.h"
#include "Kosaraju/kosaraju.h"

double karp(const WeightedDigraph & G)
{
    std::vector<std::vector<double>> F(G.num_nodes()+1, std::vector<double>(G.num_nodes(), std::numeric_limits<double>::max()));
}

int main()
{
    constexpr int size = 8;
    WeightedDigraph G(size);
    G.add_edge(3,4,2);
    G.add_edge(4,3,3);
    G.add_edge(5,6,6);
    G.add_edge(6,7,1);
    G.add_edge(1,2,3);
    G.add_edge(2,3,8);
    G.add_edge(7,5,0.2);
    G.add_edge(7,3,-1);
    G.add_edge(0,3,1);
    G.add_edge(3,0,5);
    G.add_edge(4,6,3);
    G.add_edge(0,7,0.5);
    G.add_edge(4,2,1);

    // first check if G is strongly connected, convert it to a regular digraph.

    int components = kosaraju(G.lose_weight());
    if (components != 1) {
        std::cout << "The given graph is not strongly connected." << std::endl;
    }

    double min_mean_cycle = karp(G);

    // output the distances

    std::cout << "The minimum mean cycle has mean weight " << min_mean_cycle << "." << std::endl;

    return 0;

}