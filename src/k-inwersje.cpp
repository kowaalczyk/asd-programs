#include <iostream>

using namespace std;

#define MOD_BIL %1000000000

static const unsigned long long TREE_MAX_SIZE = 160000;
static const int N_MAX_SIZE = 20000;

// INTERVAL TREE IMPLEMENTATION
// assuming 1 is the index of root, [tree_size/2...tree_size-1] are leaves
// (tree[0] is just a guard preventing overflow in loops)
unsigned long long tree[TREE_MAX_SIZE];
unsigned long long tree_size;

// tree helpers

unsigned long long tree_pos(unsigned long long leaf_pos) {
    return tree_size/2 + leaf_pos;
}

unsigned long long tree_parent_pos(unsigned long long node_pos) {
    return node_pos/2;
}

unsigned long long tree_lson_pos(unsigned long long node_pos) {
    return node_pos*2;
}

unsigned long long tree_rson_pos(unsigned long long node_pos) {
    return 1 + node_pos*2;
}

// tree interface

void tree_update_parents(unsigned long long node_pos) {
    if(node_pos > 1) {
        unsigned long long parent_pos = tree_parent_pos(node_pos);
        tree[parent_pos] = (tree[tree_lson_pos(parent_pos)] MOD_BIL  + tree[tree_rson_pos(parent_pos)] MOD_BIL) MOD_BIL;
        tree_update_parents(parent_pos);
    }
}

void tree_reset() {
    for(int i=0; i < TREE_MAX_SIZE; i++) {
        tree[i] = 0;
    }
}

void tree_create(unsigned long long leaves) {
    tree_size = 1;
    while(tree_size <= 2*leaves) {
        tree_size *= 2;
    }
    tree_size *= 2;
    tree_reset();
}

void tree_set(unsigned long long leaf_pos, unsigned long long val) {
    unsigned long long pos = tree_pos(leaf_pos);
    tree[pos] = val MOD_BIL;
    tree_update_parents(pos);
}

// for leaf_pos j, this will return sum of values from leaves at positions [0..j-1]
unsigned long long leaf_sum(unsigned long long leaf_pos) {
    unsigned long long sum = 0;

    unsigned long long pos = tree_pos(leaf_pos);
    unsigned long long prev_pos = pos;
    pos = tree_parent_pos(pos);
    while(pos>=1) {
        // add sum from left subtree if we have moved to the left
        if(prev_pos != tree_lson_pos(pos)) {
            sum = (sum MOD_BIL + tree[tree_lson_pos(pos)] MOD_BIL) MOD_BIL;
        }
        prev_pos = pos;
        pos = tree_parent_pos(pos);
    }
    return sum MOD_BIL;
}

// SOLUTION
// 2lists: one contains current_k-inversions, second one - (current_k-1)-inversions
// tree is initialized to zeros for every level
// iterating from r to l, we count the current_k from the tree
// and place (current_k-1)-inversions in its place (so that it can be counted by following iterations in a loop)

int main() {
    unsigned long long n, k;
    unsigned long long tab[N_MAX_SIZE];
    unsigned long long new_leaves[N_MAX_SIZE];
    unsigned long long old_leaves[N_MAX_SIZE];
    unsigned long long k_inversions = 0;

    cin >> n >> k;
    for(unsigned long long i=0; i<n; i++) {
        cin >> tab[i];
        old_leaves[i] = 1;
        new_leaves[i] = 0;
    }
    for(unsigned long long i=n; i < N_MAX_SIZE; i++) {
        old_leaves[i] = 0;
        new_leaves[i] = 0;
        tab[i] = 0;
    }
    tree_create(n);

    /*
     * new_leaves[i] := (current_k)-inversions where i is the biggest number
     * old_leaves[i] := (current_k-1)-inversions where i is the biggest number
     */
    for(unsigned long long current_k=2; current_k<=k; current_k++) {
        tree_reset();
        for(unsigned long long i=n; i>0; i--) {
            unsigned long long val = tab[i-1];
            // calculate (current_k)-inversions and store it in new leaves
            new_leaves[val-1] = leaf_sum(val-1) MOD_BIL;
            // add number of (current_k-1)-inversions to the tree so that the following numbers can count it
            tree_set(val-1, old_leaves[val-1]);
        }
        if(current_k == k) {
            // calculate sum of (k)-inversions from new_leaves array
            for(unsigned long long i=0; i < N_MAX_SIZE; i++) {
                k_inversions = (k_inversions MOD_BIL + new_leaves[i] MOD_BIL) MOD_BIL;
            }
        } else {
            // update old leaves
            for(unsigned long long i=0; i < N_MAX_SIZE; i++) {
                old_leaves[i] = new_leaves[i] MOD_BIL;
            }
        }
    }
    cout << k_inversions MOD_BIL << endl;
    return 0;
}
