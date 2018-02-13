//
// Created by kowal on 13.02.18.
//

#include <cstdio>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

int n, m; // nodes, edges
vector<pair<int, bool>> G[500001]; // <node, color>
int dist[500001][2]; // dist[node][color] -1 | distance from 1
queue<pair<int, bool>> q; // <node, reaching color>

void bfs() {
    for(int i=1; i<=n; i++) {
        dist[i][0] = -1;
        dist[i][1] = -1;
    }
    dist[1][0] = 0;
    dist[1][1] = 0;

    for(auto neigh : G[1]) {
        q.emplace(neigh.first, neigh.second);
        dist[neigh.first][neigh.second] = 1;
    }
    while(!q.empty()) {
        auto curr_data = q.front();
        q.pop();
        int curr_node = curr_data.first;
        bool curr_color = curr_data.second;

        for(auto neigh : G[curr_node]) {
            if(dist[neigh.first][neigh.second] == -1 && neigh.second != curr_color) {
                q.emplace(neigh.first, neigh.second);
                dist[neigh.first][neigh.second] = dist[curr_node][curr_color] + 1;
            }
        }
    }
}

int main() {
    scanf("%d %d", &n, &m);

    for(int i=0; i<m; i++) {
        int a, b, k;
        scanf("%d %d %d", &a, &b, &k);
        G[a].emplace_back(b, k);
        G[b].emplace_back(a, k);
    }

    bfs();

    for(int i=2; i<=n; i++) {
        int real_dist = (dist[i][0] == -1) ? dist[i][1] :
                        (dist[i][1] == -1) ? dist[i][0] : min(dist[i][0], dist[i][1]);
        printf("%d\n", real_dist);
    }
    return 0;
}