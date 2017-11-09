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

void tree_rebase_min(tree_val_t tree[], tree_pos_t node_pos) {
    if(node_pos > 1) {
        tree_pos_t parent_pos = tree_parent_pos(node_pos);
        tree[parent_pos] = min(tree[tree_lson_pos(parent_pos)], tree[tree_rson_pos(parent_pos)]);
        tree_rebase_min(tree, parent_pos);
    }
}

void tree_rebase_max(tree_val_t tree[], tree_pos_t node_pos) {
    if(node_pos > 1) {
        tree_pos_t parent_pos = tree_parent_pos(node_pos);
        tree[parent_pos] = max(tree[tree_lson_pos(parent_pos)], tree[tree_rson_pos(parent_pos)]);
        tree_rebase_max(tree, parent_pos);
    }
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
        tree_rebase_min(min_t, leaf);
        max_t[leaf] = rev[i];
        tree_rebase_max(max_t, leaf);
    }

    // calculate base growth periods in O(n)
    tree_pos_t growth_periods = 0;
    for(tree_pos_t i = 2; i <= n; i++) {
        if(rev[i-1] < rev[i]) {
            growth_periods++;
        }
    }

    for(int I=0; I<m; I++) {
        int current_l, current_r, current_delta;
        cin >> current_l >> current_r >> current_delta;

        // critical_points := minimal set of nodes in the tree covering interval [l,r] with their subtrees
        vector<tree_pos_t > critical_points;

        tree_pos_t left_pos = tree_pos(current_l-1);
        tree_pos_t right_pos = tree_pos(current_r-1);
        tree_pos_t prev_left_pos = 0; //not a position within a tree but always a valid tree_pos_t
        tree_pos_t prev_right_pos = 0;

        tree_pos_t left_pos_l_range = left_pos;
        tree_pos_t left_pos_r_range = left_pos;
        tree_pos_t right_pos_l_range = right_pos;
        tree_pos_t right_pos_r_range = right_pos;

        tree_pos_t level_multiplier = 1;
        while(left_pos != right_pos) {
            // calculate critical_points in O(log(n))
            // NOTE: They cannot be higher than LCA(current_l, current_r)
            prev_left_pos = left_pos;
            prev_right_pos = right_pos;
            left_pos = tree_parent_pos(left_pos);
            right_pos = tree_parent_pos(right_pos);
            level_multiplier *= 2;

            if(tree_rson_pos(left_pos) == prev_left_pos) {
                // left_pos moved to the left
                tree_pos_t rson_l_range = left_pos_l_range;
                left_pos_l_range -= level_multiplier/2;
                if(rson_l_range >= tree_pos(current_l-1) && left_pos_l_range < tree_pos(current_l-1)) {
                    //moved outside of interval, mark as critical point
                    critical_points.push_back(tree_rson_pos(left_pos));
                }
            } else {
                // left_pos moved to the right
                tree_pos_t rson_l_range = left_pos_r_range+1;
                left_pos_r_range += level_multiplier/2;
                if(rson_l_range >= tree_pos(current_l-1) && left_pos_l_range < tree_pos(current_l-1)) {
                    //moved outside of interval, mark as critical point
                    critical_points.push_back(tree_rson_pos(left_pos));
                }
            }
            if(tree_rson_pos(right_pos) == prev_right_pos) {
                // right_pos moved to the left
                tree_pos_t lson_r_range = right_pos_l_range-1;
                right_pos_l_range -= level_multiplier/2;
                if(lson_r_range <= tree_pos(current_r-1) && right_pos_r_range > tree_pos(current_r-1)) {
                    //moved outside of interval, mark as critical point
                    critical_points.push_back(tree_lson_pos(right_pos));
                }
            } else {
                // right_pos moved to the right
                tree_pos_t lson_r_range = right_pos_r_range;
                right_pos_r_range += level_multiplier/2;
                if(lson_r_range <= tree_pos(current_r-1) && right_pos_r_range > tree_pos(current_r-1)) {
                    //moved outside of interval, mark as critical point
                    critical_points.push_back(tree_lson_pos(right_pos));
                }
            }
        } // left_pos == right_pos
        if(critical_points.empty()) {
            //means LCA(current_l, current_r is the only critical point
            critical_points.push_back(left_pos);
        }

        //calculate updated rev values in critical points TODO: Use compares in loop instead of vectors
        vector<tree_val_t> critical_min;
        vector<tree_val_t> critical_max;
        for(auto cp : critical_points) {
            auto update_val = tree_sum_to_root(delta, cp);
            critical_max.push_back(max_t[cp] + update_val);
            critical_min.push_back(min_t[cp] + update_val);
        }
        auto interval_min = *min_element(critical_min.begin(), critical_min.end());
        auto interval_max = *max_element(critical_max.begin(), critical_max.end());
        if(interval_max + current_delta > MAX_ALLOWED_REVENUE || interval_min + current_delta < MIN_ALLOWED_REVENUE) {
            //handle too big delta error
            cout << -1 << endl;
            continue;
        }
        // Update deltas in critical points and min/max in their parents (for correct comparison in future iterations)
        for(auto cp : critical_points) {
            delta[cp] += current_delta;
            auto parent_pos = tree_parent_pos(cp);
            auto lson_pos = tree_lson_pos(parent_pos);
            auto rson_pos = tree_rson_pos(parent_pos);
            min_t[parent_pos] = min((min_t[lson_pos] + delta[lson_pos]), (min_t[rson_pos] + delta[rson_pos]));
            tree_rebase_min(min_t, parent_pos);
            max_t[parent_pos] = max((max_t[lson_pos] + delta[lson_pos]), (max_t[rson_pos] + delta[rson_pos]));
            tree_rebase_max(max_t, parent_pos);
        }

        //check if delta affects growth_periods (only possible in rev[r] and rev[l+1])
        if(current_l >= 2) {
            tree_val_t real_l = rev[current_l] + tree_sum_to_root(delta, tree_pos(current_l-1));
            tree_val_t real_l_prev = real_l - current_delta;
            tree_val_t real_l_left = rev[current_l-1] + tree_sum_to_root(delta, tree_pos(current_l-2));
            if(real_l_prev > real_l_left && real_l <= real_l_left) {
                growth_periods--;
            }
            if(real_l_prev <= real_l_left && real_l > real_l_left) {
                growth_periods++;
            }
        }
        if(current_r <= n-1) {
            tree_val_t real_r = rev[current_r] + tree_sum_to_root(delta, tree_pos(current_r-1));
            tree_val_t real_r_prev = real_r - current_delta;
            tree_val_t real_r_right = rev[current_r+1] + tree_sum_to_root(delta, tree_pos(current_r));
            if(real_r_prev >= real_r_right && real_r < real_r_right) {
                growth_periods++;
            }
            if(real_r_prev < real_r_right && real_r >= real_r_right) {
                growth_periods--;
            }
        }
        cout << growth_periods << endl;
    }

    return 0;
}