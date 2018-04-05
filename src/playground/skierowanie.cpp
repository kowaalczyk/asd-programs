//
// Created by kowal on 13.02.18.
//

#include <cstdio>
#include <unordered_set>

using namespace std;


int m;
unsigned long n;

unordered_set<int> G[500001];
int depth[500001];

/// false if there exists a cycle with odd number of edges
bool dfs_no_odd_cycles(int current_node) {
    bool ans = true;
    for(auto n : G[current_node]) {
        if(depth[n] != -1 && (depth[current_node]-depth[n])% 2 == 0) {
            return false;
        } else if(depth[n] == -1) {
            depth[n] = depth[current_node] + 1;
            ans = ans && dfs_no_odd_cycles(n);
        }
    }
    return ans;
}


int main() {
    scanf("%d %d", &n, &m);

    for(int i=0; i<m; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        G[a].emplace(b);
        G[b].emplace(a);
    }
    for (int i = 1; i <= n; ++i) {
        depth[i] = -1;
    }
    bool ans = true;
    for (int i = 1; i <= n; ++i) {
        if(depth[i] == -1) {
            depth[i] = 0;
            ans = ans && dfs_no_odd_cycles(i);
        }
    }
    if(ans) {
        printf("TAK\n");
    } else {
        printf("NIE\n");
    }

    return 0;
}