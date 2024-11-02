cmake_minimum_required(VERSION 3.29)
project(graph_algs)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(kosaraju
        digraph.h
        Kosaraju/kosaraju.cpp)

add_executable(euler_cycle
        digraph.h
        euler_cycle/euler_cycle.cpp)

add_executable(edmonds
        digraph.h
        min_spanning_tree/edmonds/edmonds.cpp)

add_executable(kruskal
        min_spanning_tree/kruskal/kruskal.cpp)

add_executable(prim
        min_spanning_tree/prim/prim.cpp)

add_executable(top_order
        digraph.h
        top_order/top_order.cpp)

add_executable(tree_isomorphism
        tree_isomorphism/tree_isomorphism.cpp)

add_executable(dijsktra
        digraph.h
        shortest_paths/dijkstra.cpp)

add_executable(dijsktra_radix
        shortest_paths/dijkstra_radix.cpp
        digraph.h)

add_executable(moore_bellman_ford
        digraph.h
        shortest_paths/moore_bellman_ford.cpp)

add_executable(floyd_warshall
        digraph.h
        shortest_paths/floyd_warshall.cpp)

add_executable(karp
        digraph.h
        minimum_mean_cycle/karp.cpp
        minimum_mean_cycle/kosaraju.h)

add_executable(ford_fulkerson max_flows/ford_fulkerson.cpp
        digraph.h)

add_executable(edmonds_karp max_flows/edmonds_karp.cpp
        digraph.h)

add_executable(tarjan bridge_finding/tarjan.cpp
        digraph.h)
