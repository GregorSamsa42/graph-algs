cmake_minimum_required(VERSION 3.29)
project(graph_algs)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(kosaraju
        digraph.cpp
        digraph.h
        Kosaraju/kosaraju.cpp
        Kosaraju/kosaraju.h)

add_executable(euler_cycle
        digraph.cpp
        digraph.h
        euler_cycle/euler_cycle.cpp)

add_executable(edmonds
        weighted_digraph.cpp
        weighted_digraph.h
        min_spanning_tree/edmonds/edmonds.cpp)

add_executable(kruskal
        weighted_graph.cpp
        weighted_graph.h
        min_spanning_tree/kruskal/kruskal.cpp)

add_executable(prim
        weighted_graph.cpp
        weighted_graph.h
        min_spanning_tree/prim/prim.cpp)

add_executable(top_order
        digraph.cpp
        digraph.h
        top_order/top_order.cpp)

add_executable(tree_isomorphism
        tree_isomorphism/tree_isomorphism.cpp)

add_executable(dijsktra
        weighted_digraph.cpp
        weighted_digraph.h
        shortest_paths/dijkstra.cpp)

add_executable(dijsktra_radix
        shortest_paths/dijkstra_radix.cpp
        weighted_digraph_integral.cpp
        weighted_digraph_integral.h)

add_executable(moore_bellman_ford
        weighted_digraph.cpp
        weighted_digraph.h
        shortest_paths/moore_bellman_ford.cpp)

add_executable(floyd_warshall
        weighted_digraph.cpp
        weighted_digraph.h
        shortest_paths/floyd_warshall.cpp)

add_executable(karp
        weighted_digraph.cpp
        weighted_digraph.h
        digraph.cpp
        digraph.h
        minimum_mean_cycle/karp.cpp
        minimum_mean_cycle/kosaraju.cpp)

add_executable(ford_fulkerson max_flows/ford_fulkerson.cpp
        network.h
        network.cpp
        weighted_digraph.cpp
        weighted_digraph.h
        digraph.h
        digraph.cpp)
