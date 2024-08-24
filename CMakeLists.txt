cmake_minimum_required(VERSION 3.29)
project(graph_algs)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(kosaraju
        digraph.cpp
        digraph.h
        Kosaraju/main.cpp)

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

