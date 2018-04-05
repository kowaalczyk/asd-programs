//
// Created by kowal on 16.02.18.
//

#include <cmath>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;

// TODO: Segfault - need to move to pointer implementation

int tree[1048576]; // [1] - root, [2n] - lson of n, [2n+1] - rson of n
int total_height = 0;
int root_height = 0;

void tree_insert(int new_node) {
    int pos = 1;
    while(tree[pos] != -1) {
        if(tree[pos] > new_node) {
            pos = 2*pos;
        } else if(tree[pos] < new_node) {
            pos = 2*pos + 1;
        } else {
            return; // already present in a tree
        }
    }
    tree[pos] = new_node;
}

int dfs_calc_height(int node) {
    int curr_height;
    if(tree[2*node] == -1 && tree[2*node+1] == -1) {
        curr_height = 0;
    } else if(tree[2*node] == -1) {
        curr_height = dfs_calc_height(2*node+1) + 1;
    } else if(tree[2*node+1] == -1) {
        curr_height = dfs_calc_height(2*node) + 1;
    } else {
        curr_height = max(dfs_calc_height(2*node), dfs_calc_height(2*node+1)) + 1;
    }
    total_height += curr_height;
    return curr_height;
}

int main() {
    memset(tree, -1, sizeof(int)*1048576);

    int n;
    scanf("%d", &n);
    for(int i=0; i<n; i++) {
        int new_node;
        scanf("%d", &new_node);
        tree_insert(new_node);
    }

    root_height = dfs_calc_height(1);
    printf("%d\n%d\n", root_height, total_height);
    return 0;
}
