#include <iostream>
#include <assert.h>

using namespace std;

/*
 * Wzbogacone drzewo przedziałowe:
 * oprócz trzymania sumy synów, w każdym wierzchołku trzymamy jeszcze
 *
 * W pzypadku naszego zadania:
 * W każdym węźle trzymamy ile jest białego w całej podautostradzie
 * - nie dałoby się tego aktualizować w czasie logarytmicznym:
 *      musielibyśmy dla aktualizacji każdego przedziału aktualizować wszystkie jego kawałki (liście)
 * - dlatego zamiast tego, spróbujmy trzymać informację:
 *      całe jest białe | całe jest czarne | nie wiadomo, zczytaj wartości z poddrzewa
 *      (w przypadku 1 i 2 opcji w poddrzewie są śmieci)
 * - wtedy zaznaczanie działa tak:
 *      1. na początku mamy czarne drzewo (korzeń: UNKNOWN)
 *      2. chcemy pomalować na biało od i do j:
 *      3. ustawiamy (korzeń: UNKNOWN)
 *      4. idziemy od góry do dołu, pamietamy informację BLACK | WHITE z poprzedniego wierzchołka
 *      5. jeżeli minęliśmy wierzchołek WHITE | BLACK to aktualizujemy wszystkie dzieci tego wierzchołka
 *         (czyli kolejne wierzchołki idąc w dół - operacja: zejść do syna zrzucając informację o kolorze)
 *         dzięki temu zmiana BLACK lub WHITE na UNKNOWN nie zaburzy nam systemu drzewa
 *         alternatywnie: jeżeli zmienam BLACK | WHITE na UNKNOWN to w momencie zmiany można zmieniać kolor
 *      6. kolorowanie przedziału i,j zaczynamy po kroku (5) od dołu:
 *         kolorujemy liście i,j i ich rodziców idąc do góry (dzięki (5) możemy czytać wartości z synów przy zmianie do UNKNOWN)
 * NZM: Idąc do lewego krańca, prawegych potomków kolorujemy na dany kolor jeśli to jest możliwe
 *      Idąc do prawego krańca - symetrycznie lewych potomkach
 *      Na koniec idziemy od góry i zliczamy sobie sumę
 *
 * Krótko mówiąc:
 * 1. Zrzucamy kolory przedziału z góry idąc po ścieżce (odkąd natrafimy na konkretny kolor)
 * 2. Rysjemy ścieżki z dołu
 * 3. Aktualizujemy sumę z góry
 *
 * Inicjujemy korzeń na czarny-0
 */


typedef long long tree_val_t;
typedef int tree_pos_t;

const tree_val_t black = 0;
const tree_val_t white = 1;
const tree_val_t unknown = 2;

// PROBLEM PARAMS --------------------------------------------------------------------------


// INTERVAL TREE PARAMS --------------------------------------------------------------------

static const tree_pos_t TREE_MAX_SIZE = 2097152; // == 2^21
static tree_pos_t tree_size;

tree_val_t state[TREE_MAX_SIZE];
tree_val_t exact_count[TREE_MAX_SIZE];

// INTERVAL TREE BASE -----------------------------------------------------------------------

tree_pos_t tree_root_pos() {
    return 1;
}

tree_pos_t tree_first_leaf_pos() {
    return tree_size/2;
}

tree_pos_t tree_pos(tree_pos_t leaf_pos) {
    return tree_first_leaf_pos() + leaf_pos;
}

tree_pos_t tree_last_leaf_pos() {
    return tree_size-1;
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

void update_interval_recursive(
        tree_pos_t current_pos,
        tree_pos_t current_left_bound,
        tree_pos_t current_right_bound,
        tree_pos_t left_bound,
        tree_pos_t right_bound,
        tree_val_t val) {
    assert(val == black || val == white);
    if(current_left_bound > current_right_bound || current_left_bound > right_bound || current_right_bound < left_bound) {
        // not where we want to be
        return;
    }
    auto current_state = state[current_pos];
    if(current_left_bound < current_right_bound && (current_state == white || current_state == black)) {
        // children exist and current state is determinate => drop state to children
        state[tree_lson_pos(current_pos)] = current_state;
        state[tree_rson_pos(current_pos)] = current_state;
        if(current_state == white) {
            // set children's exact_count
            tree_val_t subtree_range = (current_right_bound - current_left_bound + 1)/2;
            exact_count[tree_lson_pos(current_pos)] = subtree_range;
            exact_count[tree_rson_pos(current_pos)] = subtree_range;
        } else {
            exact_count[tree_lson_pos(current_pos)] = 0;
            exact_count[tree_rson_pos(current_pos)] = 0;
        }
    }
    if(current_left_bound >= left_bound && current_right_bound <= right_bound) {
        // critical point => update node
        state[current_pos] = val;
        exact_count[current_pos] = val==white ? current_right_bound - current_left_bound + 1 : 0;
        return;
    }
    tree_pos_t divider = (current_left_bound + current_right_bound) / 2;
    update_interval_recursive(tree_lson_pos(current_pos), current_left_bound, divider, left_bound, right_bound, val);
    update_interval_recursive(tree_rson_pos(current_pos), divider+1, current_right_bound, left_bound, right_bound, val);
    if(state[tree_lson_pos(current_pos)] == state[tree_rson_pos(current_pos)]) {
        // left and right subtrees have identical state - mark it for quicker reading in the future
        state[current_pos] = state[tree_lson_pos(current_pos)];
    } else {
        state[current_pos] = unknown;
    }
    exact_count[current_pos] = exact_count[tree_lson_pos(current_pos)] + exact_count[tree_rson_pos(current_pos)];
}

void update_interval(tree_pos_t l, tree_pos_t r, tree_val_t val) {
    update_interval_recursive(tree_root_pos(), tree_pos(0), tree_size-1, tree_pos(l-1), tree_pos(r-1), val);
}

// HELPERS ----------------------------------------------------------------------------------

tree_val_t white_segments() {
    return exact_count[tree_root_pos()];
}

// SOLUTION ---------------------------------------------------------------------------------

int main() {
    tree_pos_t n, m;
    cin >> n >> m;

    tree_create(state, n, black);
    tree_create(exact_count, n, 0);

    for(tree_pos_t i=0; i<m; i++) {
        tree_pos_t a, b;
        char c;
        cin >> a >> b >> c;
        if(c == 'C') {
            update_interval(a, b, black);
        } else {
            update_interval(a, b, white);
        }
        cout << white_segments() << endl;
    }
    return 0;
}
