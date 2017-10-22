#include <iostream>
#include <algorithm>

using namespace std;

// bitow w long longu jest wiecej niz 50 (64) wiec
// mecze mozna pamietac jako bity w long longu
// wczytujemy mecze, sortujemy, dodajemy 2^(umer meczu) dla zawodnikow dla 2 polowy
// mamy n (tyle ile zawodnikow) wektorow liczb ktore reprezentuja wystapienia danego zwodnika w meczu
// jesli mamy 2 takie same ciagi{0,1} (liczby) w posortowanym wektorze tzn ze NIE
// w przeciwnym wypadku TAK\

int main() {
    int n, m;
    cin >> n;
    cin >> m;

    unsigned long long team_list[40000];
    for (int i = 0; i < 40000; i++) {
        team_list[i] = 0;
    }

    unsigned long long match_bit = 1;
    for (int i = 0; i < m; i++) {
        int j = 0;
        int current_player;
        // read players from 1st team (0s)
        while (j < n / 2) {
            cin >> current_player;
            //not adding 0s to the team_list
            j++;
        }

        // read players from 2nd team (1s)
        while (j < n) {
            cin >> current_player;
            team_list[current_player - 1] += match_bit;
            j++;
        }

        // update match_bit
        match_bit *= 2;
    }

    sort(begin(team_list), end(team_list));

    for (int i = 0; i < n - 1; i++) {
        if (team_list[i] == team_list[i + 1]) {
            cout << "NIE" << endl;
            return 0;
        }
    }

    cout << "TAK" << endl;
    return 0;
}
