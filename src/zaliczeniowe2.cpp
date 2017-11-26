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

const int MAX_NODES = 20000;
const int MAX_COUPONS = 10;
const int MAX_CONNECTIONS = 1000000;
const int NODE_UNREACHABLE = -1;

vector<pair<int, int>> G[MAX_NODES]; /// j is one of G[i].first <==> route i->j is present in graph. G[i].second is a connection_id
vector<pair<int, int>> G_pred[MAX_NODES]; /// j is one of G_pred[i].first <==> route j->i is present in graph. G_pred[i].second is a connection_id
pair<long long, long long> connections[MAX_CONNECTIONS];

queue<int> Q; /// contains all nodes for which we can calculate min_cost
int routes_to_calc[MAX_NODES]; /// # of direct routes to each node without calculated cost
long long min_cost[MAX_COUPONS+1][MAX_NODES]; /// min_cost[k][n] := minimal cost to reach node n with k coupons

int get_node_id(pair<int, int> connection) {
    return connection.first;
}

long long get_cost(pair<int, int> connection) {
    return connections[connection.second].first;
}

long long get_discount(pair<int, int> connection) {
    return connections[connection.second].second;
}

long long calc_min_cost(int node_id) {
    long long ans = -1;
    for(int coupons = MAX_COUPONS; coupons >= 0; coupons--) {
        if(min_cost[coupons][node_id] != -1) {
            if(ans == -1) {
                ans = min_cost[coupons][node_id];
            } else {
                ans = min(ans, min_cost[coupons][node_id]);
            }
        }
    }
    return ans;
}

int main() {
    // init arrays
    for(int i=0; i<MAX_NODES; i++) {
        routes_to_calc[i] = 0;
        for(int j=0; j<=MAX_COUPONS; j++) {
            min_cost[j][i] = NODE_UNREACHABLE;
        }
    }

    // load variables
    int n, m, k; // # of cities, # of connections, # of coupons
    cin >> n >> m >> k;
    int last_node_id = n-1;

    for(int connection_id=0; connection_id<m; connection_id++) {
        int v, w, b, c; // from, to (indexed from 0), discount, cost without discount
        cin >> v >> w >> b >> c;
        // register connections in graph nodes
        G[v].emplace_back(w, connection_id);
        G_pred[w].emplace_back(v, connection_id);
        routes_to_calc[w]++;
        // register connection cost and discount
        connections[connection_id].first = c;
        connections[connection_id].second = b;
    }

    assert(routes_to_calc[0] == 0); // TODO: This fails because starting node can have dependencies as well
    Q.push(0);
    while(!Q.empty()) {
        int current_node_id = Q.front();
        Q.pop();
        assert(routes_to_calc[current_node_id] == 0);
        // if we are calculating first node, set all costs to 0
        if(G_pred[current_node_id].empty()) {
            for(int coupons=0; coupons< MAX_COUPONS; coupons++) {
                min_cost[coupons][current_node_id] = 0;
            }
        }
        // calculate cost without discounts
        for(pair<int, int> & connection : G_pred[current_node_id]) {
            min_cost[0][current_node_id] = min_cost[0][get_node_id(connection)] + get_cost(connection);
        }
        // calculate cost with discounts
        for(int coupons=1; coupons <= MAX_COUPONS; coupons++) {
            for(auto connection : G_pred[current_node_id]) {
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
        for(auto connection : G[current_node_id]) {
            int following_node_id = get_node_id(connection);
            routes_to_calc[following_node_id]--;
            if(routes_to_calc[following_node_id] == 0) {
                Q.push(following_node_id);
            }
        }
    }
    // calculate optimal cost of reaching last node
    if(min_cost[0][last_node_id] == -1) {
        return -1;
    } else {

    }

    cout << calc_min_cost(last_node_id) << endl;
    return 0;
}
