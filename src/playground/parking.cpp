//
// Created by kowal on 15.02.18.
//


#include <cstdio>
#include <vector>
#include <set>

using namespace std;



int n, m;
vector<int> G[500001];
bool visited[500001];
bool parked[500001];
set<int> reachable;


void dfs_parked(int i) {
    for(auto nb : G[i]) {
        if(!visited[nb]) {
            visited[nb] = true;
            if(parked[nb]) {
                reachable.emplace(nb);
            } else {
                dfs_parked(nb);
            }
        }
    }
}


int main() {
    scanf("%d %d", &n, &m);

    for(int i=1; i<=n; i++) {
        visited[i] = false;
        scanf("%d", &parked[i]);
    }
    for(int i=0; i<m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        G[u].emplace_back(v);
        G[v].emplace_back(u);
    }

    dfs_parked(1);

    for(auto v : reachable) {
        printf("%d\n", v);
    }
    return 0;
}