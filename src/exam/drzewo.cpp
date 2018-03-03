//
// Created by kowal on 03.03.18.
//

#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;


vector<int>G[500000];

vector<int> subtree_sum;
vector<int> subtree_count;
vector<int> ans;


void sum_and_size(int node) {
    for(auto sub : G[node]) {
        sum_and_size(sub);
        subtree_sum[node] += subtree_sum[sub];
        subtree_sum[node] += subtree_count[sub] + 1;
        subtree_count[node] += subtree_count[sub] + 1;
    }
    printf("Node: %d, count: %d, sum: %d\n", node, subtree_count[node], subtree_sum[node]);
}

void remove_parent(int node, int parent) {
    if(parent != -1) {
        G[node].erase(remove(G[node].begin(), G[node].end(), parent));
    }
    for(auto sub : G[node]) {
        remove_parent(sub, node);
    }
}


void root_tree(int node) {
    for(auto sub : G[node]) {
        remove_parent(sub, node);
    }
    sum_and_size(node);
}


int main() {
    int n;
    scanf("%d", &n);
    subtree_sum.reserve(static_cast<unsigned long>(n));
    subtree_count.reserve(static_cast<unsigned long>(n));
    ans.reserve(static_cast<unsigned long>(n));
    for(int i=0; i<n-1; i++) {
        int p,q;
        scanf("%d %d", &p, &q);
        G[p].emplace_back(q);
        G[q].emplace_back(p);
    }
    for(int i=0; i<n; i++) {
        ans[i] = 0;
        subtree_count[i] = 0;
        subtree_sum[i] = 0;
    }

    root_tree(1);

    return 0;
}
