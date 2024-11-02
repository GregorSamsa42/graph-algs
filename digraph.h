// Implementation of a digraph class with support for weighted edges and for network flows
// Author: Georgi Kocharyan

#ifndef C___DIGRAPH_H
#define C___DIGRAPH_H

#include <list>
#include <vector>
#include <concepts>
#include <algorithm>
#include <limits>


struct Edge
{
    using weight_type = bool;
    int from;
    int to;
    Edge(int from_id, int to_id) : from(from_id), to(to_id)
    {
    }
};

template<typename weight_t>
struct WeightedEdge : public Edge
{
    using weight_type = weight_t;
    weight_type weight;
    WeightedEdge(int from_id, int to_id) : Edge(from_id, to_id), weight(0)
    {
    }
    WeightedEdge(int from_id, int to_id, weight_type wgt) : Edge(from_id,to_id), weight(wgt)
    {
    }

    bool operator<(WeightedEdge const &other) const
    {
        return weight < other.weight;
    }
};

template<typename weight_t>
struct NetworkEdge : Edge
{
    using weight_type = weight_t;
    weight_t capacity;
    weight_t flow;
    bool marking;
    bool reversed;
    NetworkEdge* partner; // in residual graphs the reverse edge representing reducing flow
    // NetworkEdge(Edge edge); // flow always initialised to zero
    // NetworkEdge(Edge edge, NetworkEdge* represents);
    NetworkEdge(int from, int to, double capacity, double flow) : Edge(from,to), capacity(capacity), marking(false), reversed(false), flow(flow) {};
    void pump(double additional_flow);
    double rest_capacity() const; // determines how much can still be pumped through
    void mark();
    void unmark();
};

template<typename E>
concept IsWeighted = requires (E e)
{
    {e.weight};
};

template<typename E>
concept HasMarking = requires (E e)
{
    {e.marking};
};

template<typename E>
concept HasFlow = requires (E e)
{
    {e.flow};
    {e.capacity};
};

template<typename edge_type>
struct Node
{
    using weight_type = typename edge_type::weight_type;

    std::list<edge_type> neighbours;

    int name;

    Node() = default;

    explicit Node(std::list<int> neighbours_) : neighbours(std::move(neighbours_)), name(0) {}

    Node(std::list<Edge> neighbours_, int name_) : neighbours(std::move(neighbours_)), name(name_) {}

    void add_edge(int from, int to, weight_type weight) requires IsWeighted<edge_type>;

    void add_edge(int from, int to);

    void add_edge(edge_type edge);

    int outdeg() const;

    void give_name(int new_name);
};

template<typename edge_type>
class Digraph
{
    using weight_type = typename edge_type::weight_type;

public:
    template<typename U = edge_type>
    explicit Digraph(std::enable_if_t<IsWeighted<U>, size_t> num_nodes) : nodes(num_nodes)
    {
        edges = 0;
        std::vector<edge_type> v;
        v.reserve(num_nodes);
        for (int i = 0; i < num_nodes; i++) {
            v.emplace_back(0, i, std::numeric_limits<weight_type>::max());
        }
        mins = v;
        max = std::numeric_limits<weight_type>::min();
    }

    template<typename U = edge_type>
    explicit Digraph(std::enable_if_t<(!IsWeighted<U>), size_t> num_nodes) : nodes(num_nodes)
    {
        edges = 0;
        max = 0;
    }

    void add_edge(int from, int to, weight_type weight) requires IsWeighted<edge_type>;

    void add_edge(int from, int to) requires (!IsWeighted<edge_type>);

    void add_edge(int from, int to, weight_type capacity, weight_type flow) requires (HasFlow<edge_type> && !IsWeighted<edge_type>);

    void add_edge(edge_type edge);

    int node_name(int node_id) const;

    std::list<edge_type> adjList(int node_id) const;

    std::list<edge_type> &adjList_ref(int node_id);

    int num_edges() const;

    edge_type min_ingoing_edge(int node_id) const requires IsWeighted<edge_type>;

    Digraph contract_set(std::vector<int> const & v, std::vector<edge_type> & min_edges_in, std::vector<edge_type> & min_edges_out) const;

    size_t num_nodes() const;

    Digraph transpose() const;

    std::vector<edge_type> find_cycle() const;

    Digraph modified_weights() const requires IsWeighted<edge_type>;

    void name_node(int node_id, int new_name);

    int node_name(int node_id);

    Digraph<Edge> lose_weight() const requires IsWeighted<edge_type>;

    Digraph remove_parallel() const requires IsWeighted<edge_type>;

    bool isEdge(int from, int to) const;

    int outdeg(int node_id) const;

    weight_type get_max() const;

    std::vector<int> indegrees() const;

    edge_type pop_edge(int node_id);

    void mark() requires HasMarking<edge_type>;

    void unmark() requires HasMarking<edge_type>;

private:
    std::vector<Node<edge_type>> nodes;
    int edges;
    int max;
    std::vector<edge_type> mins;
    bool dfs(int v, std::vector<bool> & visited, std::vector<bool> & possible, std::vector<edge_type> & cycle) const;
};

template<typename weight_t>
void NetworkEdge<weight_t>::pump(double additional_flow)
{
    flow += additional_flow;
}

template<typename weight_t>
double NetworkEdge<weight_t>::rest_capacity() const
{
    if (reversed) {
        return flow;
    }
    return capacity - flow;
}

template<typename weight_t>
void NetworkEdge<weight_t>::mark()
{
    marking = true;
}

template<typename weight_t>
void NetworkEdge<weight_t>::unmark()
{
    marking = false;
}

template<typename edge_type>
void Node<edge_type>::add_edge(int from, int to, weight_type weight) requires IsWeighted<edge_type>
{
    neighbours.emplace_back(from, to, weight);
}

template<typename edge_type>
void Node<edge_type>::add_edge(int from, int to)
{
    neighbours.emplace_back(from, to);
}

template<typename edge_type>
void Node<edge_type>::add_edge(edge_type edge)
{
    neighbours.push_back(edge);
}

template<typename edge_type>
int Node<edge_type>::outdeg() const
{
    return neighbours.size();
}

template<typename edge_type>
void Node<edge_type>::give_name(const int new_name)
{
    name = new_name;
}

template<typename edge_type>
void Digraph<edge_type>::add_edge(int from, int to, weight_type weight) requires IsWeighted<edge_type>
{
    nodes[from].add_edge(from, to, weight);
    edges++;
    // check if weight of newly added edge is smaller than the min, and update accordingly
    // the entries of min should not be interpreted as Edges
    if (mins[to].weight > weight) {
        edge_type v(from, to, weight);
        mins[to] = v;
    }
    if (weight > max) {
        max = weight;
    }
}

template<typename edge_type>
void Digraph<edge_type>::add_edge(int from, int to) requires (!IsWeighted<edge_type>)
{
    nodes[from].add_edge(from, to);
    edges++;
}

template<typename edge_type>
void Digraph<edge_type>::add_edge(int from, int to, weight_type capacity, weight_type flow) requires (HasFlow<edge_type> && !IsWeighted<edge_type>)
{
    nodes[from].neighbours.emplace_back(from, to, capacity, flow);
    edges++;
}

template<typename edge_type>
void Digraph<edge_type>::add_edge(edge_type edge)
{
    nodes[edge.from].add_edge(edge);
    edges++;
}

template<typename edge_type>
int Digraph<edge_type>::node_name(int node_id) const
{
    return nodes[node_id].name;
}

template<typename edge_type>
std::list<edge_type> Digraph<edge_type>::adjList(int node_id) const
{
    return (nodes[node_id]).neighbours;
}

template<typename edge_type>
std::list<edge_type>& Digraph<edge_type>::adjList_ref(int node_id)
{
    return ((nodes[node_id]).neighbours);
}

template<typename edge_type>
int Digraph<edge_type>::num_edges() const
{
    return edges;
}

template<typename edge_type>
edge_type Digraph<edge_type>::min_ingoing_edge(int node_id) const requires IsWeighted<edge_type>
{
    return mins[node_id];
}

template<typename edge_type>
Digraph<edge_type> Digraph<edge_type>::contract_set(std::vector<int> const &v, std::vector<edge_type> &min_edges_in,
    std::vector<edge_type> &min_edges_out) const
{
    // min_edge_in and min_edge_out store the exact edges going into and out of the contracted set, preventing loss of info for Edmonds
    Digraph<edge_type> digraph(num_nodes() - v.size() + 1);
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
            // ignore edges from the contracted set to itself
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

template<typename edge_type>
size_t Digraph<edge_type>::num_nodes() const
{
    return nodes.size();
}

template<typename edge_type>
Digraph<edge_type> Digraph<edge_type>::transpose() const
{
    Digraph G(num_nodes());
    for (int i = 0; i < num_nodes(); i++) {
        for (auto const &j: ((nodes[i]).neighbours)) {
            G.add_edge(j.to, j.from);
        }
    }
    return G;
}

template<typename edge_type>
std::vector<edge_type> Digraph<edge_type>::find_cycle() const
{
    std::vector<bool> visited(num_nodes(), false);
    std::vector<bool> possible(num_nodes(), false);
    std::vector<edge_type> cycle;
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

template<typename edge_type>
bool Digraph<edge_type>::dfs(int v, std::vector<bool> &visited, std::vector<bool> &possible,
    std::vector<edge_type> &cycle) const
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


template<typename edge_type>
Digraph<edge_type> Digraph<edge_type>::modified_weights() const requires IsWeighted<edge_type>
{
    Digraph H(num_nodes());
    for (int i = 0; i < num_nodes(); i++) {
        H.name_node(i, node_name(i));
        for (auto const & outgoing_edge : adjList(i)) {
            H.add_edge(outgoing_edge.from, outgoing_edge.to, outgoing_edge.weight - mins[outgoing_edge.to].weight);
        }
    }
    return H;
}

template<typename edge_type>
void Digraph<edge_type>::name_node(int node_id, int new_name)
{
    nodes[node_id].give_name(new_name);
}

template<typename edge_type>
int Digraph<edge_type>::node_name(int node_id)
{
    return nodes[node_id].name;
}

template<typename edge_type>
Digraph<Edge> Digraph<edge_type>::lose_weight() const requires IsWeighted<edge_type>
{
    Digraph<Edge> H(num_nodes());
    for (int i = 0; i < num_nodes(); i++) {
        for (auto const & edge : adjList(i)) {
            H.add_edge(edge.from, edge.to);
        }
    }
    return H;
}

template<typename edge_type>
Digraph<edge_type> Digraph<edge_type>::remove_parallel() const requires IsWeighted<edge_type>
{
    Digraph H(num_nodes());
    for (int i = 0; i < num_nodes(); i++) {
        std::vector<double> weights(num_nodes(),std::numeric_limits<weight_type>::max()); //store minimum weight pointing from i to every other node
        for (auto edge : adjList(i)) {
            if (weights[edge.to] > edge.weight) {
                weights[edge.to] = edge.weight;
            }
        }
        for (int j = 0; j < num_nodes(); j++) {
            if (weights[j] < std::numeric_limits<weight_type>::max()) {
                H.add_edge(i, j, weights[j]);
            }
        }
    }
    return H;
}

template<typename edge_type>
bool Digraph<edge_type>::isEdge(int from, int to) const
{
    return (std::find(adjList(from).begin(), adjList(from).end(), to) != adjList(from).end());
}

template<typename edge_type>
int Digraph<edge_type>::outdeg(int node_id) const
{
    return nodes[node_id].outdeg();
}

template<typename edge_type>
typename Digraph<edge_type>::weight_type Digraph<edge_type>::get_max() const
{
    return max;
}


template<typename edge_type>
std::vector<int> Digraph<edge_type>::indegrees() const
{
    std::vector<int> indegs(num_nodes(), 0);
    for (int i = 0; i < num_nodes(); i++) {
        for (auto const &j: ((nodes[i]).neighbours)) {
            indegs[j]++;
        }
    }
    return indegs;
}

template<typename edge_type>
edge_type Digraph<edge_type>::pop_edge(int node_id)
{
    const edge_type result = adjList(node_id).front();
    (nodes[node_id].neighbours).pop_front();
    return result;
}

template<typename edge_type>
void Digraph<edge_type>::mark() requires HasMarking<edge_type>
{
    for (int i = 0; i < num_nodes(); i++) {
        for (auto const & edge : adjList(i)) {
            edge.mark();
        }
    }
}

template<typename edge_type>
void Digraph<edge_type>::unmark() requires HasMarking<edge_type>
{
    for (int i = 0; i < num_nodes(); i++) {
        for (auto & edge : adjList(i)) {
            edge.unmark();
        }
    }
}
#endif //C___DIGRAPH_H
