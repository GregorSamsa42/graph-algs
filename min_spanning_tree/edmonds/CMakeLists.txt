cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE Debug)
project(edmonds)
add_executable(edmonds
	../../weighted_digraph.cpp
	../../weighted_digraph.h
	edmonds.cpp)
