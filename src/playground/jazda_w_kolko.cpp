//
// Created by kowal on 15.02.18.
//

#include <cstdio>
#include <vector>

using namespace std;


int m;
unsigned long n;
vector<int>G[200001];
bool visited[200001];

bool dfs_cycle(int i, int from) {
    visited[i] = true;

    bool ans = false;
    for(auto nb : G[i]) {
        if(!visited[nb]) {
            ans = ans || dfs_cycle(nb, i);
        } else if(nb != from) {
            // visited and not the one we came from => there is a cycle
            ans = true;
        }
    }
    return ans;
}

int main() {
    scanf("%d %d", &n, &m);
    for(int i=0; i<m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        G[u].push_back(v);
        G[v].push_back(u);
    }
    for(int i=1; i<=n; i++) {
        visited[i] = false;
    }

    bool ans = false;
    for(int i=1; i<=n; i++) {
        if(!visited[i]) {
            ans = ans || dfs_cycle(i, 0);
        }
    }
    if(ans) {
        printf("TAK");
    } else {
        printf("NIE");
    }
    return 0;
}
