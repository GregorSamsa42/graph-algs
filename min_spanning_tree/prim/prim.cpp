//  Algorithm generating a minimum spanning tree using Prim's algorithm
//  Authors: Georǵi Kocharyan

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <functional>

#include "weighted_graph.h"

struct Vertex
{
  int node_id;
  double distance;
  
  Vertex(const int node, const double dist) : node_id(node), distance(dist) {}
  // smallest weight should have highest priority
  bool operator<(Vertex const & other) const {
    return distance > other.distance;
  }
};

void next_edge(WeightedGraph const & G, std::vector<Vertex> & min_neighbours, std::priority_queue<Vertex> & notMST, std::vector<bool> & inMST, int & last_added, double & total_weight) {
    // update the priority queue
    for (const auto& j: G.adjList(last_added))
    {
        int to = j.first;
        double weight = j.second;
        if (weight < min_neighbours[to].distance)
        {
            min_neighbours[to] = Vertex(last_added, weight);
            notMST.emplace(to, weight);
        }
    }
    // add the next edge
    const Vertex node = notMST.top();
    notMST.pop();
    if (!inMST[node.node_id])
    {
        std::cout << node.node_id << "-" << min_neighbours[node.node_id].node_id << "\t" << node.distance << std::endl;
        last_added = node.node_id;
        inMST[node.node_id] = true;
        total_weight = total_weight + node.distance;
    }
    
}

void prim(WeightedGraph const & G) {
    // preprocessing: remove all double edges except the minimal ones

    const WeightedGraph H = G.remove_parallel();

    // preprocessing: create a vector with the min MST neighbour of all vertices

    std::vector<Vertex> min_neighbours;
    for (int i = 0; i < H.num_nodes(); i++) {
        min_neighbours.emplace_back(0,std::numeric_limits<double>::infinity());
    }
    // preprocessing: track with a vector which elements are in the MST

    std::vector<bool> inMST(H.num_nodes(), false);
    inMST[0] = true;

    // preprocessing: create a priority queue of vertices not yet in the MST

    std::priority_queue<Vertex> notMST;
    for (int i = 1; i < H.num_nodes(); i++)
    {
        notMST.emplace(i, std::numeric_limits<double>::infinity());
    }
    int last_added = 0;
    double total_weight = 0;
    // add edges to tree by weight
    while (!notMST.empty()) 
    {
        next_edge(H, min_neighbours, notMST, inMST, last_added, total_weight);
    }
    std::cout << "The total weight of the MST is " << total_weight << std::endl;
}

int main() {
    const int size = 8;
    WeightedGraph G(size);
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

    prim(G);
    
    return 0;

}
