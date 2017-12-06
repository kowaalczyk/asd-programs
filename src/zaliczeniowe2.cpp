#include <iostream>
#include <vector>
#include <cassert>
#include <queue>

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

typedef long long int cost_t;
typedef pair<int, int> connection_with_id;
typedef pair<cost_t , cost_t > connection_data_t;

typedef tuple<id_t, id_t, id_t, cost_t, cost_t> connection_t; /// connection_id, from_node, to_node, cost, discount
typedef pair<cost_t, id_t> connection_dijkstra_t; ///


vector<connection_with_id> connections_from[MAX_NODES]; /// j is one of G[i].first <==> route i->j is present in graph. G[i].second is a connection_id
vector<connection_with_id> connections_to[MAX_NODES]; /// j is one of G_pred[i].first <==> route j->i is present in graph. G_pred[i].second is a connection_id

connection_data_t connection_data[MAX_CONNECTIONS]; /// connection_data[connection_id] := {connection_id.cost, connection_id.discount}
queue<int> Q; /// contains all nodes for which we can calculate min_cost
int routes_to_calc[MAX_NODES]; /// # of direct routes to each node without calculated cost

cost_t min_cost[MAX_COUPONS+1][MAX_NODES]; /// min_cost[k][n] := minimal cost to reach node n with k coupons

int get_node_id(connection_with_id connection) {
    return connection.first;
}

cost_t get_cost(connection_data_t connection) {
    return connection_data[connection.second].first;
}

cost_t get_discount(connection_data_t connection) {
    return connection_data[connection.second].second;
}

cost_t calc_min_cost(int node_id) {
    cost_t ans = NODE_UNREACHABLE;
    for(int coupons = MAX_COUPONS; coupons >= 0; coupons--) {
        if(min_cost[coupons][node_id] != NODE_UNREACHABLE) {
            if(ans == NODE_UNREACHABLE) {
                ans = min_cost[coupons][node_id];
            } else {
                ans = min(ans, min_cost[coupons][node_id]);
            }
        }
    }
    return ans;
}

// TODO: Rewrite everything in dijkstra

int main() {
    // init arrays
    for(int i=0; i<MAX_NODES; i++) {
        routes_to_calc[i] = 0;
        for(int j=0; j<=MAX_COUPONS; j++) {
            min_cost[j][i] = NODE_UNREACHABLE;
        }
    }

    // load variables
    int n, m, k; // # of cities, # of connection_data, # of coupons
    cin >> n >> m >> k;
    int last_node_id = n-1;

    for(int connection_id=0; connection_id<m; connection_id++) {
        int v, w, b, c; // from, to (indexed from 0), discount, cost without discount
        cin >> v >> w >> b >> c;
        // register connection_data in graph nodes
        connections_from[v].emplace_back(w, connection_id);
        connections_to[w].emplace_back(v, connection_id);
        // register connection cost and discount
        connection_data[connection_id].first = c;
        connection_data[connection_id].second = b;
    }

    // first bfs maps possible routes from node 0 to each of the nodes
    assert(routes_to_calc[0] == 0);
    Q.push(0);
    while(!Q.empty()) {
        int current_node_id = Q.front();
        Q.pop();
        for(auto connection : connections_from[current_node_id]) {
            int following_node_id = get_node_id(connection);
            if(routes_to_calc[following_node_id] == 0 && following_node_id != 0) {
                Q.push(following_node_id);
            }
            routes_to_calc[following_node_id]++;
        }
    }

    if(routes_to_calc[n-1] == 0) {
        // no route from node 0 leads to node n-1
        cout << -1 << endl;
        return 0;
    }

    // second bfs allows to calculate optimal cost dynamically
    assert(routes_to_calc[0] == 0); // TODO: This fails because starting node can have dependencies as well
    Q.push(0);
    while(!Q.empty()) {
        int current_node_id = Q.front();
        Q.pop();
        assert(routes_to_calc[current_node_id] == 0);
        // if we are calculating first node, set all costs to 0
        if(connections_to[current_node_id].empty()) {
            for(int coupons=0; coupons< MAX_COUPONS; coupons++) {
                min_cost[coupons][current_node_id] = 0;
            }
        }
        // calculate cost without discounts
        for(pair<int, int> & connection : connections_to[current_node_id]) {
            min_cost[0][current_node_id] = min_cost[0][get_node_id(connection)] + get_cost(connection);
        }
        // calculate cost with discounts
        for(int coupons=1; coupons <= MAX_COUPONS; coupons++) {
            for(auto connection : connections_to[current_node_id]) {
                long long cost_with_discounted_prev = min_cost[coupons][get_node_id(connection)] + get_cost(connection);
                long long cost_with_discounted_curr = min_cost[coupons-1][get_node_id(connection)] + get_cost(connection) - get_discount(connection);
                min_cost[coupons][current_node_id] = min(cost_with_discounted_prev, cost_with_discounted_curr);
            }
        }
        // stop if cost of reaching last node has just been calculated
        if(current_node_id == n-1) {
            break;
        }
        // add next nodes to queue if possible
        for(auto connection : connections_from[current_node_id]) {
            int following_node_id = get_node_id(connection);
            routes_to_calc[following_node_id]--;
            if(routes_to_calc[following_node_id] == 0) {
                Q.push(following_node_id);
            }
        }
    }

    cout << calc_min_cost(last_node_id) << endl;
    return 0;
}
