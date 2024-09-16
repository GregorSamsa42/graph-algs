// Implementation of the Floyd-Warshall algorithm for finding all pairs of distances in weighted digraphs.
// Need the weights to be conservative, i.e. no negative weight cycle to exist.
// Author: Georgi Kocharyan

#include <algorithm>
#include <iostream>
#include <limits>
#include <ostream>

#include "weighted_digraph.h"

void floyd_warshall (const WeightedDigraph & G, std::vector<std::vector<double>> & min_distances)
{
    for (int i = 0; i < G.num_nodes(); i++) {
        min_distances[i][i] = 0;
    }
    for (int i = 0; i < G.num_nodes(); i++) {
        for (const auto & edge : G.adjList(i)) {
            min_distances[edge.to][edge.from] = edge.weight;
        }
    }
    // d_i,j(0) is now initialised. Now run through the recursion n times.
    std::vector<std::vector<double>> temp(G.num_nodes(), std::vector<double>(G.num_nodes()));
    for (int k = 0; k < G.num_nodes(); k++) {
        for (int i = 0; i < G.num_nodes(); i++) {
            for (int j = 0; j < G.num_nodes(); j++) {
                temp[i][j] = std::min(min_distances[i][j], min_distances[i][k] + min_distances[k][j]);
            }
        }
        // set the k-th iteration of min_distances to temp
        for (int i = 0; i < G.num_nodes(); i++) {
            for (int j = 0; j < G.num_nodes(); j++) {
                min_distances[i][j] = temp[i][j];
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
    G.add_edge(7,3,-1);
    G.add_edge(0,3,1);
    G.add_edge(3,0,5);
    G.add_edge(4,6,3);
    G.add_edge(0,7,0.5);
    G.add_edge(4,2,1);
    // initialise all min distances to infinity

    std::vector<std::vector<double>> min_distances(size, std::vector<double>(size, std::numeric_limits<double>::max()));

    floyd_warshall(G, min_distances);

    // output the distances

        for (int i = 0; i < G.num_nodes(); ++i) {
            for (int j = 0; j < G.num_nodes(); ++j) {
                if (min_distances[i][j] == std::numeric_limits<double>::max()) {
                    std::cout << "There is no path from " << i << " to " << j << " ." << std::endl;
                }
                else {
                    std::cout << i << " has distance " << min_distances[i][j] << " from " << j << "." << std::endl;
                }
            }

        }

    return 0;

}