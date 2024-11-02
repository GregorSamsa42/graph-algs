//  Algorithm generating a minimum spanning tree using Prim's algorithm
// we use a directed graph class to model an undirected graph
//  Authors: Georǵi Kocharyan

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <functional>

#include "digraph.h"

using WeightedGraph = Digraph<WeightedEdge<double>>;
using Edge_w = WeightedEdge<double>;
using Node_w = Node<WeightedEdge<double>>;

class isLighter {
public:
    bool operator() (const Edge_w e1, const Edge_w e2) const
    {
        return e1.weight > e2.weight;
    }
};


void next_edge(WeightedGraph const & G, std::vector<Edge_w> & min_neighbours, std::priority_queue<Edge_w, std::vector<Edge_w>, isLighter> & notMST, std::vector<bool> & inMST, int & last_added, double & total_weight, int & current_edges) {
    // update the priority queue
    for (const auto& edge: G.adjList(last_added))
    {
    // if any of the neighbours of last_added is connected to it by a weight smaller than their current best MST connecting edge, replace their best edge.
        if (edge.weight < min_neighbours[edge.to].weight)
        {
            min_neighbours[edge.to] = edge; // we do not consider the orientation of edges in min_neighbours
            notMST.push(edge);
        }
    }
    // add the next edge
    const Edge_w edge = notMST.top();
    notMST.pop();
    // are guaranteed by construction that at least one of the endpoints of edge is in the MST.
    // if both of them are, nothing happens.
    if (!inMST[edge.to])
    {
        std::cout << edge.from << "-" << edge.to << "\t" << edge.weight << std::endl;
        last_added = edge.to;
        inMST[edge.to] = true;
        total_weight = total_weight + edge.weight;
        current_edges++;
    }
    else if (!inMST[edge.from])
    {
        std::cout << edge.from << "-" << edge.to << "\t" << edge.weight << std::endl;
        last_added = edge.from;
        inMST[edge.from] = true;
        total_weight = total_weight + edge.weight;
        current_edges++;
    }
    
}

void prim(WeightedGraph const & G) {
    // preprocessing: remove all double edges except the minimal ones, treat as G a undirected graph.

    WeightedGraph H(G.num_nodes());

    for (int i = 0; i < G.num_nodes(); i++) {
        for (const auto edge: G.adjList(i)) {
            H.add_edge(edge.to,edge.from, edge.weight);
            H.add_edge(edge.from,edge.to, edge.weight);
        }
    }

    H = H.remove_parallel_min();

    // preprocessing: create a vector with the min MST connecting edge of all vertices

    std::vector<Edge_w> min_neighbours;
    for (int i = 0; i < H.num_nodes(); i++) {
        min_neighbours.emplace_back(i,0,std::numeric_limits<double>::infinity());
    }
    // preprocessing: track with a vector which elements are in the MST

    std::vector<bool> inMST(H.num_nodes(), false);
    inMST[0] = true;

    // preprocessing: create a priority queue of eligible edges not yet in the MST

    std::priority_queue<Edge_w, std::vector<Edge_w>, isLighter> notMST;
    // for (int i = 1; i < H.num_nodes(); i++)
    // {
    //     notMST.emplace(i, std::numeric_limits<double>::infinity());
    // }
    int last_added = 0; // add 0
    double total_weight = 0;
    // add edges to tree by weight
    int current_edges_added = 0;
    while (current_edges_added < H.num_nodes()-1) // a spanning tree has n-1 edges
    {
        next_edge(H, min_neighbours, notMST, inMST, last_added, total_weight, current_edges_added);
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
