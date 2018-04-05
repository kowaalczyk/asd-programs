#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    int n, m;
    cin >> n;
    cin >> m;

    vector<unsigned long long> team_list;
    team_list.resize(40000);
    for (int i = 0; i < 40000; i++) {
        team_list.push_back(0);
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

//    sorting the vector to add duplicates (only sort elements from [0..n) to prevent reading default zeros)
    sort(team_list.begin(), team_list.begin()+n);

    for (int i = 0; i < n - 1; i++) {
        if (team_list[i] == team_list[i + 1]) {
            cout << "NIE" << endl;
            return 0;
        }
    }
//    no duplicates => no 2 players played in exactly the same teams in every mach
    cout << "TAK" << endl;
    return 0;
}
