#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>

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

typedef pair<int, int> coords; // {x, y} - coordinates
typedef pair<int, coords> node; // {id, coords} - node

node nodes[N_MAX+1]; // nodes sorted by id
int distance_from_first[N_MAX+1]; // [i] := distance between 1st and i-th node
vector<node> nodes_x; // nodes sorted by x
vector<node> nodes_y; // nodes sorted by y

priority_queue<node, vector<node>, greater<node>> Q; // TODO: Q of node_with_distance

bool compare_x(node lhs, node rhs) {
    coords l_coords = lhs.second;
    coords r_coords = rhs.second;
    if (l_coords.first < r_coords.first) {
        return true;
    } else if (l_coords.first > r_coords.first) {
        return false;
    } else {
        return l_coords.second < r_coords.second;
    }
}

bool compare_y(node lhs, node rhs) {
    coords l_coords = lhs.second;
    coords r_coords = rhs.second;
    if (l_coords.second < r_coords.second) {
        return true;
    } else if (l_coords.second > r_coords.second) {
        return false;
    } else {
        return l_coords.first < r_coords.first;
    }
}

int node_id(node n) { return n.first; }

int dist_diff_x(node p, node q) {
    if (p.second.first > q.second.first) {
        return p.second.first - q.second.first;
    } else {
        return q.second.first - p.second.first;
    }
}

int dist_diff_y(node p, node q) {
    if (p.second.second > q.second.second) {
        return p.second.second - q.second.second;
    } else {
        return q.second.second - p.second.second;
    }
}

node closest_update_distance(node n) {
    coords n_coords = n.second;
    int n_distance = distance_from_first[node_id(n)];

    auto pos_x = lower_bound(nodes_x.begin(), nodes_x.end(), n_coords);
    node before_x = *(pos_x-1);
    int before_old_distance = distance_from_first[node_id(before_x)];
    int before_new_distance = distance_from_first[node_id(n)] + dist_diff_x(n, before_x);
    if(before_new_distance < before_old_distance) {
        distance_from_first[node_id(before_x)] = before_new_distance;
        Q.push(before_x); // TODO: Make sure pushing nodes to Q only when distance is lesser is ok
    }
    // TODO: Move to separate function
    // TODO: Finish from here (other closest nodes)

    node after_x = *(pos_x+1);


    auto pos_y = lower_bound(nodes_y.begin(), nodes_y.end(), n_coords);
    node before_y = *(pos_y-1);
    node after_y = *(pos_y+1);
    node mins[4] = {before_x, after_x, before_y, after_y};
}

int main() {
    // reset data
    for(int i=1; i<=N_MAX; i++) {
        distance_from_first[i] = INT_MAX; // TODO: Make sure this value is ok for init
    }
    // load data
    int n;
    cin >> n; // liczba wysp
    const int start_id = 1;
    const int end_id = n;
    nodes_x.reserve(n);
    nodes_y.reserve(n);
    for (int i=1; i<=n; i++) {
        int x, y;
        cin >> x >> y; // współrzędne i-tej wyspy
        node current_node(i, coords(x, y));
        nodes[i] = current_node;
        nodes_x.push_back(current_node);
        nodes_y.push_back(current_node);
    }
    sort(nodes_x.begin(), nodes_x.end(), compare_x);
    sort(nodes_y.begin(), nodes_y.end(), compare_y);

    distance_from_first[start_id] = 0;
    Q.push(nodes[start_id]);
    while (!Q.empty()) {
        // get first node
        node current_node = Q.top();
        Q.pop();

        // get closest node to current_node

        // set distance to closest node

        // if distance to last_node was just calculated, break
    }

    return 0;
}
