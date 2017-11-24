#include <iostream>
#include <cassert>

using namespace std;

// HELPERS

/// returns floor of log_2(x)
inline int log_floor(int x) {
    assert(x>0);
    int ans = 0;
    while (x >>= 1) ++ans;
    return ans;
}

/// returns 2^x
inline int pow_2(int x) {
    assert(x>=0);
    return (x > 0 ? 2<<(x-1) : 1);
}

// MAIN FUNCTIONS AND STRUCTS

struct Node {
    int parent; //for traversal
    int left; //for traversal
    int right; //for traversal
    int level; //calc in bfs1
    int down_range; //calc in bfs1
    int down_range_length; //calc in bfs1
    int up_range; //calc in bfs2
    int up_range_length; //calc in bfs2
};

const int START_NODE_ID = 1;
Node tree[500001];
int hop[20][500001]; // hop[i][v] := 2^i-th parent of node v

/// use calc_levels
void calc_levels_recursive(int current_node_id, int level) {
    assert(current_node_id != -1);
    Node *current_node = &tree[current_node_id];
    current_node->level = level;
    if(current_node->left != -1) {
        calc_levels_recursive(current_node->left, level+1);
    }
    if(current_node->right != -1) {
        calc_levels_recursive(current_node->right, level+1);
    }
}

/// calculates levels(depth) for all nodes in a tree
void calc_levels() {
    calc_levels_recursive(START_NODE_ID, 0);
}

/// returns id of ancestor found by hopping up in O(log(n))
int find_ancestor(int node_id, int h, int total_nodes) {
    int ans = node_id;
    int i = log_floor(total_nodes);
    while(h > 0) {
        if(pow_2(i) > h) {
            i -= 1;
        } else {
            ans = hop[i][ans];
            h -= pow_2(i);
        }
    }
    assert(ans >= START_NODE_ID && ans <= total_nodes);
    return ans;
}

/// returns id of lowest common ancestor for given nodes
int lca(int node_1_id, int node_2_id, int total_nodes) {
    int level_1 = tree[node_1_id].level;
    int level_2 = tree[node_2_id].level;

    if(level_1 < level_2) {
        node_2_id = find_ancestor(node_2_id, level_2-level_1, total_nodes);
        level_2 = tree[node_2_id].level;
    } else if(level_1 > level_2) {
        node_1_id = find_ancestor(node_1_id, level_1-level_2, total_nodes);
        level_1 = tree[node_1_id].level;
    }
    assert(level_1 == level_2); //TODO: Failed (3 == 0) on
    if(node_1_id == node_2_id) {
        return node_1_id;
    }
    for(int i = log_floor(total_nodes); i >= 0; i--) {
        if(hop[i][node_1_id] != hop[i][node_2_id]) {
            node_1_id = hop[i][node_1_id];
            node_2_id = hop[i][node_2_id];
        }
    }
    return tree[node_1_id].parent;
}

/// sets range_down for given node, assuming the value was calculated for its children
void set_range_down(int node_id) {
    auto current_node = &tree[node_id];
    int ans_node_id = node_id;
    int ans_node_range = 0;
    if(current_node->left != -1) {
        auto left_node = tree[current_node->left];
        // check furthest down in left subtree
        int left_node_range = left_node.down_range_length;
        if(left_node_range+1 > ans_node_range) {
            ans_node_id = left_node.down_range;
            ans_node_range = left_node_range+1;
        }
    }
    if(current_node->right != -1) {
        // check furthest down in right subtree
        auto right_node = tree[current_node->right];
        int right_node_range = right_node.down_range_length;
        if(right_node_range+1 > ans_node_range) {
            ans_node_id = right_node.down_range;
            ans_node_range = right_node_range+1;
        }
    }
    current_node->down_range = ans_node_id;
    current_node->down_range_length = ans_node_range;
}

/// calculates range down for each node by iterating over the tree post-order
void calc_range_down_recursive(int current_node_id) {
    assert(current_node_id != -1);
    Node *current_node = &tree[current_node_id];
    if(current_node->left != -1) {
        calc_range_down_recursive(current_node->left);
    }
    if(current_node->right != -1) {
        calc_range_down_recursive(current_node->right);
    }
    set_range_down(current_node_id);
}

/// sets range_up for given node, assuming range_up was calculated for parent and range_down was calculated for sibling
void set_range_up(int node_id) {
    auto current_node = &tree[node_id];
    int ans_node_id = node_id;
    int ans_node_range = 0;
    if(current_node->parent != -1) {
        // check furthest up in parent
        auto parent_node = &tree[current_node->parent];
        int parent_node_range = parent_node->up_range_length;
        if(parent_node_range+1 > ans_node_range) {
            ans_node_id = parent_node->up_range;
            ans_node_range = parent_node_range+1;
        }
        int sibling_id = parent_node->left == node_id ? parent_node->right : parent_node->left;
        if(sibling_id != -1 && tree[sibling_id].down_range_length+2 > ans_node_range) {
            // check furthest down in sibling if it exists
            ans_node_id = tree[sibling_id].down_range;
            ans_node_range = tree[sibling_id].down_range_length+2;
        }
    }
    current_node->up_range = ans_node_id;
    current_node->up_range_length = ans_node_range;
}

/// calculates range up for each node by iterating over the tree pre-order
void calc_range_up_recursive(int current_node_id) {
    assert(current_node_id != -1);
    set_range_up(current_node_id);
    auto current_node = &tree[current_node_id];
    if(current_node->left != -1) {
        calc_range_up_recursive(current_node->left);
    }
    if(current_node->right != -1) {
        calc_range_up_recursive(current_node->right);
    }
}

/// calculates hops for all nodes
void calc_hops(int total_nodes) {
    // fill array with -1s
    for(int dist = 0; dist < 20; dist++) {
        for (int v = 1; v <= total_nodes; v++) {
            hop[dist][v] = -1;
        }
    }
    // calculate hops
    for(int dist = 0; dist < 20; dist++) {
        for(int v = 1; v <= total_nodes; v++) {
            if(dist == 0) {
                hop[dist][v] = tree[v].parent;
            } else if(hop[dist-1][v] != -1) {
                hop[dist][v] = hop[dist-1][hop[dist-1][v]];
            } else {
                hop[dist][v] = -1;
            }
        }
    }
}

/// returns id of any node d edges away from v
int find_node(int v, int d, int total_nodes) {
    assert(v != -1);
    auto current_node = tree[v];
    int max_length = max(current_node.down_range_length, current_node.up_range_length);
    if(max_length < d) {
        return -1;
    }
    auto max_node = max_length==current_node.down_range_length ? current_node.down_range : current_node.up_range;
    int lca_node_id = lca(v, max_node, total_nodes);
    if(d <= current_node.level - tree[lca_node_id].level) {
        return find_ancestor(v, d, total_nodes);
    } else {
        return find_ancestor(max_node, max_length - d, total_nodes);
    }
}

int main() {
    //load input
    int n;
    cin >> n;
    for(int i=1; i<=n; i++) {
        int l_path, r_path;
        cin >> l_path >> r_path;
        tree[i].left = l_path;
        tree[i].right = r_path;
        // (-1) means there is no path, START_NODE_ID==1 is a root
        if(i == START_NODE_ID) {
            tree[i].parent = -1;
        }
        if(l_path != -1) {
            tree[l_path].parent = i;
        }
        if(r_path != -1) {
            tree[r_path].parent = i;
        }
    }
    int m;
    cin >> m;
    // pre-process as much as possible
    calc_levels();
    calc_hops(n);
    calc_range_down_recursive(START_NODE_ID);
    calc_range_up_recursive(START_NODE_ID);
    // process each request in O(log(n))
    for(int i=0; i<m; i++) {
        int a, d;
        cin >> a >> d;
        cout << find_node(a, d, n) << endl;
    }

    return 0;
}
