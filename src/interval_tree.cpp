#include "interval_tree.h"

#include <iostream>
using std::cout;
using std::endl;

// CORE INTERVAL TREE IMPLEMENTATION -----------------------------------------------------------------------------------
// assuming 1 is the index of root, [tree_size/2...tree_size-1] are leaves
// (tree[0] is just a guard preventing overflow in loops)

//#define MOD_BIL %1000000000
//
//typedef unsigned long long tree_val_t;
//typedef unsigned int tree_pos_t;
//
//static const tree_pos_t TREE_MAX_SIZE = 160000;
//static const tree_pos_t N_MAX_SIZE = 20000;

//multiple trees can be handled by adding more arrays
tree_val_t tree[TREE_MAX_SIZE];
tree_val_t tree_size;

// helpers

tree_pos_t tree_root_pos() {
    return 1;
}

tree_pos_t tree_pos(tree_pos_t leaf_pos) {
    return tree_size/2 + leaf_pos;
}

tree_pos_t tree_parent_pos(tree_pos_t node_pos) {
    return node_pos/2;
}

tree_pos_t tree_lson_pos(tree_pos_t node_pos) {
    return node_pos*2;
}

tree_pos_t tree_rson_pos(tree_pos_t node_pos) {
    return 1 + node_pos*2;
}

tree_val_t tree_base_func(tree_val_t lson_value, tree_val_t rson_value) {
    return lson_value MOD_BIL + rson_value MOD_BIL;
}

void tree_print_debug(tree_val_t tree[]) {
    tree_pos_t breaker = 1;
    tree_pos_t counter = 0;

    for(tree_pos_t i=1; i < tree_size; i++) {
        cout << tree[i] << ' ';
        counter++;
        if(counter >= breaker) {
            cout << endl;
            breaker*=2;
            counter = 0;
        }
    }
}

// BASIC TREE METHODS --------------------------------------------------------------------------------------------------
// for sum-based interval tree

void tree_update_parents(tree_val_t tree[], tree_pos_t node_pos) {
    if(node_pos > 1) {
        tree_pos_t parent_pos = tree_parent_pos(node_pos);
        tree[parent_pos] = tree_base_func(tree[tree_lson_pos(parent_pos)] MOD_BIL, tree[tree_rson_pos(parent_pos)] MOD_BIL) MOD_BIL;
        tree_update_parents(tree, parent_pos);
    }
}

void tree_reset(tree_val_t tree[]) {
    for(tree_pos_t i=0; i < TREE_MAX_SIZE; i++) {
        tree[i] = 0;
    }
}

void tree_create(tree_val_t tree[], tree_pos_t leaves) {
    tree_size = 1;
    while(tree_size <= 2*leaves) {
        tree_size *= 2;
    }
    tree_size *= 2;
    tree_reset(tree);
}

void tree_set(tree_val_t tree[], tree_pos_t leaf_pos, tree_val_t val) {
    tree_pos_t pos = tree_pos(leaf_pos);
    tree[pos] = val MOD_BIL;
    tree_update_parents(tree, pos);
}

// EXTENDED TREE METHODS -----------------------------------------------------------------------------------------------

// for leaf_pos j, this will return sum of values from leaves at positions [0..j-1]
tree_val_t tree_leaf_sum(tree_val_t tree[], tree_pos_t leaf_pos) {
    tree_val_t sum = 0;

    tree_pos_t pos = tree_pos(leaf_pos);
    tree_pos_t prev_pos = pos;
    pos = tree_parent_pos(pos);
    while(pos>=1) {
        // add sum from left subtree if we have moved to the left
        if(prev_pos != tree_lson_pos(pos)) {
            sum = tree_base_func(sum MOD_BIL, tree[tree_lson_pos(pos)] MOD_BIL) MOD_BIL;
        }
        prev_pos = pos;
        pos = tree_parent_pos(pos);
    }
    return sum MOD_BIL;
}
