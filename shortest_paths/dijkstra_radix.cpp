// Dijkstra's algorithm to find shortest paths in a directed weighted graph with nonnegative weights.
// implemented with a radix heap in the case of integral weights bounded by max.
// Author: Georgi Kocharyan


#include <cmath>
#include <complex>
#include <iostream>
#include <limits>
#include <ostream>
#include <queue>

#include "weighted_digraph_integral.h"

struct Vertex_with_predecessor
{
    int node_id;
    int distance;
    int predecessor_id;

    Vertex_with_predecessor(const int node, const int dist, const int pred_id) : node_id(node), distance(dist), predecessor_id(pred_id) {}
    // smallest weight should have highest priority
    bool operator<(Vertex_with_predecessor const & other) const {
        return distance > other.distance;
    }
};

bool radix_heap_empty(const std::vector<std::priority_queue<Vertex_with_predecessor>> & heap)
{
    for (const auto & i : heap) {
        if (!i.empty()) {
            return false;
        }
    }
    return true;
}

Vertex_with_predecessor radix_pop(std::vector<std::priority_queue<Vertex_with_predecessor>> & heap, std::vector<int> & boundaries)
{
    for (int j = 0; j < heap.size(); j++) {
        if (!heap[j].empty()) {
            const Vertex_with_predecessor v = heap[j].top();
            heap[j].pop();
            if (j == 0) {
                return v;
            }
            // change all boundaries and insert the elements of bucket i into the buckets 1,...,i-1 accordingly.
            boundaries[0] = v.distance-1;
            if (j != 1) {
                boundaries[1] = v.distance;
            }
            for (int i = 2; i <= j; i++) {
                boundaries[i] = std::min(boundaries[i-1]+(1 << (i-2)), boundaries[j+1]);
            }
            // insert elements into lower buckets
            while (!heap[j].empty()) {
                Vertex_with_predecessor redistributed_vertex = heap[j].top();
                heap[j].pop();
                for (int k = 1; k <= j; k++) {
                    if (redistributed_vertex.distance < boundaries[k]) {
                        heap[k-1].push(redistributed_vertex);
                    }
                }
            }
            return v;
        }
    }
    return Vertex_with_predecessor(0, -1, -1);
}

void radix_insert(std::vector<std::priority_queue<Vertex_with_predecessor>> & heap, std::vector<int> & boundaries, const int node_id, const int distance, const int pred_id)
{
    for (int j = 1; j <= heap.size(); j++) {
        if (distance <= boundaries[j]) {
            heap[j-1].emplace(node_id, distance, pred_id);
            break;
        }
    }
}

void dijkstra_radix(const WeightedDigraphIntegral & G, std::vector<int> & min_distances, const int measuring_from, std::vector<int> & predecessor)
{
    // initialise the radix heap
    min_distances[measuring_from] = 0;
    std::vector<bool> fixed(G.num_nodes(), false);
    const int B = ceil(log(G.max) / log(2))+1;
    std::vector<std::priority_queue<Vertex_with_predecessor>> buckets(B);
    std::vector<int> boundaries(B+1);
    boundaries[0] = -1;
    boundaries[1] = 0;
    for (int i = 2; i <= B; i++) {
        boundaries[i] = boundaries[i-1] + (1 << (i-2));
    }
    buckets[0].emplace(measuring_from, 0, 0);
    // run through the radix heap until it is empty
    while(!radix_heap_empty(buckets)) {
        const Vertex_with_predecessor v = radix_pop(buckets, boundaries);
        if (fixed[v.node_id]) {
            continue;
        }
        // its weight is the minimum distance from the root node
        min_distances[v.node_id] = v.distance;
        predecessor[v.node_id] = v.predecessor_id;
        fixed[v.node_id] = true;
        // loop through all neighbours of v and add them in if their min distance hasn't been determined yet
        for (auto edge : (G.adjList(v.node_id))) {
            if (!fixed[edge.to]) {
                radix_insert(buckets, boundaries, edge.to, min_distances[v.node_id]+edge.weight, v.node_id);
            }
        }
    }
}

int main()
{
    constexpr int size = 8;
    WeightedDigraphIntegral G(size);
    G.add_edge(3,4,2);
    G.add_edge(4,3,3);
    G.add_edge(5,6,6);
    G.add_edge(6,7,1);
    G.add_edge(1,2,3);
    G.add_edge(2,3,8);
    G.add_edge(7,5,5);
    G.add_edge(7,3,9);
    G.add_edge(0,3,1);
    G.add_edge(3,0,5);
    G.add_edge(4,6,3);
    G.add_edge(0,7,10);
    G.add_edge(4,2,1);

    std::vector<int> min_distances(size, std::numeric_limits<int>::max());
    constexpr int measuring_from = 0;
    std::vector<int> predecessor(size, measuring_from);

    // max weight is now 10 and all weights are integers

    dijkstra_radix(G, min_distances, measuring_from, predecessor);

    // output isn't done very efficiently but in principle could be done better ( O(n^2+m) runtime applies to method dijkstra )

    std::cout << measuring_from << " is the root node." << std::endl;
    std::cout << "The maximal edge weight is " << G.max << "." << std::endl;
    for (int i = 0; i < G.num_nodes(); ++i) {
        if (i == measuring_from) {
            continue;
        }
        std::cout << i;
        if (min_distances[i] == std::numeric_limits<int>::max()) {
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