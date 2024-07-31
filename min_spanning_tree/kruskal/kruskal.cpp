//  Algorithm generating a minimum spanning tree using Kruskal's algorithm
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
#include <tuple>


 struct key_compare
 {  
   bool operator()(const std::tuple<int, int, double>& l, const std::tuple<int, int, double>& r)  
   {  
       return std::get<2>(l) < std::get<2>(r);  
   }  
 };

void next_edge(WeightedGraph const & G, std::vector<std::list<int>> & elements, std::vector<int> component) {
    
}

void kruskal(WeightedGraph const & G) {
    // preprocessing: remove all double edges except the minimal ones

    WeightedGraph H = G.remove_parallel();
    double total_weight = 0;

    // preprocessing: create a vector of lists tracking the elements of the components

    std::vector<std::list<int>> elements;
    elements.resize(H.num_nodes());
    for (int i = 0; i < H.num_nodes(); i++)
    {
        elements[i].push_back(i);
    }

    // preprocessing: create a vector tracking which component each element belongs to

    std::vector<int> component;
    component.resize(H.num_nodes());
    for (int i = 0; i < H.num_nodes(); i++)
    {
        component[i] = i;
    }

    // preprocessing: create a vector containing all edges in O(m)
    // then sort them according to their weight

    std::vector<std::tuple<int, int, double>> edges;
    edges.resize(H.num_edges());
    int count = 0;
    for (int i = 0; i < H.num_nodes(); i++)
    {
        for (const auto& j : H.adjList(i))
        {
            edges[count] = std::make_tuple(i,j.first,j.second);
            count++;
        }
    }
    std::sort(edges.begin(), edges.end(),key_compare());

    for (const auto& edge : edges)
    {
        // check if edge connects two of the same component
        if (!(component[std::get<0>(edge)]==component[std::get<1>(edge)]))
        {
            // output edge
            std::cout << std::get<0>(edge) << "-" << std::get<1>(edge) << "\t" << std::get<2>(edge) << std::endl;
            total_weight = total_weight + std::get<2>(edge);
            // make the components of each node the same
            // the larger component absorbs the second to guarantee O(mlogn) runtime
            if (elements[component[std::get<0>(edge)]].size() >= elements[component[std::get<1>(edge)]].size())
            {
                // move all elements of second component to first
                int moved = component[std::get<1>(edge)];
                int movedto = component[std::get<0>(edge)];
                for (const auto& node : (elements[moved]))
                {
                    elements[movedto].push_back(node);
                    component[node] = movedto;
                }
                elements[moved].clear();
            }
            else
            {
                // move all elements of first component to second
                int moved = component[std::get<0>(edge)];
                int movedto = component[std::get<1>(edge)];
                for (const auto& node : (elements[moved]))
                {
                    elements[movedto].push_back(node);
                    component[node] = movedto;
                }
                elements[moved].clear();
            }
        }

    }
    std::cout << "The total weight of the MST is " << total_weight << std::endl;
}

// example of a CONNECTED input graph

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

    kruskal(G);
    
    return 0;

}