/*
 * TODO
 * Zadanie ze słownikami:
 * - słownik podsłów bazowych - struktura pamiętająca kolejność podsłów występujących na wejściu
 * - sprawdzamy pozycje danych słów w słowniku i porównujemy ich pierwsze litery (???)
 *
 * Preprocessing i zwracanie wyników W CZASIE STAŁYM
 */

#include <iostream>
#include <cassert>

using namespace std;

const int N_MAX = 300000;
const int N_LOG_MAX = 20; // more than enough

string word;
int order[N_MAX][N_LOG_MAX];

/// compares substring word[a..b] with word[c..d], returns '<', '>' or '='
char lex_compare(int a, int b, int c, int d) {
    // TODO

    return 'x';
}

int main() {
    ios_base::sync_with_stdio(false);

    // init order array
    for (auto &i : order) {
        for (int &j : i) {
            j = 0;
        }
    }
    // load word
    int n, m;
    cin >> n >> m;
    // TODO: Make sure loading to string is fine
    cin >> word;
    assert(word.length() == n);

    // TODO: Pre-process word

    // load queries
    for(int i=0; i<m; i++) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
         cout << lex_compare(a, b, c, d) << endl;
    }
    return 0;
}