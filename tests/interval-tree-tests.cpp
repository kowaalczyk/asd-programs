//
// Created by kowal on 04.11.17.
// Usage:
// 0. read code requirements below
// 1. paste Your code anywhere before main()
// 2. compile and run
//

#include <iostream>
#define MOD_BIL %1000000000

using namespace std;

// NOTE: Code should implement following methods / constants:
// Example implementation:
// assuming 1 is the index of root, [tree_size/2...tree_size-1] are leaves
// (tree[0] is just a guard preventing overflow in loops)
static const unsigned long long TREE_MAX_SIZE = 160000;
static const int N_MAX_SIZE = 20000;
unsigned long long tree[TREE_MAX_SIZE];
unsigned long long tree_size;
void debug_print_tree();
unsigned long long tree_root();
unsigned long long tree_pos(unsigned long long leaf_pos);
unsigned long long tree_parent_pos(unsigned long long node_pos);
unsigned long long tree_lson_pos(unsigned long long node_pos);
unsigned long long tree_rson_pos(unsigned long long node_pos);
void tree_update_parents(unsigned long long node_pos);
void tree_reset();
void tree_create(unsigned long long leaves);
void tree_set(unsigned long long leaf_pos, unsigned long long val);
unsigned long long leaf_sum(unsigned long long leaf_pos); // for leaf_pos j, this will return sum of values from leaves at positions [0..j-1]

// example tree printer:
void debug_print_tree() {
    int breaker = 1;
    int counter = 0;

    for(unsigned long long i=1; i < tree_size; i++) {
        cout << tree[i] << ' ';
        counter++;
        if(counter >= breaker) {
            cout << endl;
            breaker*=2;
            counter = 0;
        }
    }
}

// TEST ENGINE IMPLEMENTATION

static unsigned long long case_num = 0;
static string current_context = "";
static string current_case = "";

void assert_eq(unsigned long long real, unsigned long long expected) {
    if(expected != real) {
        cout << "assertion failed, got:" << real << " instead of: " << expected << endl;
        cout << "in case: " << case_num << " - " << current_case << endl;
        cout << endl;
        debug_print_tree();
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
            tree_create(n);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("empty tree can be reset");
        {
            tree_reset();
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("leaves start at tree_size/2");
        {
            assert_eq(tree_pos(0), tree_size/2);
        }

        start_case("leaves end at tree_size/2 + (n-1)");
        {
            assert_eq(tree_pos(n-1), tree_size/2 + n-1);
        }

        start_case("single value inserted correctly");
        {
            tree_reset();
            tree_set(0, 2137);
            assert_eq(tree[tree_pos(0)], 2137);
        }

        start_case("tree is reset to 0s");
        {
            tree_reset();
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("root is equal to single inserted value");
        {
            tree_reset();
            tree_set(0, 2137);
            assert_eq(tree[tree_root()], 2137);
        }

        start_case("node value == sum of values from sons (where possible)");
        {
            tree_reset();
            for(unsigned long long i=tree_root(); tree_rson_pos(i)<tree_size; i++) {
                assert_eq(tree[i], tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]);
            }
        }
    }

    start_context("n = 2^k leaves");
    {
        unsigned long long n = 256;

        start_case("initialization to 0s");
        {
            tree_create(n);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("empty tree can be reset");
        {
            tree_reset();
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("leaves start at tree_size/2");
        {
            assert_eq(tree_pos(0), tree_size/2);
        }

        start_case("leaves end at tree_size/2 + (n-1)");
        {
            assert_eq(tree_pos(n-1), tree_size/2 + n-1);
        }

        start_case("single value inserted correctly");
        {
            tree_reset();
            tree_set(0, 2137);
            assert_eq(tree[tree_pos(0)], 2137);
        }

        start_case("tree is reset to 0s");
        {
            tree_reset();
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("root is equal to single inserted value");
        {
            tree_reset();
            tree_set(0, 2137);
            assert_eq(tree[tree_root()], 2137);
        }

        start_case("root is equal to sum of multiple inserted values");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            assert_eq(tree[tree_root()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("node value == sum of values from sons (where possible)");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i*i);
            }
            for(unsigned long long i=tree_root(); tree_rson_pos(i)<tree_size; i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(leaf_sum(i), ((i*(i-1))/2) MOD_BIL);
            }
        }

        n = 16384; //testing modulo calculations for large values
        tree_reset();
        tree_create(n);

        start_case("large values: root is equal to sum of multiple inserted values");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            assert_eq(tree[tree_root()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("large values: node value == sum of values from sons (where possible)");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            for(unsigned long long i=tree_root(); tree_rson_pos(i)<tree_size; i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("large values: sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(leaf_sum(i), ((i*(i-1))/2) MOD_BIL);
            }
        }
    }

    start_context("n = 2^k - 1 leaves");
    {
        unsigned long long n = 15;

        start_case("initialization to 0s");
        {
            tree_create(n);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("empty tree can be reset");
        {
            tree_reset();
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("leaves start at tree_size/2");
        {
            assert_eq(tree_pos(0), tree_size/2);
        }

        start_case("leaves end at tree_size/2 + (n-1)");
        {
            assert_eq(tree_pos(n-1), tree_size/2 + n-1);
        }

        start_case("single value inserted correctly");
        {
            tree_reset();
            tree_set(0, 2137);
            assert_eq(tree[tree_pos(0)], 2137);
        }

        start_case("tree is reset to 0s");
        {
            tree_reset();
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("root is equal to single inserted value");
        {
            tree_reset();
            tree_set(0, 2137);
            assert_eq(tree[tree_root()], 2137);
        }

        start_case("root is equal to sum of multiple inserted values");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            assert_eq(tree[tree_root()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("node value == sum of values from sons (where possible)");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            for(unsigned long long i=tree_root(); tree_rson_pos(i)<tree_size; i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(leaf_sum(i), ((i*(i-1))/2) MOD_BIL);
            }
        }

        n = 16383; //testing modulo calculations for large values
        tree_reset();
        tree_create(n);

        start_case("large values: root is equal to sum of multiple inserted values");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            assert_eq(tree[tree_root()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("large values: node value == sum of values from sons (where possible)");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            for(unsigned long long i=tree_root(); tree_rson_pos(i)<tree_size; i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("large values: sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(leaf_sum(i), ((i*(i-1))/2) MOD_BIL);
            }
        }
    }

    start_context("n = 2^k + 1 leaves");
    {
        unsigned long long n = 257;

        start_case("initialization to 0s");
        {
            tree_create(n);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("empty tree can be reset");
        {
            tree_reset();
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("leaves start at tree_size/2");
        {
            assert_eq(tree_pos(0), tree_size/2);
        }

        start_case("leaves end at tree_size/2 + (n-1)");
        {
            assert_eq(tree_pos(n-1), tree_size/2 + n-1);
        }

        start_case("single value inserted correctly");
        {
            tree_reset();
            tree_set(0, 2137);
            assert_eq(tree[tree_pos(0)], 2137);
        }

        start_case("tree is reset to 0s");
        {
            tree_reset();
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("root is equal to single inserted value");
        {
            tree_reset();
            tree_set(0, 2137);
            assert_eq(tree[tree_root()], 2137);
        }

        start_case("root is equal to sum of multiple inserted values");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            assert_eq(tree[tree_root()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("node value == sum of values from sons (where possible)");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i*i);
            }
            for(unsigned long long i=tree_root(); tree_rson_pos(i)<tree_size; i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(leaf_sum(i), ((i*(i-1))/2) MOD_BIL);
            }
        }

        n = 16385; //testing modulo calculations for large values
        tree_reset();
        tree_create(n);

        start_case("large values: root is equal to sum of multiple inserted values");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            assert_eq(tree[tree_root()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("large values: node value == sum of values from sons (where possible)");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            for(unsigned long long i=tree_root(); tree_rson_pos(i)<tree_size; i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("large values: sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(leaf_sum(i), ((i*(i-1))/2) MOD_BIL);
            }
        }
    }

    start_context("n = (2^k + 2^(k+1)) / 2 leaves");
    {
        unsigned long long n = 192;

        start_case("initialization to 0s");
        {
            tree_create(n);
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("empty tree can be reset");
        {
            tree_reset();
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("leaves start at tree_size/2");
        {
            assert_eq(tree_pos(0), tree_size/2);
        }

        start_case("leaves end at tree_size/2 + (n-1)");
        {
            assert_eq(tree_pos(n-1), tree_size/2 + n-1);
        }

        start_case("single value inserted correctly");
        {
            tree_reset();
            tree_set(0, 2137);
            assert_eq(tree[tree_pos(0)], 2137);
        }

        start_case("tree is reset to 0s");
        {
            tree_reset();
            for(unsigned long long i=0; i<TREE_MAX_SIZE; i++) {
                assert_eq(tree[i], 0);
            }
        }

        start_case("root is equal to single inserted value");
        {
            tree_reset();
            tree_set(0, 2137);
            assert_eq(tree[tree_root()], 2137);
        }

        start_case("root is equal to sum of multiple inserted values");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            assert_eq(tree[tree_root()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("node value == sum of values from sons (where possible)");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i*i);
            }
            for(unsigned long long i=tree_root(); tree_rson_pos(i)<tree_size; i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(leaf_sum(i), ((i*(i-1))/2) MOD_BIL);
            }
            debug_print_tree();
        }

        n = 12288; //testing modulo calculations for large values
        tree_reset();
        tree_create(n);

        start_case("large values: root is equal to sum of multiple inserted values");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            assert_eq(tree[tree_root()], ((n*(n-1))/2) MOD_BIL);
        }

        start_case("large values: node value == sum of values from sons (where possible)");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            for(unsigned long long i=tree_root(); tree_rson_pos(i)<tree_size; i++) {
                assert_eq(tree[i], (tree[tree_lson_pos(i)] + tree[tree_rson_pos(i)]) MOD_BIL);
            }
        }

        start_case("large values: sum of smaller values is equal to sum calculated in O(log(n))");
        {
            tree_reset();
            for(unsigned long long i=0; i<n; i++) {
                tree_set(i, i);
            }
            for(unsigned long long i=0; i<n; i++) {
                assert_eq(leaf_sum(i), ((i*(i-1))/2) MOD_BIL);
            }
        }
    }

    return 0;
}