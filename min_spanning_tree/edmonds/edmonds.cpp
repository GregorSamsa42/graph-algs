// Program implementing Edmond's algorithm for finding the minimum spanning arborescence in a weighted directed graph.
// Author: Georgi Kocharyan


#include <iostream>
#include <vector>
#include <limits>

#include "../../weighted_digraph.h"

WeightedDigraph guess_arborescence(WeightedDigraph const & G)
{
    // guess that a minimal arborescence could be one made out of the minimal edge going into each vertex
    // by default root it at 0
    // make sure to copy over the names!
    WeightedDigraph arborescence(G.num_nodes());
    for (int i = 1; i < G.num_nodes(); i++) {
        arborescence.add_edge(G.min_ingoing_edge(i));
        arborescence.name_node(i, G.node_name(i));
    }
    return arborescence;
}

std::vector<int> edges_to_nodes(std::vector<Edge> const & edges)
{
    // given the output of find_cycle, which is a vector of edges, we convert it to a vector of nodes to serve as input for contracting
    std::vector<int> nodes(edges.size());
    for (int i = 0; i < edges.size(); i++) {
        nodes[i] = edges[i].from;
    }
    return nodes;
}

WeightedDigraph edmonds(WeightedDigraph const & G)
{
    // H is the weighted digraph where we have subtracted off the weight of the lowest weight incoming edge from all edges incoming into a vertex
    WeightedDigraph H = G.modified_weights();
    // make a guess for the minimal arborescence and check if it works
    WeightedDigraph guess = guess_arborescence(H);
    std::vector<Edge> cycle = guess.find_cycle();
    if (cycle.empty()) {
        return guess;
    }
    // contract the cycle
    std::vector<Edge> min_edge_in;
    std::vector<Edge> min_edge_out;
    min_edge_out.reserve(H.num_nodes());
    min_edge_in.reserve(H.num_nodes());
    for (int i = 0; i < H.num_nodes(); i++) {
        min_edge_in.emplace_back(i, 0, std::numeric_limits<double>::max());
        min_edge_out.emplace_back(0,i, std::numeric_limits<double>::max());
    }
    WeightedDigraph contraction = H.contract_set(edges_to_nodes(cycle), min_edge_in, min_edge_out);
    // the vertices of contraction are now named by which node of the contraction mapped to them. Apply Edmonds recursively.
    WeightedDigraph contracted_max_arborescence = edmonds(contraction);
    // now recreate the actual arborescence in H.
    WeightedDigraph max_arborescence(H.num_nodes());
    for (int i = 0; i < contracted_max_arborescence.num_nodes(); i++) {
        for (auto const & edge : contracted_max_arborescence.adjList(i)) {
            // must differentiate between going into the contracted set, going out, or not.
            if (edge.from == contracted_max_arborescence.num_nodes() - 1) {
                max_arborescence.add_edge(min_edge_out[contracted_max_arborescence.node_name(edge.to)]);
            }
            else if (edge.to == contracted_max_arborescence.num_nodes() - 1) {
                max_arborescence.add_edge(min_edge_in[contracted_max_arborescence.node_name(edge.from)]);
            }
            else {
                max_arborescence.add_edge(contracted_max_arborescence.node_name(edge.from), contracted_max_arborescence.node_name(edge.to), edge.weight);
            }
        }
    }
    return max_arborescence;
}

int main()
{
    int size = 8;
    WeightedDigraph G(size);
    G.add_edge(3, 4, 2);
    G.add_edge(4, 3, 3);
    G.add_edge(5, 6, 6);
    G.add_edge(6, 7, 1);
    G.add_edge(1, 2, 3);
    G.add_edge(5, 1, 3);
    G.add_edge(2, 3, 8);
    G.add_edge(7, 5, 0.2);
    G.add_edge(7, 3, 9);
    G.add_edge(0, 3, 4);
    G.add_edge(3, 0, 5);
    G.add_edge(4, 6, 3);
    G.add_edge(0, 7, 0.5);
    G.add_edge(4, 2, 1);

    WeightedDigraph max_arborescence = edmonds(G);
    for (int i = 0; i < size; i++) {
        for (auto const & j : max_arborescence.adjList(i)) {
            std::cout << j.from << "-" << j.to << std::endl;
        }
    }
    return 0;
}
