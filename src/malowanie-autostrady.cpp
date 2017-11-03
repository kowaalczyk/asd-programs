#include <iostream>
#include <cmath>
#include <cassert>

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

enum state{
    BLACK = 0,
    WHITE = 1,
    UNKNOWN = 2
};

unsigned int tree[2000000];
state tree_state[2000000];
unsigned int tree_size;

// tree helpers
void debug_print_tree() {
//    comment-out for debugging:
//    return;

    unsigned int breaker = 0;
    unsigned int counter = 0;

    for(unsigned int i=1; i< tree_size; i++) {
        cout << tree[i] << ' ';
        counter++;
        if(counter >= (int)pow((float)2, (float)breaker)) {
            cout << endl;
            breaker++;
            counter = 0;
        }
    }
}

unsigned int tree_pos(unsigned int leaf_pos) {
    return tree_size/2 + leaf_pos;
}

unsigned int tree_parent_pos(unsigned int node_pos) {
    return node_pos/2;
}

unsigned int tree_lson_pos(unsigned int node_pos) {
    return node_pos*2;
}

unsigned int tree_rson_pos(unsigned int node_pos) {
    return 1 + node_pos*2;
}

// tree interface

void tree_reset() {
    for(int i=1; i<tree_size; i++) {
        tree[i] = 0;
        tree_state[i] = BLACK;
    }
}

void tree_create(unsigned int leaves) {
    tree_size = 2*leaves;
    tree_reset();
}

unsigned int tree_iterate(unsigned int current_pos, unsigned int target_pos) {
    assert(target_pos != current_pos);

    if(target_pos > current_pos) {
        return tree_rson_pos(current_pos);
    } else {
        return tree_lson_pos(current_pos);
    }
}

void tree_extend_state_to_children(unsigned int current_pos, state paint_state) {
    unsigned int paint_value = tree[current_pos];
    if (paint_state != UNKNOWN) {
        // extend paint state to children if it's known
        tree_state[tree_lson_pos(current_pos)] = paint_state;
        tree_state[tree_rson_pos(current_pos)] = paint_state;
        tree[tree_lson_pos(current_pos)] = paint_value;
        tree[tree_rson_pos(current_pos)] = paint_value;
    }
}

void tree_extend_state_on_path(unsigned int leaf_target) {
    unsigned int current_pos = 1;
    state paint_state = tree_state[1];
    unsigned int paint_value = tree[1];

    // go along path from root to target leaf and extend state to children of each node
    current_pos = tree_iterate(current_pos, leaf_target);
    while (current_pos != leaf_target) {
        state current_state = tree_state[current_pos];
        if(paint_state == UNKNOWN && current_state != UNKNOWN) {
            // define paint state if undefined
            paint_state = current_state;
        }
        tree_extend_state_to_children(current_pos, paint_state);
        current_pos = tree_iterate(current_pos, leaf_target);
    }
}

void tree_extend_state(unsigned int leaf_start, unsigned int leaf_end) {
    tree_extend_state_on_path(leaf_start);
    tree_extend_state_on_path(leaf_end);
}

void tree_set(unsigned int leaf_pos, unsigned int val) {
    // TODO
    unsigned int pos = tree_pos(leaf_pos);
    tree[pos] = val;
}

int main() {
    return 0;
}