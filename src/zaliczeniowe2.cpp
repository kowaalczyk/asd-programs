#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cassert>

using namespace std;

// KOSZT_MAX = 100 MLD -> long long ?

/*
 * Chcemy dla każdego wierzchołka mieć dostępny
 * koszt dojścia do niego z wykorzystaniem x kuponów (0 <= x <= 10) <- lub (-1) jeśli się nie da
 * wtedy min koszt dojścia do n-1 to: minimum z tych kosztów różnych od (-1)
 *
 */

/*
 *
 * for(k : # possible coupons):
 *   - (INV) all reachable nodes have cost w/ (k-1)-discounts calculated
 *
 *   1. current node := front of Q
 *   - (INV) all nodes visited previously have cost w/ k-discounts calculated
 *
 *   2. current node cost w/ k discounts := min(its previous cost w/ k discounts, its q-top cost)
 *      - if node n has been calculated, BREAK
 *
 *   3. for following node:
 *      - Q-pushed cost of following node w/ k-discounts :=
 *          min(cost of current node w/ (k-1) discounts + cost of reaching next w/ discount,
 *              cost of current node w/ k-discounts + cost o reaching next w/o discount)
 *      - push <Q-pushed cost, following node> to Q
 *
 *   NOTES:
 *    - There is no decrease_key(Q) ==> need to break early (might have to implement own Q, but pre-process w/ BFS should suffice)
 *    - Cost of reaching nodes w/o discounts can be calculated separately to prevent arr[-1] errors
 *
 */

/*
 * TODO: Try alternative solution:
 *
 * 1) For each city in input create (k+1) nodes in graph (for each level of discounts used to reach the node)
 * 2) For each connection v->w for each level in 0..k create edge in the level and create an edge to higher level (representing more discounts used)
 * 3) Run Dijkstra on this graph to reach all nodes in every level with optimal cost
 * 4) Cots of last node in last level is the answer
 *
 */



typedef int graph_id_t;
typedef int cost_t;

const graph_id_t MAX_NODES = 20000;
const int MAX_COUPONS = 10;
const graph_id_t MAX_CONNECTIONS = 100000;
const cost_t NODE_UNREACHABLE = -1;

typedef tuple<graph_id_t, cost_t, cost_t> connection_t; /// := {to_node, cost, discount}
typedef pair<cost_t, graph_id_t> connection_dijkstra_t; /// := {connection_temporary_cost, connection_id} for use in Dijkstra queue

priority_queue<connection_dijkstra_t, vector<connection_dijkstra_t>, greater<connection_dijkstra_t>> dijkstra_q; /// priority queue for Dijkstra algorithm

/// all connection data
connection_t *connections() {
    static connection_t ans[MAX_CONNECTIONS];
    return ans;
}

/// connections_from[n] := vector of connection ids starting at node n
vector<graph_id_t> *connections_from() {
    static vector<graph_id_t> ans[MAX_NODES];
    return ans;
}

bool visited[MAX_NODES];
cost_t min_cost[MAX_COUPONS+1][MAX_NODES]; /// min_cost[k][n] := minimal cost to reach node n with k coupons

cost_t get_cost(const connection_t &connection) {
    return get<1>(connection);
}

cost_t get_discounted_cost(const connection_t &connection) {
    return get<1>(connection) - get<2>(connection);
}

graph_id_t get_destination(const connection_t &connection) {
    return get<0>(connection);
}

void load_connection(graph_id_t connection_id) {
    int from, target, discount, cost;
    cin >> from >> target >> discount >> cost;
    connection_t new_connection {target, cost, discount};
    swap(connections()[connection_id], new_connection);
    connections_from()[from].emplace_back(connection_id);
}

void calc_discounted_cost(priority_queue<connection_dijkstra_t, vector<connection_dijkstra_t>, greater<connection_dijkstra_t>> &dijkstra_q, graph_id_t start_node_id, graph_id_t end_node_id, int discounts) {
    assert(dijkstra_q.empty());

    dijkstra_q.emplace(0, start_node_id);
    while(!dijkstra_q.empty()) {
        // visit next node from queue
        auto current_node_data = dijkstra_q.top();
        dijkstra_q.pop();
        graph_id_t current_node_id = current_node_data.second;
        visited[current_node_id] = true;

        // get both cost variants for current_k
        cost_t current_node_previous_cost = min_cost[discounts][current_node_id];
        cost_t current_node_temp_cost = current_node_data.first;

        // calculate cost and update values
        cost_t current_node_calculated_cost;
        if(current_node_previous_cost == NODE_UNREACHABLE) {
            current_node_calculated_cost = current_node_temp_cost;
        } else {
            current_node_calculated_cost = min(current_node_previous_cost, current_node_temp_cost);
        }
        min_cost[discounts][current_node_id] = current_node_calculated_cost;

        // add following nodes to Dijkstra queue with calculated temporary cost
        for(graph_id_t following_connection_id : connections_from()[current_node_id]) {
            const connection_t &following_connection = connections()[following_connection_id];
            graph_id_t destination_node_id = get_destination(following_connection);
            if(!visited[destination_node_id]) {
                if(discounts>0) {
                    cost_t cost_variant[2];
                    cost_variant[0] = min_cost[discounts][current_node_id] + get_cost(following_connection);
                    cost_variant[1] = min_cost[discounts-1][current_node_id] + get_discounted_cost(following_connection);
                    dijkstra_q.emplace(min(cost_variant[0], cost_variant[1]), destination_node_id);
                } else {
                    dijkstra_q.emplace(current_node_calculated_cost + get_cost(following_connection), destination_node_id);
                }
            }
        }
    }
    // clear queue and visited for all nodes
    for (bool &i_visted : visited) {
        i_visted = false;
    }
    priority_queue<connection_dijkstra_t, vector<connection_dijkstra_t>, greater<connection_dijkstra_t>> empty;
    swap(dijkstra_q, empty);
}

int main() {
    // init arrays
    for(int i=0; i<MAX_NODES; i++) {
        for(int j=0; j<=MAX_COUPONS; j++) {
            min_cost[j][i] = NODE_UNREACHABLE;
        }
        visited[i] = false;
    }

    // load variables
    int n, m, k; // # of cities, # of connection_data, # of coupons
    cin >> n >> m >> k;
    const int first_node_id = 0;
    const int last_node_id = n-1;
    for(graph_id_t connection_id=0; connection_id<m; connection_id++) {
        load_connection(connection_id);
    }

    // calcaulate cost with 0 discounts
    calc_discounted_cost(dijkstra_q, first_node_id, last_node_id, 0);
    if(min_cost[0][last_node_id] == NODE_UNREACHABLE) {
        // last node is unreachable ==> there is no way to reach it in further iterations
        cout << -1 << endl;
        exit(0);
    }

    // if target node was reachable, calculate cost for more discounts
    for(int current_k = 1; current_k <= k; current_k++) {
        calc_discounted_cost(dijkstra_q, first_node_id, last_node_id, current_k);
    }
    cout << min_cost[k][last_node_id] << endl;
    return 0;
}
