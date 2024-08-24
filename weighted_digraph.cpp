#include "weighted_digraph.h"
#include <unordered_map>
#include <limits>

Node::Node(std::list<Edge> neighbours_) : neighbours(std::move(neighbours_))
{
    name = 0;
    marking = false;
}

Node::Node(std::list<Edge> neighbours_, int name) : neighbours(std::move(neighbours_)), name(name)
{
    marking = false;
}

void Node::add_edge(int from, int to, double weight)
{
    neighbours.emplace_back(from, to, weight);
}

int Node::deg() const
{
    return neighbours.size();
}

void Node::mark()
{
    marking = true;
}

WeightedDigraph::WeightedDigraph(size_t num_nodes) : nodes(num_nodes)
{
    edges = 0;
    std::vector<Edge> v;
    v.reserve(num_nodes);
    for (int i = 0; i < num_nodes; i++) {
        v.emplace_back(0, i, std::numeric_limits<double>::infinity());
    }
    mins = v;
}


void WeightedDigraph::add_edge(Edge edge)
{
    add_edge(edge.from, edge.to, edge.weight);
}

int WeightedDigraph::node_name(int node_id) const
{
    return nodes[node_id].name;
}

void WeightedDigraph::add_edge(int from, int to, double weight)
{
    nodes[from].add_edge(from, to, weight);
    edges++;
    // check if weight of newly added edge is smaller than the min, and update accordingly
    // the entries of min should not be interpreted as Edges
    if (mins[to].weight > weight) {
        Edge v(from, to, weight);
        mins[to] = v;
    }
}


std::list<Edge> WeightedDigraph::adjList(int node_id) const
{
    return (nodes[node_id]).neighbours;
}

int WeightedDigraph::deg(int node_id) const
{
    return nodes[node_id].deg();
}

size_t WeightedDigraph::num_nodes() const
{
    return nodes.size();
}

int WeightedDigraph::num_edges() const
{
    return edges;
}

WeightedDigraph WeightedDigraph::contract_set(std::vector<int> const & v, std::vector<Edge> & min_edges_in, std::vector<Edge> & min_edges_out) const
{
    // min_edge_in and min_edge_out store the exact edges going into and out of the contracted set, preventing loss of info for Edmonds
    WeightedDigraph digraph(num_nodes() - v.size() + 1);
    int count = 0;
    // marking is interpreted here as being within the contracted set: this is to minimise computation
    std::vector<bool> marking(num_nodes(), false);
    for (int i: v) {
        marking[i] = true;
    }
    // want to store which nodes in the new graph the old nodes are mapped to
    std::vector<int> mappedto(num_nodes());
    for (int i = 0; i < num_nodes(); i++) {
        if (!marking[i]) {
            digraph.nodes[count].name = i;
            mappedto[i] = count;
            count++;
        } else {
            // everything in the contracted set gets mapped to the final node.
            mappedto[i] = num_nodes() - v.size();
        }
    }
    // now copy over all edges, treating the contracted set as a single vertex
    for (int i = 0; i < num_nodes(); i++) {
        for (auto const & j: nodes[i].neighbours) {
            digraph.add_edge(mappedto[j.from], mappedto[j.to], j.weight);
            // ignore nodes from the contracted set to itself
            // if (marking[j.from] && marking[j.to]) {
            //     break;
            // }
            if (marking[j.from] && min_edges_out[j.to].weight > j.weight) {
                min_edges_out[j.to] = j;
            }
            if (marking[j.to] && min_edges_in[j.from].weight > j.weight) {
                min_edges_out[j.from] = j;
            }
        }
    }
    return digraph;
}

Edge WeightedDigraph::min_ingoing_edge(int node_id) const
{
    return mins[node_id];
}

bool WeightedDigraph::dfs(int v, std::vector<bool> &visited, std::vector<bool> &possible, std::vector<Edge> &cycle) const
{
    visited[v] = true;
    possible[v] = true;
    for (auto outgoing_edge : adjList(v)) {
        if(!visited[outgoing_edge.to]) {
            cycle.emplace_back(outgoing_edge);
            if (dfs(outgoing_edge.to, visited, possible, cycle)) {
                return true;
            }
        }
        else if (possible[outgoing_edge.to]) {
            cycle.emplace_back(outgoing_edge);
            // remove front of cycle vector -- there might be more efficient way to do this...
            for (int i = 0; i < cycle.size(); i++) {
                if (cycle[i].from == outgoing_edge.to) {
                    cycle.assign(cycle.begin() + i, cycle.end());
                    break;
                }
            }
            return true;
        }

    }
    possible[v] = false;
    cycle.pop_back();
    return false;

}

std::vector<Edge> WeightedDigraph::find_cycle() const
{
    std::vector<bool> visited(num_nodes(), false);
    std::vector<bool> possible(num_nodes(), false);
    std::vector<Edge> cycle;
    cycle.reserve(num_nodes());

    for (int i = 0; i < num_nodes(); i++) {
        if (dfs(i, visited, possible, cycle)) {
            return cycle;
        }
        cycle = {};
        std::fill(visited.begin(), visited.end(), false);
        std::fill(possible.begin(), possible.end(), false);
    }

    return {};
}

WeightedDigraph WeightedDigraph::modified_weights() const
{
    WeightedDigraph H(num_nodes());
    for (int i = 0; i < num_nodes(); i++) {
        for (auto const & outgoing_edge : adjList(i)) {
            H.add_edge(outgoing_edge.from, outgoing_edge.to, outgoing_edge.weight - mins[outgoing_edge.to].weight);
        }
    }
    return H;
}
