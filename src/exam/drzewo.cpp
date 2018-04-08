//
// Created by kowal on 03.03.18.
//

#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;


vector<int>G[500001];

vector<unsigned long long> subtree_sum;
vector<unsigned long long> subtree_count;
vector<unsigned long long> ans;


/// calculate sum and size of subtree recursively
void sum_and_size(int node) {
    for(auto sub : G[node]) {
        sum_and_size(sub);
        subtree_sum[node] += subtree_sum[sub];
        subtree_sum[node] += subtree_count[sub] + 1;
        subtree_count[node] += subtree_count[sub] + 1;
    }
//    printf("Node: %d, count: %d, sum: %d\n", node, subtree_count[node], subtree_sum[node]);
}

/// remove parents recursively
void remove_parent(int node, int parent) {
    if(parent != -1) {
        G[node].erase(remove(G[node].begin(), G[node].end(), parent));
    }
    for(auto sub : G[node]) {
        remove_parent(sub, node);
    }
}

/// root tree and calculate subtree params recursively
void root_tree(int node) {
    remove_parent(node, -1);
    sum_and_size(node);
}

/// calculate sum of all lengths in tree recursively
void calculate_sum(int node, int parent, int total_nodes) {
    if(parent != -1) {
//        ans[node] = (subtree_sum[parent]-subtree_sum[node]) + (total_nodes - subtree_count[node]);
        ans[node] = ans[parent] + total_nodes - 2*subtree_count[node] -2;
    } else {
        ans[node] = subtree_sum[node];
    }
    for(auto sub : G[node]) {
        calculate_sum(sub, node, total_nodes);
    }
}


int main() {
    // load data
    int n;
    scanf("%d", &n);
    subtree_sum.reserve(static_cast<unsigned long>(n+1));
    subtree_count.reserve(static_cast<unsigned long>(n+1));
    ans.reserve(static_cast<unsigned long>(n+1));
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
    // calculate answers
    root_tree(1);
    calculate_sum(1, -1, n);
    for(int i=1; i<=n; i++) {
        printf("%llu\n", ans[i]);
    }
    return 0;
}