//  Kosaraju’s algorithm to find strongly connected components of a directed graph
//  Authors: Georǵi Kocharyan, Maximilian Keßler

#include <iostream>
#include <cstdio>
#include <vector>
#include <stack>

#include "digraph.h"


using UnweightedDigraph = Digraph<Edge>;
// push nodes in post-order
void dfs1(UnweightedDigraph const & G, int n, std::vector<bool> & vis, std::stack<int> & node_order) {
    if (vis[n])
    {
      return;  //if node is already visited don't
    }

    vis[n] = true;

    for(auto i: G.adjList(n))
    {
        dfs1(G, i.to, vis, node_order);
    }

    node_order.push(n);
}

//this function traverses the transpose graph
void dfs2(UnweightedDigraph const & G, int n, std::vector<bool> & vis2, std::stack<int> & node_order){
    if (vis2[n])
    {
      return;  // if node is already visited
    }

    std::cout << n << " ";
    vis2[n] = true;

    for(auto i: G.adjList(n))
    {
        dfs2(G, i.to, vis2, node_order);
    }
}

// print each component in seperate line, output amount
int kosaraju(UnweightedDigraph const & G) {
    int scc_count = 0;   //keep count of strongly connected components
    std::stack<int> node_order;
    std::vector<bool> vis2 (G.num_nodes(), false);  
    std::vector<bool> vis (G.num_nodes(), false);                       //store node visit stat for dfs
    for(int i = 0; i < G.num_nodes(); i++)
    {
            dfs1(G, i, vis, node_order);
    }
    while(!node_order.empty()) {
        int node_id = node_order.top();
        node_order.pop();
        if (vis2[node_id] == false)
        {
            dfs2(G.transpose(), node_id, vis2, node_order);
            scc_count++;
            std::cout << std::endl;
        }
    }
    return scc_count;
}