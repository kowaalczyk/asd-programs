#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
 * Chcemy znaleźć najszybszą drogę od wyspy 1 do wyspy n
 * Nie opłaca się płynąć po drodze A..B po drodze na której jest jakikolwiek punkt.
 *  ==> nie ważne czy omijamy wyspy:
 *  jeśli chcemy płynąć z A do B to jest to samo co płynięcie do najbliższej wg. x kropki
 *
 *  - w danym punkcie wystarczy pamiętać najbliższy punkt względem współrzędnych x i y (4 punkty dla każdego punktu)
 *
 *  Jeżeli nie mielibyśmy żadnych dwóch punktów na jednym odcinku, moglibyśmy zrobić Dijkstre
 *  - jak mamy linie możemy umówić się że płyniemy do najdalszego w linii wierzchołków
 *    , wrzucając do kolejki wszystkie minięte wierzchołki z linii
 *
 *  Impl:
 *  - dla każdego wierzchołka trzymamy 4 najbliżse wierzchołki, ale nie w standardowy sposób
 *    tylko w formie 2 wektorów wierzchołków - 1 posortowany wg. x, 2 posortowany wg. y
 *    O(nlogn) sortowanie na początku, potem w O(1) znajdowanie najbliższych
 *  - DecreaseKey na PriorityQueue w c++:
 *    - po prostu ściagamy wierzchołek i jeżeli nie jest znana dla niego odległość to
 *      wrzucacmy go jeszcze raz ze zmniejszonym kluczem
 *  - na początku w kolejce priorytetowej jest tylko start
 *  -
 */

const int N_MAX = 200000;

vector<pair<int, int>> nodes_x;
vector<pair<int, int>> nodes_y;

bool compare_x(pair<int, int> lhs, pair<int, int> rhs) {
    int l_x = lhs.first;
    int r_x = rhs.first;
    if (l_x < r_x) {
        return true;
    } else if (l_x > r_x) {
        return false;
    } else {
        return lhs.second < rhs.second;
    }
}

bool compare_y(pair<int, int> lhs, pair<int, int> rhs) {
    int l_y = lhs.second;
    int r_y = rhs.second;
    if (l_y < r_y) {
        return true;
    } else if (l_y > r_y) {
        return false;
    } else {
        return lhs.first < rhs.first;
    }
}

int main() {
    int n;
    cin >> n; // liczba wysp
    nodes_x.reserve(n);
    nodes_y.reserve(n);
    for(int i=1; i<=n; i++) {
        int x, y;
        cin >> x >> y; // współrzędne i-tej wyspy
        pair<int, int> node(x, y);
        nodes_x.push_back(node);
        nodes_y.push_back(node);
    }
    sort(nodes_x.begin(), nodes_x.end(), compare_x);
    sort(nodes_y.begin(), nodes_y.end(), compare_y);

    return 0;
}
