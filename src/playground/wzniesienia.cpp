//
// Created by kowal on 26.01.18.
//

#include <iostream>
#include <vector>
#include <cstring>
#include <set>
#include <unordered_map>

using namespace std;

typedef int_fast32_t weight_t;
typedef int_fast32_t node_t;

set<pair<weight_t, size_t>> G[500001]; // G[i] := set<weight, # of neighbours with such weight>
//unordered_map<weight_t, size_t> G[500001];
size_t total_edges[500001]; // total_edges[i] := # of all edges at node i


/// mark node v as a neighbour of u with weight w, and calculate new bumps at node u
size_t insert_count_new_bumps(node_t u, node_t v, weight_t w) {
    auto possible_pos = lower_bound(G[u].begin(), G[u].end(), pair<weight_t, size_t >(w, 0));

    if(possible_pos != G[u].end() && (*possible_pos).first == w) {
        // nodes u already has a neighbour with weight w

        // update count in a current node
        size_t new_count = (*possible_pos).second+1;
        possible_pos = G[u].erase(possible_pos);
        G[u].emplace_hint(possible_pos, w, new_count);
        total_edges[u]++;

        // new node creates a bump with each edge of different weight
        return total_edges[u] - new_count;
    }
    // node u has no neighbour with weight w

    // insert new weight into the set of neighbours
    G[u].emplace_hint(possible_pos, w, 1);
    total_edges[u]++;

    // new node creates a bump with each of the previous edges
    return total_edges[u]-1;
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
