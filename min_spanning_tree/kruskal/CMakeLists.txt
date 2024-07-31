cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE Debug)
project(kruskal)
add_executable(kruskal
	../../weighted_graph.cpp
	../../weighted_graph.h
	kruskal.cpp)
