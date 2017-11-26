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

long long get_cost(pair<int, int> connection) {
    return connections[connection.second].first;
}

long long get_discount(pair<int, int> connection) {
    return connections[connection.second].second;
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

    assert(routes_to_calc[0] == 0);
    Q.push(0);
    while(!Q.empty()) {
        int current_node_id = Q.front();
        Q.pop();
        assert(routes_to_calc[current_node_id] == 0);

        for(int coupons=0; coupons <= MAX_COUPONS; coupons++) {
            if(G_pred[current_node_id].empty()) {
                min_cost[coupons][current_node_id] = 0;
            } else {
                for(int pred_id : G_pred[current_node_id]) {
                    // cost with x discounts =
                    // = min(
                    //       prev_cost_with_x_discounts + current_cost,
                    //       prev_cost_with_(x-1)_discounts + current_cost_with_discount
                    // )
                }
            }
        }
    }

    // TODO: min koszt przejścia od 0 do n-1 || -1 jeśli niemożliwe
    return 0;
}
