#ifndef PROJECT_INTERVAL_TREE_H
#define PROJECT_INTERVAL_TREE_H


// TODO: Rename
#define MOD_BIL %1000000000

// PARAMS
typedef unsigned long long tree_val_t;
typedef unsigned int tree_pos_t;

static const tree_pos_t TREE_MAX_SIZE = 160000;
static const tree_pos_t N_MAX_SIZE = 20000;

// Helpers
tree_pos_t &tree_size();
tree_pos_t tree_root_pos();
tree_pos_t tree_pos(tree_pos_t leaf_pos);
tree_pos_t tree_parent_pos(tree_pos_t node_pos);
tree_pos_t tree_lson_pos(tree_pos_t node_pos);
tree_pos_t tree_rson_pos(tree_pos_t node_pos);

// Debug functions
void tree_print_debug(tree_val_t tree[]);

/**
 * Core tree function.
 * Base function used for calculating values for intervals
 * @param lson_value - value of left son
 * @param rson_value - value of right son
 * @return aggregate of the parameters, determines how interval values are calculated
 */
tree_val_t tree_base_func(tree_val_t lson_value, tree_val_t rson_value);

// Basic tree functions
void tree_update_parents(tree_val_t tree[], tree_pos_t node_pos);
void tree_reset(tree_val_t tree[]);
void tree_create(tree_val_t tree[], tree_pos_t leaves);
void tree_set(tree_val_t tree[], tree_pos_t leaf_pos, tree_val_t val);

// Extended tree functions (for specific use cases)

/**
 * Aggregate value for leaves smaller then leaf_pos
 * @param tree - tree on which the calculation is performed
 * @param leaf_pos - position of the leaf
 * @return
 */
tree_val_t tree_leaf_sum(tree_val_t tree[], tree_pos_t leaf_pos);


#endif //PROJECT_INTERVAL_TREE_H
