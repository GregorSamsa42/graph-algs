//
// Created by gregorsamsa on 19.09.24.
//

#include <iostream>
#include <ostream>

#include "network.h"
#include "weighted_digraph.h"

bool rest_capacity_order(const NetworkEdge *a, const NetworkEdge *b)
{
    return (*a) < (*b);
}

void dfs(Network & G, NetworkEdge & edge, const int & sink, std::vector<bool> & vis, std::vector<NetworkEdge*> & path, bool & found) {
    if (vis[edge.to] || found || !edge.marking)
    {
        return;  //if node is already visited don't
    }
    vis[edge.to] = true;
    path.push_back(&edge);
    if (edge.to == sink) {
        found = true;
        return;
    }
    for(auto & i: G.adjList(edge.to))
    {
        dfs(G, i, sink, vis,path, found);
        if (found) {
            return;
        }
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
    G_edges.sort(rest_capacity_order);
    // recursively add into residual graph, and stop as soon as there is an s-t path
    bool found_path = true;

    // found_path is true for the while loop to start. after that it will only be set to false at the end if none is found
    while (found_path) {
        std::vector<NetworkEdge*> path;
        path.reserve(G.num_nodes());
        found_path = false;
        std::vector<bool> vis(G.num_nodes(), false);
        for (const auto & edge: G_edges) {
            if (edge->rest_capacity() > 0) {
                edge->mark();
                dfs(G, fake_edge, G.sink, vis, path, found_path);
                std::fill(vis.begin(), vis.end(), false);
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
            // ignore the first edge as it is always fake_edge
            const double augment = (*std::max_element(++path.begin(), path.end(),rest_capacity_order))->rest_capacity();
            for (int i = 1; i < path.size(); i++) {
                path[i]->pump(augment);
            }
            max_flow += augment;
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

    std::cout << "The maximum flow is " << max_flow << "." << std::endl;
    for (int node_id = 0; node_id < size; ++node_id) {
        for (const auto & edge: G.adjList(node_id)) {
            std::cout << "The edge from " << edge.from << " to " << edge.to << " has capacity " << edge.weight << " and flow " << edge.flow << "." << std::endl;
        }
    }
}
