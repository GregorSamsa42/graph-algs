#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

#include "../../weighted_digraph.h"

WeightedDigraph guess_arborescence(WeightedDigraph G) {
    // guess that a minimal arborescence could be one made out of the minimal edge going into each vertex
    // by default root it at 0
    WeightedDigraph arborescence(G.num_nodes());
    for (int i = 1; i < G.num_nodes(); i++) {
        arborescence.add_edge(G.min_ingoing_edge(i));
    }
    return arborescence;
}

void edmonds(WeightedDigraph G) {

}

int main() {

    int size = 8;
    WeightedDigraph G(size);
    G.add_edge(3,4,2);
    G.add_edge(4,3,3);
    G.add_edge(5,6,6);
    G.add_edge(6,7,1);
    G.add_edge(1,2,3);
    G.add_edge(2,3,8);
    G.add_edge(7,5,0.2);
    G.add_edge(7,3,9);
    G.add_edge(0,3,1);
    G.add_edge(3,0,5);
    G.add_edge(4,6,3);
    G.add_edge(0,7,0.5);
    G.add_edge(4,2,1);

    
    return 0;

}
