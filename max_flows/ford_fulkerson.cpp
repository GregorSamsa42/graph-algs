// Implementation of the Ford-Fulkerson algorithm using an edge-sorting trick to keep the runtime O(m^3*log(W)), where
// W is the maximum flow (or the sum of all capacities exiting the source, as an upper bound)
// Author: Georgi Kocharyan

#include <iostream>
#include <ostream>

#include "digraph.h"

using Network = Digraph<NetworkEdge<double>>;
using Edge_n = NetworkEdge<double>;

bool rest_capacity_order(const Edge_n *a, const Edge_n *b)
{
    return a->rest_capacity() > b->rest_capacity();
}

void dfs(Network & G, Edge_n & edge, const int & sink, std::vector<bool> & vis, std::vector<Edge_n*> & path_in_G, bool & found) {
    if (vis[edge.to] || found || !edge.marking || edge.rest_capacity() == 0)
    {
        return;  //if node is already visited don't
    }
    vis[edge.to] = true;
    path_in_G.push_back(&edge);
    if (edge.to == sink) {
        found = true;
        return;
    }
    for(auto & i: G.adjList_ref(edge.to))
    {
        dfs(G, i, sink, vis,path_in_G, found);
        if (found) {
            return;
        }
    }
    path_in_G.pop_back();
}

double ford_fulkerson(Network & G, int source, int sink)
{
    Edge_n fake_edge(-1,source, 1,0);
    fake_edge.mark();
    double max_flow = 0;

    // turn G into its residual graph by adding reverse edges
    for (int node_id = 0; node_id < G.num_nodes(); ++node_id) {
        for (auto & edge: G.adjList_ref(node_id)) {
            if (!edge.reversed) {
                Edge_n edge_reversed(edge.to,edge.from,edge.capacity,0);
                edge_reversed.reversed = true;
                // partner up edge pairs
                edge_reversed.partner = &edge;
                edge.partner = &edge_reversed;
                // add to edge list
                G.add_edge(edge_reversed);
            }
        }
    }

    std::vector<Edge_n*> residual_edges;
    residual_edges.reserve(G.num_edges());
    for (int node_id = 0; node_id < G.num_nodes(); ++node_id) {
        for (auto & edge: G.adjList_ref(node_id)) {
            residual_edges.push_back(&edge);
        }
    }
    std::sort(residual_edges.begin(), residual_edges.end(), rest_capacity_order);
    // recursively add into residual graph, and stop as soon as there is an s-t path
    Network residual_graph(G.num_nodes());
    bool found_path = true;

    // found_path is true for the while loop to start. after that it will only be set to false at the end if none is found
    while (found_path) {
        std::vector<Edge_n*> path;
        path.reserve(G.num_nodes());
        found_path = false;
        std::vector<bool> vis(G.num_nodes(), false);
        std::sort(residual_edges.begin(), residual_edges.end(), rest_capacity_order);
        for (const auto & edge: residual_edges) {
            edge->mark();
            dfs(G, fake_edge, sink, vis, path, found_path);
            std::fill(vis.begin(), vis.end(), false);
            if (found_path) {
                // unmark all edges and continue
                for (const auto & e: residual_edges) {
                    e->unmark();
                }
                break;
            }
        }
        // if a path has been found, augment appropriately
        if (found_path) {
            // find the edge in the path with maximal rest capacity
            // ignore the first edge as it is always fake_edge
            const double augment = (*std::max_element(++path.begin(), path.end(),rest_capacity_order))->rest_capacity();
            for (int i = 1; i < path.size(); i++) {
                if (!path[i]->reversed) {
                    path[i]->pump(augment);
                    path[i]->partner->pump(augment);
                }
                else {
                    path[i]->pump(-augment);
                    path[i]->partner->pump(-augment);
                }
            }
            max_flow += augment;
        }
    }
return max_flow;
}

int main()
{
    constexpr int size = 5;
    Network G(size);
    G.add_edge(0,1,4,0);
    G.add_edge(0,2,5,0);
    G.add_edge(1,4,1,0);
    G.add_edge(1,3,2,0);
    G.add_edge(2,3,3,0);

    const double max_flow = ford_fulkerson(G,0,3);

    std::cout << "The maximum flow is " << max_flow << "." << std::endl;
    for (int node_id = 0; node_id < size; ++node_id) {
        for (const auto & edge: G.adjList(node_id)) {
            if (!edge.reversed) {
                std::cout << "The edge from " << edge.from << " to " << edge.to << " has capacity " << edge.capacity << " and flow " << edge.flow << "." << std::endl;
            }
        }
    }
}
