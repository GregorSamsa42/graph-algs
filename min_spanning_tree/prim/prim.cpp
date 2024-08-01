//  Algorithm generating a minimum spanning tree using Prim's algorithm
//  Authors: Georǵi Kocharyan

#include <iostream>
#include <cstdio>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <string>
#include <functional>
#include "../../weighted_graph.h"


 struct key_compare
 {  
   bool operator()(const std::pair<int, double>& l, const std::pair<int, double>& r)  
   {  
       return l.second > r.second;  
   }  
 };

void next_edge(WeightedGraph const & G, std::vector<std::pair<int, double>> & min_neighbours, std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double>>, key_compare> & notMST, std::vector<bool> & inMST, int & last_added, double & total_weight) {
    // update the priority queue
    for (const auto& j: G.adjList(last_added))
    {
        int to = j.first;
        double weight = j.second;
        if (weight < min_neighbours[to].second)
        {
            min_neighbours[to] = std::make_pair(last_added, weight);
            notMST.push(std::make_pair(to, weight));
        }
    }
    // add the next edge
    std::pair<int, double> node = notMST.top();
    notMST.pop();
    if (!inMST[node.first])
    {
        std::cout << node.first << "-" << min_neighbours[node.first].first << "\t" << node.second << std::endl;
        last_added = node.first;
        inMST[node.first] = true;
        total_weight = total_weight + node.second;
    }
    
}

void prim(WeightedGraph const & G) {
    // preprocessing: remove all double edges except the minimal ones

    WeightedGraph H = G.remove_parallel();

    // preprocessing: create a vector with the min MST neighbour of all vertices

    std::vector<std::pair<int, double>> min_neighbours(H.num_nodes(), std::make_pair(0,std::numeric_limits<double>::infinity()));

    // preprocessing: track with a vector which elements are in the MST

    std::vector<bool> inMST(H.num_nodes(), false);
    inMST[0] = true;

    // preprocessing: create a priority queue of vertices not yet in the MST

    std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double>>, key_compare> notMST;
    for (int i = 1; i < H.num_nodes(); i++)
    {
        notMST.push(std::make_pair(i, std::numeric_limits<double>::infinity()));
    }
    int last_added = 0;
    double total_weight = 0;
    while (!notMST.empty()) 
    {
        next_edge(H, min_neighbours, notMST, inMST, last_added, total_weight);
    }
    std::cout << "The total weight of the MST is " << total_weight << std::endl;
}

int main() {

    int size = 8;
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