//  Kosaraju’s algorithm to find strongly connected components of a directed graph
//  Authors: Georǵi Kocharyan, Maximilian Keßler

#include <iostream>
#include <cstdio>
#include <vector>
#include <stack>

#include "../digraph.h"


int main() {
    int size = 10;
    Digraph G(size);
    G.add_edge(3,4);
    // G.add_edge(4,3);
    // G.add_edge(5,6);
    G.add_edge(6,7);
    G.add_edge(7,5);
    // G.add_edge(7,3);
    // G.add_edge(0,3);
    // G.add_edge(3,0);

    std::stack<int> zero_indegree;
    std::vector<int> indegs = G.indegrees();
    int amount = 0;

    for(int i = 0; i < size; i++)
    {
            if (indegs[i] == 0)
            {
                zero_indegree.push(i);
                amount++;
            }
    }

    while(!zero_indegree.empty()) 
    {
        int node_id = zero_indegree.top();
        zero_indegree.pop();
        std::cout << node_id << ' ';
        for(auto i: G.adjList(node_id))
        {
            if (indegs[i] = 1)
            {
                zero_indegree.push(i);
                amount++;
            }
            indegs[i]--;
        }
    }
    if (!(amount == size))
    {   
        std::cout << '\n' << "The graph contains cycles and thus has no topological order." << std::endl;
    }

    return 0;
}