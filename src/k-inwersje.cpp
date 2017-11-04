#include <iostream>

using namespace std;

#define MOD_BIL %1000000000

// INTERVAL TREE IMPLEMENTATION

// assuming 1 is the index of root, [tree_size/2...tree_size-1] are leaves
unsigned int tree[160000];
unsigned int tree_size;

// tree helpers

unsigned int tree_pos(unsigned int leaf_pos) {
    return tree_size/2 + leaf_pos;
}

unsigned int tree_parent_pos(unsigned int node_pos) {
    return node_pos/2;
}

unsigned int tree_lson_pos(unsigned int node_pos) {
    return node_pos*2;
}

unsigned int tree_rson_pos(unsigned int node_pos) {
    return 1 + node_pos*2;
}

// tree interface
void tree_update_parents(unsigned int node_pos) {
    if(node_pos > 1) {
        unsigned int parent_pos = tree_parent_pos(node_pos);
        tree[parent_pos] = (tree[tree_lson_pos(parent_pos)] MOD_BIL  + tree[tree_rson_pos(parent_pos)] MOD_BIL) MOD_BIL;
        tree_update_parents(parent_pos);
    }
}

void tree_reset() {
    for(int i=0; i<40000; i++) {
        tree[i] = 0;
    }
}

void tree_create(unsigned int leaves) {
    tree_size = 8*leaves;
    tree_reset();
}

void tree_set(unsigned int leaf_pos, unsigned int val) {
    unsigned int pos = tree_pos(leaf_pos);
    tree[pos] = val MOD_BIL;
    tree_update_parents(pos);
}

// for leaf_pos j, this will return sum of values from leaves at positions [0..j-1]
unsigned int leaf_sum(unsigned int leaf_pos) {
    unsigned int sum = 0;

    unsigned int pos = tree_pos(leaf_pos);
    unsigned int prev_pos = pos;
    pos = tree_parent_pos(pos);
    while(pos>=1) {
        // add sum from left subtree if we have moved to the left
        if(prev_pos != tree_lson_pos(pos)) {
            sum += tree[tree_lson_pos(pos)] MOD_BIL;
        }

        prev_pos = pos;
        pos = tree_parent_pos(pos);
    }
    return sum MOD_BIL;
}

// 2lists: one contains current_k-inversions, second one - (current_one-1)-inversions
// tree is initialized to zeros for every level
// iterating from r too l, we count the current_k from the tree and place
// and place (current_k-1)-inversions in its place (so that it can be counted by following iterations in a loop)

int main() {
    unsigned int n, k;
    unsigned int tab[20000];
    unsigned int new_leaves[20000];
    unsigned int old_leaves[20000];
    unsigned int k_inversions = 0;

    cin >> n >> k;
    for(unsigned int i=0; i<n; i++) {
        cin >> tab[i];
    }
    for(unsigned int i=n; i<20000; i++) {
        tab[i] = 0;
    }
    tree_create(n);
    for(unsigned int i=0; i<20000; i++) {
        old_leaves[i] = 1;
    }

    /*
     * new_leaves[i] := (current_k)-inversions where i is the smallest number
     * old_leaves[i] := (current_k-1)-inversions where i is the smallest number
     */
    for(unsigned int current_k=2; current_k<=k; current_k++) {
        tree_reset();
        for(unsigned int i=n; i>0; i--) {
            unsigned int val = tab[i-1];
            // calculate (current_k)-inversions and store it in new leaves
            new_leaves[val-1] = leaf_sum(val-1) MOD_BIL;
            // add number of (current_k-1)-inversions to the tree so that the following numbers can count it
            tree_set(val-1, old_leaves[val-1]);
        }
        if(current_k == k) {
            // calculate sum of (k)-inversions from new_leaves array
            for(unsigned int i=0; i<20000; i++) {
                k_inversions += new_leaves[i] MOD_BIL;
            }
        } else {
            // update old leaves
            for(unsigned int i=0; i<20000; i++) {
                old_leaves[i] = new_leaves[i] MOD_BIL;
            }
        }
    }

    cout << k_inversions MOD_BIL << endl;
    return 0;
}