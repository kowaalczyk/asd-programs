#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

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

/**
 * Calculates sum of values in nodes from pos to tree root (including pos and root)
 * @param tree - tree from which values are summed up
 * @param node_pos - starting position
 * @return sum of values along the path from pos to root
 */
tree_val_t tree_sum_to_root(const tree_val_t tree[], tree_pos_t node_pos) {
    tree_val_t ans = 0;
    while(node_pos >= tree_root_pos()) {
        ans += tree[node_pos];
        node_pos = tree_parent_pos(node_pos);
    }
    return ans;
}

/**
 * Update minimums along the route from specified not to the root, accounting for calculated deltas
 * @param min_t - tree with minimums to update
 * @param delta_t - tree contianing calculated delta values
 * @param node_pos - starting node
 */
void tree_rebase_min(tree_val_t min_t[], tree_val_t delta_t[], tree_pos_t node_pos) {
    if(node_pos > 1) {
        tree_pos_t parent_pos = tree_parent_pos(node_pos);
        auto lson_pos = tree_lson_pos(parent_pos);
        auto rson_pos = tree_rson_pos(parent_pos);
        min_t[parent_pos] = min((min_t[lson_pos] + delta[lson_pos]), (min_t[rson_pos] + delta[rson_pos]));
        tree_rebase_min(min_t, delta_t, parent_pos);
    }
}

/**
 * Update maximums along the route from specified not to the root, accounting for calculated deltas
 * @param max_t - tree with maximums to update
 * @param delta_t - tree contianing calculated delta values
 * @param node_pos - starting node
 */
void tree_rebase_max(tree_val_t max_t[], tree_val_t delta_t[], tree_pos_t node_pos) {
    if(node_pos > 1) {
        tree_pos_t parent_pos = tree_parent_pos(node_pos);
        auto lson_pos = tree_lson_pos(parent_pos);
        auto rson_pos = tree_rson_pos(parent_pos);
        max_t[parent_pos] = max((max_t[lson_pos] + delta[lson_pos]), (max_t[rson_pos] + delta[rson_pos]));
        tree_rebase_max(max_t, delta_t, parent_pos);
    }
}

/**
 * Calculate set of critical points for an interval [left_leaf, right_leaf].
 * Critical points are a minimal subset of nodes in the tree that cover the provided interval with its subtrees.
 * @param left_leaf - left bound of the interval
 * @param right_leaf - right bound of the interval
 * @return - vector containing critical points
 */
vector<tree_pos_t> tree_critical_pts(tree_pos_t left_leaf, tree_pos_t right_leaf) {
    vector<tree_pos_t> ans;

    // translate bounds to tree positions
    tree_pos_t left_leaf_pos = tree_pos(left_leaf);
    tree_pos_t right_leaf_pos = tree_pos(right_leaf);

    // initialize left and right position that will iterate up the tree
    tree_pos_t left_pos = left_leaf_pos;
    tree_pos_t right_pos = right_leaf_pos;
    tree_pos_t prev_left_pos = 0; //not a position within a tree but always a valid tree_pos_t
    tree_pos_t prev_right_pos = 0;

    // use dynamically calculated bounds to determine if the desired interval is covered
    tree_pos_t left_pos_l_bound = left_pos;
    tree_pos_t left_pos_r_bound = left_pos;
    tree_pos_t right_pos_l_bound = right_pos;
    tree_pos_t right_pos_r_bound = right_pos;

    // this allows us to quickly update bounds on each level
    tree_pos_t level_multiplier = 1;

    // calculate critical_points in O(log(n))
    // NOTE: They cannot be higher than LCA(left_leaf, right_leaf)
    while(left_pos != right_pos) {
        prev_left_pos = left_pos;
        prev_right_pos = right_pos;
        left_pos = tree_parent_pos(left_pos);
        right_pos = tree_parent_pos(right_pos);
        level_multiplier *= 2;

        if(tree_rson_pos(left_pos) == prev_left_pos) {
            // left_pos moved to the left
            tree_pos_t rson_l_bound = left_pos_l_bound;
            left_pos_l_bound -= level_multiplier/2;
            if(rson_l_bound >= left_leaf_pos && left_pos_l_bound < left_leaf_pos) {
                //moved outside of interval, mark as critical point
                ans.push_back(tree_rson_pos(left_pos));
            }
        } else {
            // left_pos moved to the right
            tree_pos_t rson_l_bound = left_pos_r_bound+1;
            left_pos_r_bound += level_multiplier/2;
            if(rson_l_bound >= left_leaf_pos && left_pos_l_bound < left_leaf_pos) {
                //moved outside of interval, mark as critical point
                ans.push_back(tree_rson_pos(left_pos));
            }
        }
        if(tree_rson_pos(right_pos) == prev_right_pos) {
            // right_pos moved to the left
            tree_pos_t lson_r_bound = right_pos_l_bound-1;
            right_pos_l_bound -= level_multiplier/2;
            if(lson_r_bound <= right_leaf_pos && right_pos_r_bound > right_leaf_pos) {
                //moved outside of interval, mark as critical point
                ans.push_back(tree_lson_pos(right_pos));
            }
        } else {
            // right_pos moved to the right
            tree_pos_t lson_r_bound = right_pos_r_bound;
            right_pos_r_bound += level_multiplier/2;
            if(lson_r_bound <= right_leaf_pos && right_pos_r_bound > right_leaf_pos) {
                //moved outside of interval, mark as critical point
                ans.push_back(tree_lson_pos(right_pos));
            }
        }
    } // left_pos == right_pos
    if(ans.empty()) {
        // means LCA(current_l, current_r is the only critical point
        ans.push_back(left_pos);
    }
    return ans;
}

//bool minmax_violated_recursive(tree_val_t current_delta, vector<tree_pos_t> critical_points) {
//    vector<tree_val_t> critical_min;
//    vector<tree_val_t> critical_max;
//    for(auto cp : critical_points) {
//        auto update_val = tree_sum_to_root(delta, cp);
//        critical_max.push_back(max_t[cp] + update_val);
//        critical_min.push_back(min_t[cp] + update_val);
//    }
//    auto interval_min = *min_element(critical_min.begin(), critical_min.end());
//    auto interval_max = *max_element(critical_max.begin(), critical_max.end());
//    return (interval_max + current_delta > MAX_ALLOWED_REVENUE || interval_min + current_delta < MIN_ALLOWED_REVENUE);
//}

// NEW IMPLEMENTATION OF TREE ---------------------------------------------------------------

bool minmax_violated_recursive(
        tree_pos_t current_pos,
        tree_pos_t current_left_bound,
        tree_pos_t current_right_bound,
        tree_pos_t left_bound,
        tree_pos_t right_bound,
        tree_val_t val) {
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
    if(current_left_bound > current_right_bound || current_left_bound > right_bound || current_right_bound < left_bound) {
        // totally not where we want to be
        return false;
    }
    if(current_left_bound >= left_bound && current_right_bound <= right_bound) {
        tree_val_t calc_min = min_t[current_pos] + val;
        tree_val_t calc_max = max_t[current_pos] + val;
        return (calc_min < MIN_ALLOWED_REVENUE || calc_max > MAX_ALLOWED_REVENUE);
    }
    tree_pos_t mid = (current_left_bound + current_right_bound) / 2;
    bool ans = minmax_violated_recursive(tree_lson_pos(current_pos), current_left_bound, mid, left_bound, right_bound,
                                         val);
    ans = ans || minmax_violated_recursive(tree_rson_pos(current_pos), mid+1, current_right_bound, left_bound, right_bound, val);

    return ans;
}

bool minmax_violated(tree_pos_t l, tree_pos_t r, tree_val_t val) {
    return minmax_violated_recursive(tree_root_pos(), tree_pos(0), tree_size-1, tree_pos(l-1), tree_pos(r-1), val);
}

tree_val_t update_minmax_recursive(
        tree_pos_t current_pos,
        tree_pos_t current_left_bound,
        tree_pos_t current_right_bound,
        tree_pos_t left_bound,
        tree_pos_t right_bound,
        tree_val_t val) {
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
    if(current_left_bound > current_right_bound || current_left_bound > right_bound || current_right_bound < left_bound) {
        // totally not where we want to be
        return 0;
    }
    if(current_left_bound >= left_bound && current_right_bound <= right_bound) {
        // representing change of all leaves in interval by val by changing min and max in critical points
        tree_val_t growth_delta = 0;
        min_t[current_pos] += val;
        max_t[current_pos] += val;
        if(current_left_bound != current_right_bound) {
            // not a leaf
            delta[tree_lson_pos(current_pos)] += val;
            delta[tree_rson_pos(current_pos)] += val;
        } else {
            // a leaf => check if difference between previous/following rev value changed sign
            tree_val_t prev_val = min_t[current_pos] - val;
            tree_val_t new_val = min_t[current_pos];
            if(current_pos == left_bound && current_pos > tree_pos(0)) {
                tree_val_t left_val = min_t[current_pos-1];
                if(left_val < prev_val && left_val >= new_val) {
                    growth_delta--;
                } else if(left_val >= prev_val && left_val < new_val) {
                    growth_delta++;
                }
            }
            if(current_pos == right_bound && current_pos < tree_size-2) {
                tree_val_t right_val = min_t[current_pos+1];
                if(prev_val < right_val && new_val >= right_val) {
                    growth_delta--;
                } else if(prev_val >= right_val && new_val < right_val) {
                    growth_delta++;
                }
            }
        }
        return growth_delta;
    }
    tree_pos_t mid = (current_left_bound + current_right_bound) / 2;
    tree_val_t growth_delta = update_minmax_recursive(tree_lson_pos(current_pos), current_left_bound, mid, left_bound, right_bound, val);
    growth_delta += update_minmax_recursive(tree_rson_pos(current_pos), mid+1, current_right_bound, left_bound, right_bound, val);

    min_t[current_pos] = min(min_t[tree_lson_pos(current_pos)], min_t[tree_rson_pos(current_pos)]);
    max_t[current_pos] = max(max_t[tree_lson_pos(current_pos)], max_t[tree_rson_pos(current_pos)]);

    return growth_delta;
}

tree_val_t update_minmax(tree_pos_t l, tree_pos_t r, tree_val_t val) {
    return update_minmax_recursive(tree_root_pos(), tree_pos(0), tree_size-1, tree_pos(l-1), tree_pos(r-1), val);
}

// ------------------------------------------------------------------------------------------

int main() {
    tree_pos_t n, m;
    cin >> n;
    tree_val_t rev[n+1]; // 0  is a guard (input data is indexed from 1)
    for(tree_pos_t i = 1; i <= n; i++) {
        cin >> rev[i];
    }
    cin >> m;

    // initialize interval trees
    tree_create(delta, n);
    tree_create(min_t, n, LLONG_MAX);
    tree_create(max_t, n, LLONG_MIN);
    for(tree_pos_t i = 1; i <= n; i++) {
        tree_pos_t leaf = tree_pos(i-1);
        min_t[leaf] = rev[i];
        tree_rebase_min(min_t, delta, leaf);
        max_t[leaf] = rev[i];
        tree_rebase_max(max_t, delta, leaf);
    }

    // calculate base growth periods in O(n)
    tree_pos_t growth_periods = 0;
    for(tree_pos_t i = 2; i <= n; i++) {
        if(rev[i-1] < rev[i]) {
            growth_periods++;
        }
    }

    for(int I=0; I<m; I++) {
        tree_pos_t current_l, current_r;
        tree_val_t current_delta;
        cin >> current_l >> current_r >> current_delta;

//        vector<tree_pos_t > critical_points = tree_critical_pts(current_l-1, current_r-1);
//        if(minmax_violated_recursive(current_delta, critical_points)) {
//            cout << -1 << endl;
//            continue;
//        }
        if(minmax_violated(current_l, current_r, current_delta)) {
            cout << -1 << endl;
            continue;
        }
        tree_val_t growth_changes = update_minmax(current_l, current_r, current_delta);
        growth_periods += growth_changes;
        // Update deltas in critical points and min/max in their parents (for correct comparison in future iterations)
//        for(auto cp : critical_points) {
//            delta[cp] += current_delta;
//            tree_rebase_min(min_t, delta, cp);
//            tree_rebase_max(max_t, delta, cp);
//        }
        //check if delta affects growth_periods (only possible in rev[r] and rev[l+1])
        //TODO: Move to separate function + this needs to be read from critical pts
//        if(current_l >= 2) {
//            tree_val_t real_l = rev[current_l] + tree_sum_to_root(delta, tree_pos(current_l-1));
//            tree_val_t real_l_prev = real_l - current_delta;
//            tree_val_t real_l_left = rev[current_l-1] + tree_sum_to_root(delta, tree_pos(current_l-2));
//            if(real_l_prev > real_l_left && real_l <= real_l_left) {
//                growth_periods--;
//            }
//            if(real_l_prev <= real_l_left && real_l > real_l_left) {
//                growth_periods++;
//            }
//        }
//        if(current_r <= n-1) {
//            tree_val_t real_r = rev[current_r] + tree_sum_to_root(delta, tree_pos(current_r-1));
//            tree_val_t real_r_prev = real_r - current_delta;
//            tree_val_t real_r_right = rev[current_r+1] + tree_sum_to_root(delta, tree_pos(current_r));
//            if(real_r_prev >= real_r_right && real_r < real_r_right) {
//                growth_periods++;
//            }
//            if(real_r_prev < real_r_right && real_r >= real_r_right) {
//                growth_periods--;
//            }
//        }
        cout << growth_periods << endl;
    }

    return 0;
}