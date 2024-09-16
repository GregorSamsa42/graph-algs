// Implementation of the Moore-Bellman-Ford algorithm for finding minimum distances in weighted digraphs.
// Need the weights to be conservative, i.e. no negative weight cycle to exist.
// outputs the existence of a negative cycle, in case one exists.
// Author: Georgi Kocharyan

#include <iostream>
#include <limits>
#include <ostream>

#include "weighted_digraph.h"

void moore_bellman_ford(const WeightedDigraph & G, std::vector<double> & min_distances, const int measuring_from, bool & negative_cycle)
{
    min_distances[measuring_from] = 0;
    for (int i = 1; i < G.num_nodes(); i++) {
        for (int node_id = 0; node_id < G.num_nodes(); node_id++) {
            for (const auto & edge : G.adjList(node_id)) {
                min_distances[edge.to] = std::min(min_distances[edge.to], min_distances[edge.from] + edge.weight);
            }
        }
    }
    // if there is no negative cycle, we have correctly determined the min_distances. Repeating the loop once will reveal the existence of one if the min_distances change.
    for (int node_id = 0; node_id < G.num_nodes(); node_id++) {
        for (const auto & edge : G.adjList(node_id)) {
            if (min_distances[edge.to] < min_distances[edge.from] + edge.weight) {
                negative_cycle = true;
            }
        }
    }
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
    G.add_edge(7,3,-9);
    G.add_edge(0,3,1);
    G.add_edge(3,0,5);
    G.add_edge(4,6,3);
    G.add_edge(0,7,0.5);
    G.add_edge(4,2,1);

    std::vector<double> min_distances(size, std::numeric_limits<double>::max());
    constexpr int measuring_from = 0;
    bool negative_cycle = false;

    moore_bellman_ford(G, min_distances, measuring_from, negative_cycle);

    if (negative_cycle) {
        std::cout << "Negative cycle detected, minimum distance is not well-defined." << std::endl;
    }
    else {
        std::cout << measuring_from << " is the root node." << std::endl;
        for (int i = 0; i < G.num_nodes(); ++i) {
            if (i == measuring_from) {
                continue;
            }
            std::cout << i;
            if (min_distances[i] == std::numeric_limits<double>::max()) {
                std::cout << " is inaccessible from " << measuring_from << "." << std::endl;
            }
            else {
                std::cout << " has distance " << min_distances[i] << " from the root node." << std::endl;
            }
        }
    }

    return 0;

}


