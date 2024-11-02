// Dijkstra's algorithm to find shortest paths in a directed weighted graph with nonnegative weights.
// Author: Georgi Kocharyan

#include <iostream>
#include <limits>
#include <ostream>
#include <queue>

#include "digraph.h"

using WeightedDigraph = Digraph<WeightedEdge<double>>;
using Edge_w = WeightedEdge<double>;

struct Vertex_with_predecessor
{
    int node_id;
    double distance;
    int predecessor_id;

    Vertex_with_predecessor(const int node, const double dist, const int pred_id) : node_id(node), distance(dist), predecessor_id(pred_id) {}
    // smallest weight should have highest priority
    bool operator<(Vertex_with_predecessor const & other) const {
        return distance > other.distance;
    }
};

void dijkstra(const WeightedDigraph & G, std::vector<double> & min_distances, const int measuring_from, std::vector<int> & predecessor)
{
    // initialise the priority queue
    min_distances[measuring_from] = 0;
    std::vector<bool> fixed(G.num_nodes(), false);
    std::priority_queue<Vertex_with_predecessor> pq;
    for (int i = 0; i < G.num_nodes(); ++i) {
        pq.emplace(i, min_distances[i], i);
    }
    // remove the root node
    // loop through the priority queue and update weights appropriately
    while (!pq.empty()) {
        const Vertex_with_predecessor v = pq.top();
        pq.pop();
        // if this vertex has already been popped once or is inaccessible, ignore
        if (fixed[v.node_id] || v.distance == std::numeric_limits<double>::max()) {
            continue;
        }
        // its weight is the minimum distance from the root node
        min_distances[v.node_id] = v.distance;
        predecessor[v.node_id] = v.predecessor_id;
        fixed[v.node_id] = true;
        // loop through all neighbours of v and add them in if their min distance hasn't been determined yet
        for (auto edge : (G.adjList(v.node_id))) {
            if (!fixed[edge.to]) {
                pq.emplace(edge.to, min_distances[v.node_id]+edge.weight, v.node_id);
            }
        }
    }
}

int main()
{
    constexpr int size = 8;
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

    std::vector<double> min_distances(size, std::numeric_limits<double>::max());
    constexpr int measuring_from = 0;
    std::vector<int> predecessor(size, measuring_from);

    dijkstra(G, min_distances, measuring_from, predecessor);

    // output isn't done very efficiently but in principle could be done better ( O(n^2+m) runtime applies to method dijkstra )

    std::cout << measuring_from << " is the root node." << std::endl;
    for (int i = 0; i < G.num_nodes(); ++i) {
        if (i == measuring_from) {
            continue;
        }
        std::cout << i;
        if (min_distances[i] == std::numeric_limits<double>::max()) {
            std::cout << " is inaccessible from " << measuring_from << "." << std::endl;
        }
        else {
            int pred = predecessor[i];
            while (pred != measuring_from) {
                std::cout << " - " << pred;
                pred = predecessor[pred];
            }
            std::cout << " - " << measuring_from << " is the shortest path with total weight " << min_distances[i] << "." << std::endl;
        }
    }

    return 0;
}
