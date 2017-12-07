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
 * TODO - Rewrite w/ Dijkstra:
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

const int MAX_NODES = 20000;
const int MAX_COUPONS = 10;
const int MAX_CONNECTIONS = 1000000;
const int NODE_UNREACHABLE = -1;

typedef int graph_id_t;
typedef long long int cost_t;

typedef tuple<graph_id_t, graph_id_t, graph_id_t, cost_t, cost_t> connection_t; /// := {connection_id, from_node, to_node, cost, discount}
typedef pair<cost_t, graph_id_t> connection_dijkstra_t; /// := {connection_temporary_cost, connection_id} for use in Dijkstra queue

//connection_t connections[MAX_CONNECTIONS];
//vector<graph_id_t> connections_from[MAX_NODES]; /// G[i] := vector of connection ids starting from i
priority_queue<connection_dijkstra_t, vector<connection_dijkstra_t>, greater<connection_dijkstra_t>> dijkstra_q; /// priority queue for Dijkstra algorithm

connection_t *connections() {
    static connection_t ans[MAX_CONNECTIONS];
    return ans;
}

vector<graph_id_t> *connections_from() {
    static vector<graph_id_t> ans[MAX_NODES];
    return ans;
}

cost_t min_cost[MAX_COUPONS+1][MAX_NODES]; /// min_cost[k][n] := minimal cost to reach node n with k coupons

cost_t get_cost(const connection_t &connection) {
    return get<3>(connection);
}

cost_t get_discounted_cost(const connection_t &connection) {
    return get<3>(connection) - get<4>(connection);
}

graph_id_t get_destination(const connection_t &connection) {
    return get<2>(connection);
}

void load_connection(graph_id_t connection_id) {
    int v, w, b, c;
    cin >> v >> w >> b >> c;
    connection_t new_connection {connection_id, v, w, b, c};
    connections()[connection_id] = new_connection;
    connections_from()[v].emplace_back(connection_id);
}

int main() {
//    ios_base::sync_with_stdio(false); // TODO: make sure this does not fuck up everything in functions above

    // init arrays
    for(int i=0; i<MAX_NODES; i++) {
        for(int j=0; j<=MAX_COUPONS; j++) {
            min_cost[j][i] = NODE_UNREACHABLE;
        }
    }

    // load variables
    int n, m, k; // # of cities, # of connection_data, # of coupons
    cin >> n >> m >> k;
    const int first_node_id = 0;
    const int last_node_id = n-1;
    for(graph_id_t connection_id=0; connection_id<m; connection_id++) {
        load_connection(connection_id);
    }

    // standard Dijkstra for calculating cost with 0 discount && checking whether last_node is reachable
    dijkstra_q.emplace(0, first_node_id);
    while(!dijkstra_q.empty()) {
        auto current_node_data = dijkstra_q.top();
        dijkstra_q.pop();
        // get both cost variants
        cost_t current_node_previous_cost = min_cost[0][current_node_data.second];
        cost_t current_node_temp_cost = current_node_data.first;
        // calculate actual cost and update values
        cost_t current_node_calculated_cost;
        if(current_node_previous_cost == NODE_UNREACHABLE) {
            current_node_calculated_cost = current_node_temp_cost;
        } else {
            current_node_calculated_cost = min(current_node_previous_cost, current_node_temp_cost);
        }
        min_cost[0][current_node_data.second] = current_node_calculated_cost;
        // add following nodes to Dijkstra queue with calculated temporary cost
        for(graph_id_t following_connection_id : connections_from()[current_node_data.second]) {
            const connection_t &following_connection = connections()[following_connection_id];
            if(get_destination(following_connection) != current_node_data.second) {
                dijkstra_q.emplace(current_node_calculated_cost + get_cost(following_connection), get_destination(following_connection));
            }
        }
    }

    if(min_cost[0][last_node_id] == NODE_UNREACHABLE) {
        // node is unreachable after standard Dijkstra ==> there is no way to reach it in further iterations
        cout << -1 << endl;
        exit(0);
    }

    // TODO: 2nd Dijkstra for calculating other costs
    assert(dijkstra_q.empty());
    dijkstra_q.emplace(0, first_node_id);
    for(int current_k = 1; current_k <= k; current_k++) {
        auto current_node_data = dijkstra_q.top();
        dijkstra_q.pop();
        graph_id_t current_node_id = current_node_data.second;
        // get both cost variants for current_k
        cost_t current_node_previous_cost = min_cost[current_k][current_node_id];
        cost_t current_node_temp_cost = current_node_data.first;
        // calculate cost and update values
        cost_t current_node_calculated_cost;
        if(current_node_previous_cost == NODE_UNREACHABLE) {
            current_node_calculated_cost = current_node_temp_cost;
        } else {
            current_node_calculated_cost = min(current_node_previous_cost, current_node_temp_cost);
        }
        min_cost[current_k][current_node_id] = current_node_calculated_cost;
        // add following nodes to Dijkstra queue with calculated temporary cost
        for(graph_id_t following_connection_id : connections_from()[current_node_id]) {
            const connection_t &following_connection = connections()[following_connection_id];
            if(get_destination(following_connection) != current_node_id) {
                cost_t cost_variant[2];
                cost_variant[0] = min_cost[current_k][current_node_id] + get_cost(following_connection);
                cost_variant[1] = min_cost[current_k-1][current_node_id] + get_discounted_cost(following_connection);
                dijkstra_q.emplace(min(cost_variant[0], cost_variant[1]), get_destination(following_connection));
            }
        }
    }

    cout << min_cost[k][last_node_id] << endl;
    return 0;
}
