//
// Created by kowal on 04.11.17.
// Usage:
// Include header and run!
//

#include <iostream>
#include "interval_tree.h"

using namespace std;

tree_val_t tree[TREE_MAX_SIZE];

// TEST ENGINE IMPLEMENTATION

static unsigned long long case_num = 0;
static string current_context = "";
static string current_case = "";

void assert_eq(unsigned long long real, unsigned long long expected) {
    if(expected != real) {
        cout << "assertion failed, got:" << real << " instead of: " << expected << endl;
        cout << "in case: " << case_num << " - " << current_case << endl;
        cout << endl;
        tree_print_debug(tree);
        exit(1);
    }
}

void start_context(string context) {
    current_context = context;
    cout << endl;
    cout << endl;
    cout << " --- " << current_context << " --- " << endl;
}

void start_case(string desc) {
    case_num++;
    cout << endl;
    cout << "Case #" << case_num << ": " << endl;
    cout << desc << endl;
    current_case = desc;
}

// END TEST ENGINE IMPLEMENTATION

int main() {
    start_context("n == 1");
    {
        unsigned long long n = 1;

        start_case("initialization to 0s");
        {
            tree_create(tree, n);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("empty tree can be reset");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("leaves start at tree_size/2");
        {
            assert_eq(tree_pos(0), tree_size()/2);
        }

        start_case("leaves end at tree_size/2 + (n-1)");
        {
            assert_eq(tree_pos(n-1), tree_size()/2 + n-1);
        }

        start_case("single value inserted correctly");
        {
            tree_reset(tree);
            tree_set(tree, 0, 2137);
            assert_eq(tree[tree_pos(0)], 2137);
        }

        start_case("tree is reset to 0s");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("root is equal to single inserted value");
        {
            tree_reset(tree);
            tree_set(tree, 0, 2137);
            assert_eq(tree[tree_root_pos()], 2137);
        }

        start_case("node value == sum of values from sons (where possible)");
        {
            tree_reset(tree);
            for(unsigned long long i= tree_root_pos(); tree_rson_pos(i)<tree_size(); i++) {
                assert_eq(tree[i], tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]);
            }
        }
    }

    start_context("n = 2^k leaves");
    {
        unsigned long long n = 256;

        start_case("initialization to 0s");
        {
            tree_create(tree, n);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("empty tree can be reset");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("leaves start at tree_size/2");
        {
            assert_eq(tree_pos(0), tree_size()/2);
        }

        start_case("leaves end at tree_size/2 + (n-1)");
        {
            assert_eq(tree_pos(n-1), tree_size()/2 + n-1);
        }

        start_case("single value inserted correctly");
        {
            tree_reset(tree);
            tree_set(tree, 0, 2137);
            assert_eq(tree[tree_pos(0)], 2137);
        }

        start_case("tree is reset to 0s");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("root is equal to single inserted value");
        {
            tree_reset(tree);
            tree_set(tree, 0, 2137);
            assert_eq(tree[tree_root_pos()], 2137);
        }

        start_case("root is equal to sum of multiple inserted values");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            assert_eq(tree[tree_root_pos()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("node value == sum of values from sons (where possible)");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i*i);
            }
            for(unsigned long long i= tree_root_pos(); tree_rson_pos(i)<tree_size(); i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(tree_leaf_sum(tree, i), ((i*(i-1))/2) MOD_BIL);
            }
        }

        n = 16384; //testing modulo calculations for large values
        tree_reset(tree);
        tree_create(tree, n);

        start_case("large values: root is equal to sum of multiple inserted values");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            assert_eq(tree[tree_root_pos()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("large values: node value == sum of values from sons (where possible)");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            for(unsigned long long i= tree_root_pos(); tree_rson_pos(i)<tree_size(); i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("large values: sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(tree_leaf_sum(tree, i), ((i*(i-1))/2) MOD_BIL);
            }
        }
    }

    start_context("n = 2^k - 1 leaves");
    {
        unsigned long long n = 15;

        start_case("initialization to 0s");
        {
            tree_create(tree, n);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("empty tree can be reset");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("leaves start at tree_size/2");
        {
            assert_eq(tree_pos(0), tree_size()/2);
        }

        start_case("leaves end at tree_size/2 + (n-1)");
        {
            assert_eq(tree_pos(n-1), tree_size()/2 + n-1);
        }

        start_case("single value inserted correctly");
        {
            tree_reset(tree);
            tree_set(tree, 0, 2137);
            assert_eq(tree[tree_pos(0)], 2137);
        }

        start_case("tree is reset to 0s");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("root is equal to single inserted value");
        {
            tree_reset(tree);
            tree_set(tree, 0, 2137);
            assert_eq(tree[tree_root_pos()], 2137);
        }

        start_case("root is equal to sum of multiple inserted values");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            assert_eq(tree[tree_root_pos()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("node value == sum of values from sons (where possible)");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            for(unsigned long long i= tree_root_pos(); tree_rson_pos(i)<tree_size(); i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(tree_leaf_sum(tree, i), ((i*(i-1))/2) MOD_BIL);
            }
        }

        n = 16383; //testing modulo calculations for large values
        tree_reset(tree);
        tree_create(tree, n);

        start_case("large values: root is equal to sum of multiple inserted values");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            assert_eq(tree[tree_root_pos()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("large values: node value == sum of values from sons (where possible)");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            for(unsigned long long i= tree_root_pos(); tree_rson_pos(i)<tree_size(); i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("large values: sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(tree_leaf_sum(tree, i), ((i*(i-1))/2) MOD_BIL);
            }
        }
    }

    start_context("n = 2^k + 1 leaves");
    {
        unsigned long long n = 257;

        start_case("initialization to 0s");
        {
            tree_create(tree, n);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("empty tree can be reset");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("leaves start at tree_size/2");
        {
            assert_eq(tree_pos(0), tree_size()/2);
        }

        start_case("leaves end at tree_size/2 + (n-1)");
        {
            assert_eq(tree_pos(n-1), tree_size()/2 + n-1);
        }

        start_case("single value inserted correctly");
        {
            tree_reset(tree);
            tree_set(tree, 0, 2137);
            assert_eq(tree[tree_pos(0)], 2137);
        }

        start_case("tree is reset to 0s");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("root is equal to single inserted value");
        {
            tree_reset(tree);
            tree_set(tree, 0, 2137);
            assert_eq(tree[tree_root_pos()], 2137);
        }

        start_case("root is equal to sum of multiple inserted values");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            assert_eq(tree[tree_root_pos()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("node value == sum of values from sons (where possible)");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i*i);
            }
            for(unsigned long long i= tree_root_pos(); tree_rson_pos(i)<tree_size(); i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(tree_leaf_sum(tree, i), ((i*(i-1))/2) MOD_BIL);
            }
        }

        n = 16385; //testing modulo calculations for large values
        tree_reset(tree);
        tree_create(tree, n);

        start_case("large values: root is equal to sum of multiple inserted values");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            assert_eq(tree[tree_root_pos()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("large values: node value == sum of values from sons (where possible)");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            for(unsigned long long i= tree_root_pos(); tree_rson_pos(i)<tree_size(); i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("large values: sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(tree_leaf_sum(tree, i), ((i*(i-1))/2) MOD_BIL);
            }
        }
    }

    start_context("n = (2^k + 2^(k+1)) / 2 leaves");
    {
        unsigned long long n = 192;

        start_case("initialization to 0s");
        {
            tree_create(tree, n);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("empty tree can be reset");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("leaves start at tree_size/2");
        {
            assert_eq(tree_pos(0), tree_size()/2);
        }

        start_case("leaves end at tree_size/2 + (n-1)");
        {
            assert_eq(tree_pos(n-1), tree_size()/2 + n-1);
        }

        start_case("single value inserted correctly");
        {
            tree_reset(tree);
            tree_set(tree, 0, 2137);
            assert_eq(tree[tree_pos(0)], 2137);
        }

        start_case("tree is reset to 0s");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("root is equal to single inserted value");
        {
            tree_reset(tree);
            tree_set(tree, 0, 2137);
            assert_eq(tree[tree_root_pos()], 2137);
        }

        start_case("root is equal to sum of multiple inserted values");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            assert_eq(tree[tree_root_pos()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("node value == sum of values from sons (where possible)");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i*i);
            }
            for(unsigned long long i= tree_root_pos(); tree_rson_pos(i)<tree_size(); i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(tree_leaf_sum(tree, i), ((i*(i-1))/2) MOD_BIL);
            }
            tree_print_debug(tree);
        }

        n = 12288; //testing modulo calculations for large values
        tree_reset(tree);
        tree_create(tree, n);

        start_case("large values: root is equal to sum of multiple inserted values");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            assert_eq(tree[tree_root_pos()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("large values: node value == sum of values from sons (where possible)");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            for(unsigned long long i= tree_root_pos(); tree_rson_pos(i)<tree_size(); i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("large values: sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset(tree);
            for(unsigned long long i=0; i<n; i++) {
                tree_set(tree, i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(tree_leaf_sum(tree, i), ((i*(i-1))/2) MOD_BIL);
            }
        }
    }

    return 0;
}