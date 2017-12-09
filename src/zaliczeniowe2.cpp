#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <climits>

using namespace std;

/*
 * (c) Krzysztof Kowalczyk k.kowaalczyk@gmail.com
 *
 * Problem:
 * Find an optimal way of using k discounts to travel between city 0 and city (n-1),
 * with given graph of connections between cities 0..n-1 (each with cost and possible discount).
 *
 * Solution:
 * Given k possible discounts to be taken, we represent city as (k+1) nodes in a graph.
 * Each connection between cities A, B will be represented as:
 * - edge between nodes representing A and B on the same level, with weight equal to connection cost (1 edge per every level)
 *   (traversing this edge represents taking a route from A to B without discount)
 * - edge between node representing A on level l and node B on level l+1, with weight equal to discounted connection cost,
 *   for l in 0..k-1 (traversing this edge represents taking a discount on the trip from A to B)
 * Implementing a standard Dijkstra algorithm on such tree will efficiently calculate cost of reaching last city,
 * with time complexity of O((n*k+m)*log(n*k)), where m is a number of connections.
 *
 */

typedef int graph_id_t;
typedef int level_t;
typedef int cost_t;
typedef pair<cost_t, graph_id_t> connection_t; /// connection := <cost of connection, target node>

const cost_t COST_MAX = INT_MAX;
const graph_id_t MAX_NODES = 20000;
const int MAX_COUPONS = 10;

// graph representation
level_t graph_max_level = 0; /// # of discount levels available (0 := no discounts, k := 0..k discounts can be used)
graph_id_t graph_level_size = 0; /// size of each level
vector<connection_t> connections_from[MAX_NODES*(MAX_COUPONS+1)]; /// connections_from[n] := vector of connections from node n
graph_id_t start_node = -1; /// id of starting node in Dijkstra algorithm
graph_id_t end_node = -1; /// id of finishing node in Dijkstra algorithm
cost_t total_cost[MAX_NODES*(MAX_COUPONS+1)]; /// total_cost[n] := total cost of reaching node n from start_node

/// for a given city position, get its node within the graph on specified level
graph_id_t graph_pos(graph_id_t node_id, level_t level) {
    assert(graph_level_size > 0);
    return graph_level_size*level + node_id;
}

/// for a given graph node, get its city position (position with relation to its level)
graph_id_t level_pos(graph_id_t node_id) {
    assert(graph_level_size > 0);
    return node_id % graph_level_size;
}

/// set up variables necessary for graph traversal
void create_graph(graph_id_t level_size, level_t levels) {
    // set variables
    graph_max_level = levels;
    graph_level_size = level_size;
    start_node = 0;
    end_node = level_size-1;
}

/// read connection information from stdin and update graph data
void load_connection() {
    assert(graph_level_size > 0);

    graph_id_t from, target;
    cost_t discount, cost;
    cin >> from >> target >> discount >> cost;

    cost_t discounted_cost = cost - discount;
    for(level_t l = 0; l < graph_max_level; l++) {
        connections_from[graph_pos(from, l)].emplace_back(cost, graph_pos(target, l)); // connection w/o discount
        connections_from[graph_pos(from, l)].emplace_back(discounted_cost, graph_pos(target, l+1)); // connection w/ discount
    }
    connections_from[graph_pos(from, graph_max_level)].emplace_back(cost, graph_pos(target, graph_max_level)); // connection w/o discount at max level
}

/// search for optimal path from start_id to end_id in a graph using Dijkstra algorithm
cost_t dijkstra_search() {
    assert(graph_level_size > 0);
    assert(start_node == 0);
    assert(start_node >= 0);

    // set up
    priority_queue<connection_t, vector<connection_t>, greater<connection_t>> dijkstra_q;
    bool visited[graph_level_size*(graph_max_level+1)];
    for(int i=0; i < graph_level_size*(graph_max_level+1); i++) {
        visited[i] = false;
        total_cost[i] = COST_MAX;
    }
    total_cost[start_node] = 0;
    dijkstra_q.emplace(0, start_node);

    // Dijkstra algorithm
    while(!dijkstra_q.empty()) {
        // visit node
        connection_t current_data = dijkstra_q.top(); // <total cost of connection from start_node to node_id, node_id>
        dijkstra_q.pop();
        cost_t current_cost = current_data.first;
        graph_id_t current_node = current_data.second;
        visited[current_node] = true;

        // visiting a node corresponding to last city (no matter on which level) means that it was an optimal route
        if(level_pos(current_node) == end_node) {
            return current_cost;
        }

        // add following nodes to queue
        for(connection_t connection : connections_from[current_node]) {
            graph_id_t following_node = connection.second;
            cost_t following_cost = connection.first;
            if(!visited[following_node] && current_cost + following_cost < total_cost[following_node]) {
                // optimized adding: following node will not be added to queue if it doesn't reduce cost
                total_cost[following_node] = current_cost + following_cost;
                dijkstra_q.emplace(current_cost + following_cost, following_node);
            }
        }
    }

    // last_node was not reached - function would have returned from loop if it did
    return -1;
}

int main() {
    int n, m, k; // # of cities, # of connections, # of coupons
    cin >> n >> m >> k;
    create_graph(n, k);
    for(int i=0; i<m; i++) {
        load_connection();
    }
    cout << dijkstra_search() << endl;
    return 0;
}
