//
// Created by kowal on 26.01.18.
//

#include <iostream>
#include <vector>
#include <cstring>
#include <unordered_map>

using namespace std;

typedef int_fast32_t weight_t;
typedef int_fast32_t node_t;

unordered_map<weight_t, size_t> G[500001];
size_t total_edges[500001]; // total_edges[i] := # of all edges at node i


/// mark node v as a neighbour of u with weight w, and calculate new bumps at node u
size_t insert_count_new_bumps(node_t u, node_t v, weight_t w) {
    G[u][w]++;
    total_edges[u]++;
    return total_edges[u] - G[u][w];
}

int main() {
    memset(total_edges, 0, sizeof(total_edges));
    ios_base::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    uint64_t total_bumps = 0;
    for (int i=0; i<m; i++) {
        int u, v, w;
        cin >> u >> v >> w;

        total_bumps += insert_count_new_bumps(u, v, w);
        total_bumps += insert_count_new_bumps(v, u, w);
    }

    cout << total_bumps << endl;
    return 0;
}
