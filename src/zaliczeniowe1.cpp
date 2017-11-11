#include <iostream>
#include <algorithm>
#include <climits>
#include <assert.h>

using namespace std;

typedef long long tree_val_t;
typedef int tree_pos_t;

// PROBLEM PARAMS --------------------------------------------------------------------------

static const int MAX_ALLOWED_REVENUE = 2000000000;
static const int MIN_ALLOWED_REVENUE = 0;

// INTERVAL TREE PARAMS --------------------------------------------------------------------

static const tree_pos_t TREE_MAX_SIZE = 800008; //TODO: Double-check
static tree_pos_t tree_size;

tree_val_t min_t[TREE_MAX_SIZE];
tree_val_t max_t[TREE_MAX_SIZE];
tree_val_t delta[TREE_MAX_SIZE];

// INTERVAL TREE BASE -----------------------------------------------------------------------

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

void tree_reset(tree_val_t tree[], tree_val_t base_value=0) {
    for(tree_pos_t i=0; i < TREE_MAX_SIZE; i++) {
        tree[i] = base_value;
    }
}

void tree_create(tree_val_t tree[], tree_pos_t leaves, tree_val_t base_value=0) {
    tree_size = 1;
    while(tree_size <= 2*leaves) {
        tree_size *= 2;
    }
    tree_reset(tree, base_value);
}


// INTERVAL TREE EXTENDED -------------------------------------------------------------------

/// calculates minimums and maximus bottom-up starting from node_pos
void tree_recalc(tree_pos_t node_pos) {
    if(node_pos > 1) {
        tree_pos_t parent_pos = tree_parent_pos(node_pos);
        auto lson_pos = tree_lson_pos(parent_pos);
        auto rson_pos = tree_rson_pos(parent_pos);
        min_t[parent_pos] = min((min_t[lson_pos] + delta[lson_pos]), (min_t[rson_pos] + delta[rson_pos]));
        max_t[parent_pos] = max((max_t[lson_pos] + delta[lson_pos]), (max_t[rson_pos] + delta[rson_pos]));
        tree_recalc(parent_pos);
    }
}

/// initializes trees based on array of values
void set_minmax(const tree_val_t rev[], tree_pos_t rev_size) {
    for(tree_pos_t i = 1; i <= rev_size; i++) {
        tree_pos_t leaf = tree_pos(i-1);
        min_t[leaf] = rev[i];
        max_t[leaf] = rev[i];
        tree_recalc(leaf);
    }
}

/// updates one node based on its present delta, without updating node's parents or children
void update_if_delta_present(tree_pos_t current_pos, tree_pos_t current_left_bound, tree_pos_t current_right_bound) {
    if(delta[current_pos]) {
        // update current_pos if it needs to be updated
        min_t[current_pos] += delta[current_pos];
        max_t[current_pos] += delta[current_pos];
        if(current_left_bound != current_right_bound) {
            delta[tree_lson_pos(current_pos)] += delta[current_pos];
            delta[tree_rson_pos(current_pos)] += delta[current_pos];
        }
        delta[current_pos] = 0;
    }
}

/// use minmax_violated
bool minmax_violated_recursive(
        tree_pos_t current_pos,
        tree_pos_t current_left_bound,
        tree_pos_t current_right_bound,
        tree_pos_t left_bound,
        tree_pos_t right_bound,
        tree_val_t val) {
    update_if_delta_present(current_pos, current_left_bound, current_right_bound);
    if(current_left_bound > current_right_bound || current_left_bound > right_bound || current_right_bound < left_bound) {
        return false;
    }
    if(current_left_bound >= left_bound && current_right_bound <= right_bound) {
        tree_val_t calc_min = min_t[current_pos] + val;
        tree_val_t calc_max = max_t[current_pos] + val;
        return (calc_min < MIN_ALLOWED_REVENUE || calc_max > MAX_ALLOWED_REVENUE);
    }
    tree_pos_t divider = (current_left_bound + current_right_bound) / 2;
    bool ans = minmax_violated_recursive(tree_lson_pos(current_pos), current_left_bound, divider, left_bound, right_bound, val);
    ans = ans || minmax_violated_recursive(tree_rson_pos(current_pos), divider+1, current_right_bound, left_bound, right_bound, val);
    return ans;
}

/// checks if minimum and maximum revenue requirements will not be violated after interval [l,r] is updated with val
bool minmax_violated(tree_pos_t l, tree_pos_t r, tree_val_t val) {
    return minmax_violated_recursive(tree_root_pos(), tree_pos(0), tree_size-1, tree_pos(l-1), tree_pos(r-1), val);
}

/// use update_minmax
void update_minmax_recursive(
        tree_pos_t current_pos,
        tree_pos_t current_left_bound,
        tree_pos_t current_right_bound,
        tree_pos_t left_bound,
        tree_pos_t right_bound,
        tree_val_t val) {
    update_if_delta_present(current_pos, current_left_bound, current_right_bound);
    if(current_left_bound > current_right_bound || current_left_bound > right_bound || current_right_bound < left_bound) {
        return;
    }
    if(current_left_bound >= left_bound && current_right_bound <= right_bound) {
        // representing change of all leaves in interval by val by changing min and max in critical points
        min_t[current_pos] += val;
        max_t[current_pos] += val;
        if(current_left_bound != current_right_bound) {
            // not a leaf
            delta[tree_lson_pos(current_pos)] += val;
            delta[tree_rson_pos(current_pos)] += val;
        }
        return;
    }
    tree_pos_t divider = (current_left_bound + current_right_bound) / 2;
    min_t[current_pos] = min(min_t[tree_lson_pos(current_pos)], min_t[tree_rson_pos(current_pos)]);
    max_t[current_pos] = max(max_t[tree_lson_pos(current_pos)], max_t[tree_rson_pos(current_pos)]);
}

/// updates min_t and max_t wiht val
void update_minmax(tree_pos_t l, tree_pos_t r, tree_val_t val) {
    update_minmax_recursive(tree_root_pos(), tree_pos(0), tree_size-1, tree_pos(l-1), tree_pos(r-1), val);
}

/// updates min and max for each of the leaves with its and its predecessors deltas
tree_val_t update_leaf_values(const tree_pos_t leaf_pos[], tree_pos_t count) {
    //iterating though each individual element makes sure that top-down tree search reaches bottom
    for(tree_pos_t i=0; i<count; i++) {
        update_minmax(i, i, 0);
    }
}

/// calculates revenue growth changes afer [l,r] is updated with delta
tree_val_t calc_growth_changes(tree_pos_t l, tree_pos_t r, tree_pos_t rev_size, tree_val_t updated_val) {
    tree_val_t ans = 0;
    if(l>1) {
        tree_pos_t els[2] = {tree_pos(l-2), tree_pos(l-1)};
        update_leaf_values(els, 2);

        tree_val_t prev_val = min_t[els[1]] - updated_val;
        tree_val_t new_val = min_t[els[1]];
        tree_val_t left_val = min_t[els[0]];
        if(left_val < prev_val && left_val >= new_val) {
            ans--;
        } else if(left_val >= prev_val && left_val < new_val) {
            ans++;
        }
    }
    if(r<rev_size) {
        tree_pos_t els[2] = {tree_pos(r-1), tree_pos(r)};
        update_leaf_values(els, 2);

        tree_val_t prev_val = min_t[els[0]] - updated_val;
        tree_val_t new_val = min_t[els[0]];
        tree_val_t right_val = min_t[els[1]];
        if(prev_val < right_val && new_val >= right_val) {
            ans--;
        } else if(prev_val >= right_val && new_val < right_val) {
            ans++;
        }
    }
    return ans;
}

// HELPERS ----------------------------------------------------------------------------------

/// calculates number of initial growth periods
tree_pos_t base_growth_periods(const tree_val_t rev[], tree_pos_t rev_size) {
    tree_pos_t ans = 0;
    for(tree_pos_t i = 2; i <= rev_size; i++) {
        if(rev[i-1] < rev[i]) {
            ans++;
        }
    }
    return ans;
}

// SOLUTION ---------------------------------------------------------------------------------

int main() {
    tree_pos_t n, m;
    cin >> n;
    tree_val_t rev[n+1]; // input data is indexed from 1
    for(tree_pos_t i = 1; i <= n; i++) {
        cin >> rev[i];
    }
    cin >> m;

    tree_create(delta, n);
    tree_create(min_t, n, LLONG_MAX);
    tree_create(max_t, n, LLONG_MIN);
    set_minmax(rev, n);
    tree_pos_t growth_periods = base_growth_periods(rev, n);

    for(tree_pos_t i=0; i<m; i++) {
        tree_pos_t current_l, current_r;
        tree_val_t current_delta;
        cin >> current_l >> current_r >> current_delta;

        if(minmax_violated(current_l, current_r, current_delta)) {
            cout << -1 << endl;
        } else {
            update_minmax(current_l, current_r, current_delta);
            growth_periods += calc_growth_changes(current_l, current_r, n, current_delta);
            cout << growth_periods << endl;
        }
    }
    return 0;
}
