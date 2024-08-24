//  Algorithm outputting a topological order of a directed graph
//  Authors: Georǵi Kocharyan

#include <iostream>
#include <cstdio>
#include <vector>
#include <stack>

#include "../digraph.h"

void top_order(Digraph G)
{
    // keeps track of vertices with zero indegree, these can be put at the beginning
    std::stack<int> zero_indegree;

    std::vector<int> indegs = G.indegrees();
    int amount = 0;

    for (int i = 0; i < G.num_nodes(); i++) {
        if (indegs[i] == 0) {
            zero_indegree.push(i);
            amount++;
        }
    }
    // update indegs, zero_indegree after adding a vertex to the top. order
    while (!zero_indegree.empty()) {
        int node_id = zero_indegree.top();
        zero_indegree.pop();
        std::cout << node_id << ' ';
        for (auto i: G.adjList(node_id)) {
            if (indegs[i] = 1) // this ensures each vertex added to stack only once
            {
                zero_indegree.push(i);
                amount++;
            }
            indegs[i]--;
        }
    }
    if (!(amount == G.num_nodes())) {
        std::cout << '\n' << "The graph contains cycles and thus has no topological order." << std::endl;
    }
}

int main()
{
    int size = 10;
    Digraph G(size);
    G.add_edge(3, 4);
    G.add_edge(6, 7);
    G.add_edge(7, 5);

    top_order(G);

    return 0;
}
