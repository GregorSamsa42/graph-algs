// Implementation of Tarjan's bridge finding algorithm
// Author: Georgi Kocharyan

#include <iostream>
#include <vector>

#include "digraph.h"

using UndirectedGraph = Digraph<Edge>;

void dfs(UndirectedGraph const & G, const Edge edge, std::vector<bool> & vis, std::vector<int> & node_order, std::vector<int> & lowpoint, int & time, std::vector<Edge> & bridges) {

    vis[edge.to] = true;
    node_order[edge.to] = time;
    lowpoint[edge.to] = time;
    time++;
    // go through the DFS and compute lowpoint along the way
    // lowpoint(v) is the earliest time reached in the DFS among all neighbours of descendants of v
    // it turns out that if (v,w) is in the DFS, then it is a bridge iff lowpoint(w) > time(v).
    for(const auto i: G.adjList(edge.to))
    {
        if (vis[i.to])
        {
            if (i.to != edge.from) {
                lowpoint[edge.to] = std::min(lowpoint[edge.to], node_order[i.to]);
            }
            return;  //if node is already visited don't
        }
        dfs(G, i, vis, node_order, lowpoint, time, bridges);
        lowpoint[edge.to] = std::min(lowpoint[edge.to], lowpoint[i.to]);
        if (lowpoint[i.to] > node_order[edge.to]) {
            bridges.push_back(i);
        }
    }

}
std::vector<Edge> tarjan(const UndirectedGraph & G)
{
    int time = 0;
    std::vector<Edge> bridges;
    bridges.reserve(G.num_edges());
    std::vector<bool> vis(G.num_nodes(), false);
    std::vector<int> node_order(G.num_nodes());
    std::vector<int> lowpoint(G.num_nodes());
    const Edge fake_edge(-1,0);
    dfs(G,fake_edge,vis,node_order,lowpoint,time,bridges);
    return bridges;
}

int main() {
    constexpr int size = 8;
    UndirectedGraph G(size);
    G.add_edge(3, 4);
    G.add_edge(2, 3);
    G.add_edge(4,2);
    G.add_edge(1, 2);
    G.add_edge(0,1);

    G.double_edges();
    G = G.remove_parallel();

    for (std::vector<Edge> bridges = tarjan(G); const auto & edge : bridges) {
        std::cout << edge.from << " - " << edge.to << " is a bridge.\n";
    }

    return 0;

}
