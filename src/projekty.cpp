#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// n projektów, m zależności między projektami, k projektów do zrealizowania
// n x wiersz z kosztem projektu w programistach
// m x (a, b) oznaczające że do wykonania a potrzeba b
// znaleźć minimalną liczbę programistów do wykonania k projektów

/*
 * Za wolne rozwiązanie
 * 1) Budujemy cały graf w czasie n+m
 * 2) Bierzemy jakieś k, dodajemy do kolejki zadania które wymagają mniej niż k programistów
 *
 * O(nlogn_m)
 * Jak robić to szybciej:
 * 1) Wrzucamy wierzchołki które od niczego nie zależą do kolejki priorytetowej
 * 2) Zakładamy że wykonamy te projekty w kolejności od najtańszego
 * 3) Wykonujemy projekty w tej kolejności, po wykonaniu:
 *  - usuwamy wykonany projekt z kolejki
 *  - dodajemy do kolejki projekt który wymagał wykonania poprzenio wykonanego projektu
 *    (tylko jeżeli taki istnieje i nie wymaga wykonania innych projektów)
 *    (zmieniamy wagę dodawanego projektu na max(jego waga, waga wykonanego projektu))
 */

const int N_MAX = 100000;
const int M_MAX = 500000;

int cost[N_MAX+1];
int dependencies[N_MAX+1];
vector<int> G[N_MAX+1];

priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> Q; // {cost, id}

// dla każdego wierzchołka trzymamy jego liczbę
// policzyć pary(koszt wierzchołka, indeks wierzchołka)
// w priority queue pary są sortowane po współrzędnych

int main() {
    // initialize arrays
    for(int i=1; i<=N_MAX; i++) {
        cost[i] = 0; // probably unnecessary
        dependencies[i] = 0;
    }

    // load input
    int n, m, k;
    cin >> n >> m >> k;
    for(int i=1; i<=n; i++) {
        cin >> cost[i];
    }
    for(int i=1; i<=m; i++) {
        int a, b;
        cin >> a >> b;
        G[b].push_back(a);
        dependencies[a]++;
    }

    // add single projects to priority queue
    for(int i=1; i<=n; i++) {
        if(dependencies[i] == 0) {
            Q.push(pair<int, int>(cost[i], i));
        }
    }

    int projects_to_do = k;
    int min_cost_required = 0;
    while(!Q.empty() && projects_to_do>0) {
        auto current_project = Q.top();
        Q.pop();
        int current_project_id = current_project.second;
        // remove dependency from projects depending on current_project
        for(int following_project : G[current_project_id]) {
            dependencies[following_project]--;
            if(dependencies[following_project] == 0) {
                Q.push(pair<int, int>(cost[following_project], following_project));
            }
        }
        // mark current_project as done
        min_cost_required = max(min_cost_required, cost[current_project_id]);
        projects_to_do--;
    }
    cout << min_cost_required << endl;
    return 0;
}
