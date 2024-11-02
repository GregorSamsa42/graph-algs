// Implementation of Karp's algorithm (1978) for finding a minimum mean cycle.
// Author: Georgi Kocharyan

#include <algorithm>
#include <iostream>
#include <limits>
#include <ostream>

#include <digraph.h>
#include <minimum_mean_cycle/kosaraju.h>


using WeightedDigraph = Digraph<WeightedEdge<double>>;

double karp(const WeightedDigraph & G, int starting_node)
{
    std::vector<std::vector<double>> F(G.num_nodes()+1, std::vector<double>(G.num_nodes(), std::numeric_limits<double>::max()));

    // compute the values of F recursively. F[k][i] is interpreted as the length of the shortest path from starting_node to i containing exactly k edges.
    F[0][starting_node] = 0;
    for (int k = 1; k <= G.num_nodes(); ++k) {
        for (int node_id = 0; node_id < G.num_nodes(); ++node_id) {
            for (const auto & edge : G.adjList(node_id)) {
                F[k][edge.to] = std::min(F[k][edge.to], F[k-1][edge.from] + edge.weight);
            }
        }
    }

    // compute min_{ x \in V(G) } max_{0 \leq k \leq n-1} (F[n][x] - F[k][x])/(n-k)
    std::vector<double> mins(G.num_nodes());
    for (int node_id = 0; node_id < G.num_nodes(); node_id++) {
        std::vector<double> maxs(G.num_nodes());
        for (int k = 0; k < G.num_nodes(); ++k) {
            maxs[k] = (F[G.num_nodes()][node_id] - F[k][node_id])/(G.num_nodes() - k);
        }
        mins[node_id]  = *max_element(maxs.begin(), maxs.end());
    }
    // double* ptr = &(*min_element(mins.begin(), mins.end()));
    // todo: store predecessor and output cycle
    return *min_element(mins.begin(), mins.end());
}

int main()
{
    constexpr int size = 3;
    WeightedDigraph G(size);
    G.add_edge(0,1,2);
    G.add_edge(1,2,3);
    G.add_edge(2,0,6);

    // first check if G is strongly connected, convert it to a regular digraph.

    int components = kosaraju(G.lose_weight());
    if (components != 1) {
        std::cout << "The given graph is not strongly connected." << std::endl;
    }
    else {
        double min_mean_cycle = karp(G, 0);

        // output the distances

        std::cout << "The minimum mean cycle has mean weight " << min_mean_cycle << "." << std::endl;
        return 0;
    }


    return 1;

}