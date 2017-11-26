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
const int NODE_UNREACHABLE = -1;

vector<int> G[MAX_NODES]; /// j in G[i] <==> route i->j is present in graph
vector<int> G_pred[MAX_NODES]; /// j in G_pred[i] <==> route j->i is present in graph
long long min_cost[MAX_COUPONS+1][MAX_NODES]; /// min_cost[k][n] := minimal cost to reach node n with k coupons
int routes_to_calc[MAX_NODES]; /// # of direct routes to each node without calculated cost
queue<int> Q; /// contains all nodes for which we can calculate min_cost

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
    for(int i=0; i<m; i++) {
        int v, w, b, c; // from, to (indexed from 0), discount, cost without discount
        cin >> v >> w >> b >> c;
        G[v].push_back(w);
        G_pred[w].push_back(v);
        routes_to_calc[w]++;
        // TODO: Find a way to cache discounts optimally
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
