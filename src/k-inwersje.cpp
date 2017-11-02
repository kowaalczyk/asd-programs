#include <iostream>
#include <cmath>

using namespace std;

/* dynamicznie:
 * chcemy dla każdego elementu spamiętywać ile zaczyna się w nim k-1 inwersji
 * wtedy możemy przelecieć po elementach tablicy i policzyć mniejsze elemnety
 * na odpowiednich pozycjach w czasie O(k*n^2)
 *
 * chcemy jednak mieć czas O(n*k*log(n)):
 * tworzymy sobie drzewo przdziałowe które pamięta
 * liczbę elemntów mniejszych od elementu na danej pozycji
 *
 * drzewo przedziałowe:
 * - zmiana wartoći w węźle w czasie O(logn)
 * - implementacja w tablicy mającej 2n-1 elementów,
 * - przechodzenie podobnie jak kopiec, ale lepiej indeksować indeksy od liści
 *
 * możemy liczyć sumę od k do j:
 * - policzyć sumę [0..k] i [0..j] i odjąć
 * - sumę od zera do j liczymy idąc do góry z j liścia
 *   i dodając wartość poddrzewa przy przejsciu w lewo
 *
 * zaczynamy od drzewa z jedynkami w liściach
 * indeks liścia drzewa := wartość w tablicy danych
 * w liściach drzewa przedziałowego pamiętamy k-1 inwersje
 * (pozycja liścia oznacza pozycję pierwszego elementu inwersji)
 * - chcemy policzyć ile jest k inwersji:
 * - n razy pytamy się ile wynosi liczba elementów mniejszych niż dany
 * - n razy chcemy ustawić element na zero (nie liczymy danych elementów 2 razy)
 * - drzewo można zbudować w czasie n*log(n)
 * - powyższe kroki powtarzamy k razy
 *
 * W każdym momencie potrzeba tylko 1 drzewo, ale rozwiązanie z 2 drzewami też jest ok
 * */

// drzewo przedziałowe
// assuming 1 is the index of root, [tree_size/2...tree_size-1] are leaves
int tree[40000];
int tree_size;

// helper functions

void debug_print_tree() {
//    comment-out for debugging:
//    return;

    int breaker = 0;
    int counter = 0;

    for(int i=1; i< tree_size; i++) {
        cout << tree[i] << ' ';
        counter++;
        if(counter >= (int)pow((float)2, (float)breaker)) {
            cout << endl;
            breaker++;
            counter = 0;
        }
    }
}

int tree_pos(int leaf_pos) {
    return tree_size/2 + leaf_pos;
}

int tree_parent_pos(int node_pos) {
    return node_pos/2;
}

int tree_lson_pos(int node_pos) {
    return node_pos*2;
}

int tree_rson_pos(int node_pos) {
    return 1 + node_pos*2;
}

// tree interface
void tree_update_parents(int node_pos) {
    if(node_pos > 1) {
        int parent_pos = tree_parent_pos(node_pos);
        tree[parent_pos] = tree[tree_lson_pos(parent_pos)] + tree[tree_rson_pos(parent_pos)];
        tree_update_parents(parent_pos);
    }
}

void tree_reset() {
    for(int i=tree_pos(0); i<=tree_pos(tree_size/2); i++) {
        tree[i] = 1;
//        TODO: Do it in O(n) - update parents after the loop
        tree_update_parents(i);
    }
}

void tree_create(int leaves) {
    tree_size = 2*leaves;
    tree_reset();
}

void tree_set(int leaf_pos, int val) {
    int pos = tree_pos(leaf_pos);
    tree[pos] = val;
    tree_update_parents(pos);
}

// for leaf j, this will return sum of values from leaves [0..j]
int leaf_sum(int leaf_pos) {
    // init the sum with leaf value
    int pos = tree_pos(leaf_pos);
    int prev_pos = -1;
    int sum = 0;

    prev_pos = pos;
    pos = tree_parent_pos(pos);
    while(pos>=1) {
        // add sum from left subtree if we have moved to the left
        if(prev_pos != tree_lson_pos(pos)) {
            sum += tree[tree_lson_pos(pos)];
        }

        prev_pos = pos;
        pos = tree_parent_pos(pos);
    }
    return sum;
}

int main() {
    int n, k;
    int tab[20000];

//    TODO: Uważać na indeksowanie od 1 w treści zadania
//    TODO: Wszystkie operacje mod 10^9

    cin >> n >> k;
    for(int i=0; i<n; i++) {
        cin >> tab[i];
    }

    tree_create(n);
//    tree_reset();

    debug_print_tree();

    for(int current_k=2; current_k<=k; current_k++) {
//        cout << "current_k = " << current_k << endl;
        int new_leaves[20000];

        debug_print_tree();

        for(int i=n-1; i>=0; i--) {
            int val = tab[i];
            new_leaves[val-1] = leaf_sum(val-1);
//            tree_set(val-1, 0);
        }
        for(int i=0; i<n; i++) {
//            cout << new_leaves[i] << ' ';
            tree_set(i, new_leaves[i]);
        }
//        cout << endl;
    }

    cout << tree[1] << endl;

    return 0;
}