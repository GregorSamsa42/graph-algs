// Implementation of the Ford-Fulkerson algorithm using the Edmonds-karp method of picking augmenting paths
// (pick those of minimal distance). This guarantees a runtime of O(nm^2) which is independent of the
// absolute capacity values.
// Author: Georgi Kocharyan

#include <iostream>
#include <limits>
#include <ostream>
#include <queue>
#include "digraph.h"

using Network = Digraph<NetworkEdge<double>>;
using Edge_n = NetworkEdge<double>;

void bfs(Network & G, const int & source, const int & sink, std::vector<Edge_n*> & predecessors, bool & found) {
    std::queue<Edge_n*> q;
    Edge_n start(-1,source,1,0);
    q.push(&start);
    std::vector<bool> vis(G.num_nodes(), false);
    while (!q.empty()) {
        Edge_n* edge = q.front();
        q.pop();
        if (vis[edge->to] || found)
        {
            continue;  //if node is already visited don't
        }
        vis[edge->to] = true;
        predecessors[edge->to] = edge;
        // push all neighbours into the queue. In case we reach the sink, terminate instantly (this guarantees a shortest path)
        for(auto & neighbour: G.adjList_ref(edge->to))
        {
            if (neighbour.rest_capacity() == 0) {
                continue;
            }
            if (neighbour.to != sink) {
                q.push(&neighbour);
            }
            else {
                predecessors[sink] = &neighbour;
                found = true;
                predecessors[source] = nullptr;
                return;
            }
        }
    }
}

double ford_fulkerson(Network & G, int source, int sink)
{
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
    bool found_path = true;

    // found_path is true for the while loop to start. after that it will only be set to false at the end if none is found
    while (found_path) {
        std::vector<Edge_n*> predecessors(G.num_nodes(), nullptr);
        found_path = false;
        bfs(G, source,sink, predecessors, found_path);
        // predecessors contains an edge pointing to it for each vertex, starting at the sink and ending at the source,
        // unless found_path = false.

        // if a path has been found, augment appropriately
        if (found_path) {
            // find the maximal rest capacity
            double augment = std::numeric_limits<double>::max();
            int node_id = sink;
            std::list<Edge_n*> path;
            while (true) {
                if (node_id == source) {
                    break;
                }
                path.push_back(predecessors[node_id]);
                if (augment > predecessors[node_id]->rest_capacity()) {
                    augment = predecessors[node_id]->rest_capacity();
                }
                node_id = predecessors[node_id]->from;
            }
            // for each edge, augment appropriately.
                for (auto & edge: path) {
                    if (!edge->reversed) {
                        edge->pump(augment);
                        edge->partner->pump(augment);
                    }
                    else {
                        edge->pump(-augment);
                        edge->partner->pump(-augment);
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
    G.add_edge(1,3,2,0);
    G.add_edge(1,4,1,0);
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

