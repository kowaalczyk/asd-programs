#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

/*
 * Dostajemy gigantyczne drzewo binarne
 * chcemy dla danego wierzchołka znajdować wierzchołek oddalony od niego o daną odległość.
 * BFS jest za wolny, chcemy chodzić szybciej niż liniowo
 *
 * Trick1;
 * Część naszych problemów rozwiązuje pamiętanie dla każdego wierzchołka jaki jest od niego najbardziej oddalony:
 * - (-1) możemy zwracać od razu
 * - wpp możemy ograniczyć się tylko do ścieżki pomiędzy tym wierzchołkiem a danym:
 *  a) najdalszy jest moim synem
 *  b) najdalszy jest synem któregoś z moich przodków
 *
 *  Rozwiązanie:
 *  1x BFS do policzenia najbardziej oddalonego w lewo i w prawo (synów) --> preprocessing O(n)
 *  czy uda nam się policzyć najbardziej oddalony w górę?
 *      Nie do końca:
 *      znowu idziemy BFS-em i sprawdzamy jakie są najbardziej oddalone od ojca w przeciwną stronę niż my
 *      i najbardziej oddalony w górę -> porównujemy te 2 wielkości no i mamy nasz najdalszy w górę
 *  Efekt:
 *  mamy już (-1) w czasie stałym, przy 6 pomocniczych tablicach
 *  mamy też 3 możliwości pozycji najbardziej oddalonego:
 *  a) wspólny przodek
 *  b) pod nami
 *  c) nad nami
 *  Będziemy chcieli skorzytać z tego gdzie jest najdalszy żeby to obliczyć:
 *  - możemy pamiętać jeszcze poziom każdego wierzchołka i pamiętać wspólnego przodka z między nim a najdalszym
 *  - możemy też mieć strukture która pozwala nam skakać:
 *      chcemy mieć odnośniki do wierzchołków odległych o 2,4,8,16,32,itd (wtedy mamy O(log(n)))
 *      tablica n x log(n) mieści się w pamięci (dla intów ok. 40MB)
 *      budujemy ją w czasie O(nlog(n)) po BFS-ach, przy jej budowaniu korzystamy sobie z poprzednich wartości
 *      (dla danego wierzchołka przy obliczniu 4 korzystamy z tego że mamy obliczone 2)
 *      --> zaczynamy dla policzenia odległości o 2 od każdego z wierzchłków, potem 4 itd.
 *  - ponieważ mamy głębokość i odległość (dla nas i najdalszego wierzchołka) więc możemy ocenić które z a/b/c zachodzi
 */

/*
 * Takie structy trzymamy sobie w tablicy i w bfs-ie tylko używamy wskaźników
 */

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
    return (x > 0 ? 2<<x : 1);
}

// MAIN FUNCTIONS AND STRUCTS

struct Node {
    int parent; //for traversal
    int left; //for traversal
    int right; //for traversal
    int down_range; //calc in bfs1
    int down_range_length; //calc in bfs1
    int up_range; //calc in bfs2
    int up_range_length; //calc in bfs2
    int level; //calc in bfs
};

const int START_NODE_ID = 1;
Node tree[500001];
int hop[20][500001]; // hop[i][v] := 2^i-th parent of node v

// TODO: Calculate hops

/// returns id of ancestor found by hopping up in O(log(n))
int find_ancestor(int v, int h) {
    int ans = v;
    int i = log_floor(v);
    while(h > 0) {
        if(pow_2(i) > h) {
            i -= 1;
        } else {
            ans = hop[i][ans];
            h -= pow_2(i);
        }
    }
    return ans;
}

/// returns id of lowest common ancestor for nodes u and v, n is the number of all nodes in graph
int lca(int u, int v, int n) {
    int lu = tree[u].level;
    int lv = tree[v].level;

    if(lu < lv) {
        v = find_ancestor(v, lv-lu);
        lv = tree[v].level;
    } else if(lu > lv) {
        u = find_ancestor(u, lu-lv);
        lu = tree[u].level;
    }
    assert(lu == lv);
    if(u == v) {
        return u;
    }
    for(int i = log_floor(n); i >= 0; i--) {
        if(hop[i][u] != hop[i][v]) {
            u = hop[i][u];
            v = hop[i][v];
        }
    }
    return u;
}

void set_range_down(int node_id) {
    auto current_node = &tree[node_id];
    int ans_node_id = node_id;
    int ans_node_range = 0;
    if(current_node->left != -1) {
        auto left_node = tree[current_node->left];
        // check furthest down in left subtree
        int left_node_range = left_node.down_range;
        if(left_node_range+1 > ans_node_range) {
            ans_node_id = left_node.down_range;
            ans_node_range = left_node_range+1;
        }
    }
    if(current_node->right != -1) {
        // check furthest down in right subtree
        auto right_node = tree[current_node->right];
        int right_node_range = right_node.down_range;
        if(right_node_range+1 > ans_node_range) {
            ans_node_id = right_node.down_range;
            ans_node_range = right_node_range+1;
        }
    }
    current_node->down_range = ans_node_id;
    current_node->down_range_length = ans_node_range;
}

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

void set_range_up(int node_id) {
    auto current_node = &tree[node_id];
    int ans_node_id = node_id;
    int ans_node_range = 0;
    if(current_node->parent != -1) {
        // check furthest up in parent
        auto parent_node = &tree[current_node->parent];
        int parent_node_range = parent_node->up_range;
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

void calc_ranges() {
    calc_range_down_recursive(START_NODE_ID);
    calc_range_up_recursive(START_NODE_ID);
}

void calc_hops() {
    // TODO
}

/// returns id of any node d edges away from v
int find_node(int v, int d) {
    // TODO
}

int main() {
    int n;
    cin >> n;

    //1 is a root
    for(int i=1; i<=n; i++) {
        int l_path, r_path;

        cin >> l_path >> r_path;

        tree[i].left = l_path;
        tree[i].right = r_path;
        // (-1) means there is no path

        if(l_path > 0) {
            tree[l_path].parent = i;
        }
        if(r_path > 0) {
            tree[r_path].parent = i;
        }
    }

    //bfs1 TODO: Ogarnąć od tego miejsca, znaleźć fajne implementacje BFS/DFS
    vector<int> current_nodes;
//    current_nodes.push_back(0);
//    while(!current_nodes.empty()) {
//        int i = current_nodes.front();
//        current_nodes.erase(current_nodes.begin());
//
//    }

    int m;
    cin >> m;
    for(int i=0; i<m; i++) {
        int a, d;
        cin >> a >> d;
        cout << find_node(a,d) << endl;
    }

    return 0;
}
