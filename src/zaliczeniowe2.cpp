#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

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

typedef int id_t;
typedef long long int cost_t;

typedef tuple<id_t, id_t, id_t, cost_t, cost_t> connection_t; /// connection_id, from_node, to_node, cost, discount
typedef pair<cost_t, id_t> connection_dijkstra_t;

connection_t connections[MAX_CONNECTIONS];
vector<id_t> connections_from[MAX_NODES]; /// G[i] := vector of connection ids starting from i
priority_queue<connection_dijkstra_t> dijkstra_q; /// priority queue for Dijkstra algorithm

cost_t min_cost[MAX_COUPONS+1][MAX_NODES]; /// min_cost[k][n] := minimal cost to reach node n with k coupons

void load_connection(id_t connection_id) {
    int v, w, b, c;
    cin >> v >> w >> b >> c;
    connection_t new_connection {connection_id, v, w, b, c};
    connections[connection_id] = new_connection;
    connections_from[v].push_back(connection_id);
}

int main() {
    // init arrays
    for(int i=0; i<MAX_NODES; i++) {
        for(int j=0; j<=MAX_COUPONS; j++) {
            min_cost[j][i] = NODE_UNREACHABLE;
        }
    }

    // load variables
    int n, m, k; // # of cities, # of connection_data, # of coupons
    cin >> n >> m >> k;
    const int last_node_id = n-1;

    for(id_t connection_id=0; connection_id<m; connection_id++) {
        load_connection(connection_id);
    }

    // TODO: standard Dijkstra for calculating cost with 0 discount && checking whether last_node is reachable

    // TODO: 2nd Dijkstra for calculating other costs

    cout << min_cost << endl;
    return 0;
}
