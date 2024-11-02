//  Algorithm generating a minimum spanning tree using Kruskal's algorithm
// we use a directed graph class to model an undirected graph
//  Authors: Georgi Kocharyan

#include <iostream>
#include <vector>
#include <algorithm>

#include "digraph.h"

using WeightedGraph = Digraph<WeightedEdge<double>>;
using Edge_w = WeightedEdge<double>;
using Node_w = Node<WeightedEdge<double>>;


bool isLighter (const Edge_w e1, const Edge_w e2)
{
    return e1.weight < e2.weight;
}


void kruskal(WeightedGraph const &G)
{
    // preprocessing: remove all double edges except the minimal ones, treat as G a undirected graph.

    WeightedGraph H(G.num_nodes());

    for (int i = 0; i < G.num_nodes(); i++) {
        for (const auto edge: G.adjList(i)) {
            H.add_edge(edge.to,edge.from, edge.weight);
            H.add_edge(edge.from,edge.to, edge.weight);
        }
    }

    H = H.remove_parallel();

    double total_weight = 0;

    // preprocessing: create a vector of lists tracking the elements of the components

    std::vector<std::vector<int> > elements;
    elements.resize(H.num_nodes());
    for (int i = 0; i < H.num_nodes(); i++) {
        elements[i].push_back(i);
    }

    // preprocessing: create a vector tracking which component each element belongs to

    std::vector<int> component;
    component.resize(H.num_nodes());
    for (int i = 0; i < H.num_nodes(); i++) {
        component[i] = i;
    }

    // preprocessing: create a vector containing all edges in O(m)
    // then sort them according to their weight

    std::vector<Edge_w> edges;
    edges.reserve(H.num_edges());
    for (int i = 0; i < H.num_nodes(); i++) {
        for (const auto &j: H.adjList(i)) {
            edges.emplace_back(j);
        }
    }
    std::sort(edges.begin(), edges.end(), isLighter);

    for (const auto &edge: edges) {
        // check if edge connects two of the same component
        if (component[edge.from] != component[edge.to]) {
            // output edge
            std::cout << edge.from << "-" << edge.to << "\t" << edge.weight << std::endl;
            total_weight = total_weight + edge.weight;
            // make the components of each node the same
            // the larger component absorbs the second to guarantee O(mlogn) runtime
            int moved;
            int movedto;
            if (elements[component[edge.from]].size() >= elements[component[edge.to]].size()) {
                // move all elements of second component to first
                moved = component[edge.to];
                movedto = component[edge.from];
            } else {
                // move all elements of first component to second
                moved = component[edge.from];
                movedto = component[edge.to];
            }
            for (const auto &node: (elements[moved])) {
                elements[movedto].push_back(node);
                component[node] = movedto;
            }
            elements[moved].clear();
        }
    }
    std::cout << "The total weight of the MST is " << total_weight << std::endl;
}

// example of a CONNECTED input graph

int main()
{
    constexpr int size = 8;
    WeightedGraph G(size);
    G.add_edge(3, 4, 2);
    G.add_edge(4, 3, 3);
    G.add_edge(5, 6, 6);
    G.add_edge(6, 7, 1);
    G.add_edge(1, 2, 3);
    G.add_edge(2, 3, 8);
    G.add_edge(7, 5, 0.2);
    G.add_edge(7, 3, 9);
    G.add_edge(0, 3, 1);
    G.add_edge(3, 0, 5);
    G.add_edge(4, 6, 3);
    G.add_edge(0, 7, 0.5);
    G.add_edge(4, 2, 1);

    kruskal(G);

    return 0;
}
