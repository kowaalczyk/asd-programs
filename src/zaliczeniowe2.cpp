#include <iostream>
#include <vector>
#include <queue>
#include <cassert>

using namespace std;

/*
 * Alternative solution:
 *
 * 1) For each city in input create (k+1) nodes in graph (for each level of discounts used to reach the node)
 * 2) For each connection v->w for each level in 0..k create edge in the level and create an edge to higher level (representing more discounts used)
 * 3) Run Dijkstra on this graph to reach all nodes in every level with optimal cost
 * 4) Cots of last node is the first cost calculated for last city on any level
 *
 */

typedef int graph_id_t;
typedef int level_t;
typedef int cost_t;
typedef pair<cost_t, graph_id_t> connection_t; /// connection := <cost of connection, target node>

const graph_id_t MAX_NODES = 20000;
const int MAX_COUPONS = 10;

level_t graph_max_level = 0; /// # of discount levels available (0 := no discounts, k := 0..k discounts can be used)
graph_id_t graph_level_size = 0; /// size of each level
vector<connection_t> connections_from[MAX_NODES*(MAX_COUPONS+1)]; /// connections_from[n] := vector of connections from node n
priority_queue<connection_t, vector<connection_t>, greater<connection_t>> dijkstra_q; /// priority queue for Dijkstra algorithm
graph_id_t start_id = -1; /// id of starting node in Dijkstra algorithm
graph_id_t end_id = -1; /// id of finishing node in Dijkstra algorithm
bool visited[MAX_NODES*(MAX_COUPONS+1)]; /// flag for visited nodes

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
    start_id = 0;
    end_id = level_size-1;
    // reset variables
    for(int i=0; i < graph_level_size*(graph_max_level+1); i++) {
        visited[i] = false;
        assert(connections_from[i].empty());
    }
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
    assert(start_id == 0);
    assert(start_id >= 0);
    assert(dijkstra_q.empty());

    dijkstra_q.emplace(0, start_id);
    while(!dijkstra_q.empty()) {
        // visit node
        connection_t current_data = dijkstra_q.top(); // <total cost of connection from start_id to node_id, node_id>
        dijkstra_q.pop();
        cost_t current_cost = current_data.first;
        graph_id_t current_node = current_data.second;
        visited[current_node] = true;

        // last node was reached, we can stop here
        if(level_pos(current_node) == end_id) {
            return current_cost;
        }

        // add following nodes to queue
        for(connection_t connection : connections_from[current_node]) {
            graph_id_t following_node = connection.second;
            if(!visited[following_node]) {
                dijkstra_q.emplace(current_cost + connection.first, following_node);
            }
        }
    }
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
