//
// Created by gregorsamsa on 19.09.24.
//

#include <iostream>
#include <ostream>

#include "network.h"
#include "weighted_digraph.h"

void dfs(Network const & G, const NetworkEdge & edge, const int & sink, std::vector<bool> & vis, std::vector<NetworkEdge> & path, bool & found) {
    if (vis[edge.to] || found || !edge.marking)
    {
        return;  //if node is already visited don't
    }
    vis[edge.to] = true;
    path.push_back(edge);
    if (edge.to == sink) {
        found = true;
        return;
    }
    for(const auto & i: G.adjList(edge.to))
    {
        dfs(G, i, sink, vis,path, found);
    }
    path.pop_back();
}

double ford_fulkerson(Network & G)
{
    NetworkEdge fake_edge(-1,G.source, 0);
    fake_edge.mark();
    double max_flow = 0;
    // put all edges into vector and sort by rest capacity
    std::list<NetworkEdge*> G_edges;
    for (int node_id = 0; node_id < G.num_nodes(); ++node_id) {
        for (auto & edge: G.adjList(node_id)) {
            G_edges.push_back(&edge);
        }
    }
    G_edges.sort();
    // recursively add into residual graph, and stop as soon as there is an s-t path
    std::vector<NetworkEdge> path;
    path.reserve(G.num_nodes());
    bool found_path = true;

    // found_path is true for the while loop to start. after that it will only be set to false at the end if none is found
    while (found_path) {
        found_path = false;
        std::vector<bool> vis(G.num_nodes(), false);
        for (auto & edge: G_edges) {
            if (edge->rest_capacity() > 0) {
                edge->mark();
                dfs(G, fake_edge, G.sink, vis, path, found_path);
                if (found_path) {
                    // unmark all edges and continue
                    G.unmark();
                    break;
                }
            }
        }
        // if a path has been found, augment appropriately
        if (found_path) {
            // find the edge in the path with maximal rest capacity
            const double augment = (*std::max_element(path.begin(), path.end())).rest_capacity();
            for (auto & edge: path) {
                edge.pump(augment);
                max_flow += augment;
            }
        }
    }
return max_flow;
}

int main()
{
    constexpr int size = 4;
    Network G(0,3,size);
    G.add_edge(0,1,4);
    G.add_edge(0,2,5);
    G.add_edge(1,3,2);
    G.add_edge(2,3,3);

    const double max_flow = ford_fulkerson(G);

    std::cout << max_flow << std::endl;
}
