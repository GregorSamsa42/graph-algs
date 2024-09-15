#include "weighted_digraph_integral.h"
#include <unordered_map>
#include <limits>

Node::Node(std::list<Edge> neighbours_) : neighbours(std::move(neighbours_))
{
    name = 0;
}

Node::Node(std::list<Edge> neighbours_, const int name) : neighbours(std::move(neighbours_)), name(name)
{
}

void Node::add_edge(int from, int to, int weight)
{
    neighbours.emplace_back(from, to, weight);
}

void Node::give_name(const int new_name)
{
    name = new_name;
}

int Node::deg() const
{
    return neighbours.size();
}

WeightedDigraphIntegral::WeightedDigraphIntegral(const size_t num_nodes) : nodes(num_nodes)
{
    edges = 0;
    std::vector<Edge> v;
    v.reserve(num_nodes);
    for (int i = 0; i < num_nodes; i++) {
        v.emplace_back(0, i, std::numeric_limits<int>::infinity());
    }
    mins = v;
    max = std::numeric_limits<int>::min();
}


void WeightedDigraphIntegral::add_edge(const Edge edge)
{
    add_edge(edge.from, edge.to, edge.weight);
}

int WeightedDigraphIntegral::node_name(const int node_id) const
{
    return nodes[node_id].name;
}

void WeightedDigraphIntegral::add_edge(int from, int to, int weight)
{
    nodes[from].add_edge(from, to, weight);
    edges++;
    // check if weight of newly added edge is smaller than the min, and update accordingly
    // the entries of min should not be interpreted as Edges
    if (mins[to].weight > weight) {
        Edge v(from, to, weight);
        mins[to] = v;
    }
    if (weight > max) {
        max = weight;
    }
}


std::list<Edge> WeightedDigraphIntegral::adjList(int node_id) const
{
    return (nodes[node_id]).neighbours;
}

int WeightedDigraphIntegral::deg(int node_id) const
{
    return nodes[node_id].deg();
}

size_t WeightedDigraphIntegral::num_nodes() const
{
    return nodes.size();
}

int WeightedDigraphIntegral::num_edges() const
{
    return edges;
}

WeightedDigraphIntegral WeightedDigraphIntegral::contract_set(std::vector<int> const & v, std::vector<Edge> & min_edges_in, std::vector<Edge> & min_edges_out) const
{
    // min_edge_in and min_edge_out store the exact edges going into and out of the contracted set, preventing loss of info for Edmonds
    WeightedDigraphIntegral digraph(num_nodes() - v.size() + 1);
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
            digraph.name_node(count, i);
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
            // ignore nodes from the contracted set to itself
            // not that aesthetically pleasing but this breaks the find_cycle method if not done
            if (marking[j.from] && marking[j.to]) {
                continue;
            }
            if (marking[j.from] && min_edges_out[j.to].weight > j.weight) {
                min_edges_out[j.to] = j;
            }
            else if (marking[j.to] && min_edges_in[j.from].weight > j.weight) {
                min_edges_in[j.from] = j;
            }
            digraph.add_edge(mappedto[j.from], mappedto[j.to], j.weight);
        }
    }
    return digraph;
}

Edge WeightedDigraphIntegral::min_ingoing_edge(const int node_id) const
{
    return mins[node_id];
}

bool WeightedDigraphIntegral::dfs(const int v, std::vector<bool> &visited, std::vector<bool> &possible, std::vector<Edge> &cycle) const
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

std::vector<Edge> WeightedDigraphIntegral::find_cycle() const
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

WeightedDigraphIntegral WeightedDigraphIntegral::modified_weights() const
{
    WeightedDigraphIntegral H(num_nodes());
    for (int i = 0; i < num_nodes(); i++) {
        H.name_node(i, node_name(i));
        for (auto const & outgoing_edge : adjList(i)) {
            H.add_edge(outgoing_edge.from, outgoing_edge.to, outgoing_edge.weight - mins[outgoing_edge.to].weight);
        }
    }
    return H;
}

void WeightedDigraphIntegral::name_node(const int node_id, const int new_name)
{
    nodes[node_id].give_name(new_name);
}

int WeightedDigraphIntegral::node_name(const int node_id)
{
    return nodes[node_id].name;
}
