/*
 * Próbujemy w sprytny sposób zrobić rekonstrukcję ciągów które mogłyby się do danego przesortować:
 * 1. bierzemy ostatni element
 * 2. Przykładowo dla 562143:
 *      - zakładamy pewną liczbę która była na końcu, np.4
 *          - dopisujemy liczby wg. algorytmu sortowania
 *          - zostajemy na koniec z 4 możliwościami: 52, 25, 12, 21
 *      - powtarzamy dla pozostałych możliwych cyfr na końcu
 *      - gdy wybierzemy sobie 3, zostajemy ponownie z 4 na ustalonym miejscu i cyframi 1,2,5 do wstawienia
 *          - ponieważ zastanawiamy się ile możliwych ciągów moglibyśmy uzyskać (nie interesują nas konkretne ciągi tylko ich liczba)
 *          - możemy zapamiętywać te liczby (konkretnie: ile prefixów tworzy dany przedział w danym ciągu) dynamicznie (np. wracając) -> pamięć n^2 czyli OK, czas działania: n^2
 *          - to jest optymalne rozwiązanie ale trudne w implementacji bez boosta
 *          - prościej jest zacząć samemu budować cache:
 *              - najpierw zapamiętać wszystkie 1-elementowe przedziały i stwierdzić że będą powstawały tylko z 1-elementowych prefixów
 *              - dla 2-el. mamy kończący się z lewej strony i kończący się z prawej strony
 *              - dla n liczbowych: z ilu prefixów można utworzyć [i, ..., n+i-1]? Trzeba spojrzeć na 2 ostatnie przedziały, zobaczyć które z nich sa możliwe w danej sytuacji i zsumować możliwe liczby
 * 3. Wszystkie działania robimy mod 10^9
 *
 * -- Dynamiczne rozwiązanie dla 562143 - w liniowej pamięci:
 * - a[i+1] = {jeśli pierwszy element z poprzednich jest mniejszy od bieżącego el}
 * - mamy 5 idących w prawo i 5 idących w lewo przedziałów 2-elementowych
 * - w prawą 31, 64 nie mogą powstać - cache: 0 1 1 1 0
 * - w lewą: 0 1 1 1 0
 * - 3-liczbowe: >>: 0, 2, 2, 0 (4 nie może być ani po 6 ani po 5) <<: 0, 2, 2(może być po 5 i po 6 więc sumujemy obie 1), 0(nie może być po 4, a po pozostałej cyfrze wychodz 0)
 * - itd.
 */

#include <iostream>

using namespace std;

int main() {
    unsigned int tab[1000];
    unsigned int n;
    cin >> n;
    for(unsigned int i=0; i<n; i++) {
        cin>>tab[i];
    }

    unsigned int l_cache[1000][1000]; //[length-1][first_el_pos]
    unsigned int r_cache[1000][1000]; //[length-1][last_el_pos]

    //calculate cache values for 2-sequences
    r_cache[1][0] = 0;
    l_cache[1][n-1] = 0;
    for(unsigned int i=0; i<n-1; i++) {
        if(tab[i] < tab[i+1]) {
            r_cache[1][i+1] = 1;
        } else {
            r_cache[1][i+1] = 0;
        }
        if(tab[i] < tab[i+1]) {
            l_cache[1][i] = 1;
        } else {
            l_cache[1][i] = 0;
        }
    }

    cout << "l_cache:";
    for(int i=0; i<n; i++) { cout << l_cache[1][i] << " "; }
    cout << endl;

    cout << "r_cache:";
    for(int i=0; i<n; i++) { cout << r_cache[1][i] << " "; }
    cout << endl;

    //dynamically calculate values for longer sequences
    for(unsigned int length_index = 2; length_index < n; length_index++) {
        for(unsigned int start = 0; start+length_index < n; start++) {
            unsigned int end = start + length_index;

            cout << "start:" << start << " end:" << end << " length_index:" << length_index << endl; // DEBUG

            unsigned int current_r = 0;
            if(tab[end-1] < tab[end]) {
                current_r += r_cache[length_index-1][end-1];
            }
            if(tab[start] < tab[end]) {
                current_r += l_cache[length_index-1][start];
            }
            r_cache[length_index][end] = current_r;

            unsigned int current_l = 0;
            if(tab[start] < tab[start+1]) {
                current_l += l_cache[length_index-1][start+1];
            }
            if(tab[start] < tab[end]) {
                current_l += r_cache[length_index-1][end];
            }
            l_cache[length_index][start] = current_l;

            cout << "l_cache:";
            for(int i=0; i<n; i++) { cout << l_cache[length_index][i] << " "; }
            cout << endl;

            cout << "r_cache:";
            for(int i=0; i<n; i++) { cout << r_cache[length_index][i] << " "; }
            cout << endl;

            //r_cache[start..end]: - # of (start-end)-sequences ending with tab[end]
                //add r_cache[start..end-1] if tab[end-1] < tab[end]
                //add l_cache[start..end-1] if tab[start] < tab[end]
            //l_cache[start..end]: - # of (start-end)-sequences beginning with tab[start]
                //add l_cache[start+1..end] if tab[start] < tab[start+1]
                //add r_cache[start+1..end] if tab[start] < tab[end]
        }
    }

    ans = 0;


    cout << r_cache[n-1][n-1] + l_cache[n-1][0] <<endl;

    return 0;
}