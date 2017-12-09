#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
#include <cassert>

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
typedef pair<int, int> node_with_distance; // {distance, id}

node nodes[N_MAX+1]; // nodes sorted by id
int distance_from_first[N_MAX+1]; // [i] := distance between 1st and i-th node
vector<node> nodes_x; // nodes sorted by x
vector<node> nodes_y; // nodes sorted by y

priority_queue<node_with_distance, vector<node_with_distance>, greater<node_with_distance>> Q;

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

void update_distance(node source, node to_update, bool along_x, int &min_distance, node &min_node) {
    assert(node_id(to_update) >= 1 && node_id(to_update) <= N_MAX);

    int old_distance = distance_from_first[node_id(to_update)];
    int distance_diff = along_x ? dist_diff_x(source, to_update) : dist_diff_y(source, to_update);
    int new_distance = distance_from_first[node_id(source)] + distance_diff;
    if(new_distance < old_distance) {
        distance_from_first[node_id(to_update)] = new_distance;
        Q.emplace(new_distance, to_update.first);
        min_distance = new_distance;
        min_node = to_update;
    }
}

node closest_update_distance(node n) {
    coords n_coords = n.second;
    node min_node;
    int min_dist = INT_MAX;

    auto pos_x = lower_bound(nodes_x.begin(), nodes_x.end(), n, compare_x);
    if(pos_x != nodes_x.begin()) {
        update_distance(n, *(pos_x-1), true, min_dist, min_node);
    }
    if(pos_x != nodes_x.end()-1) {
        update_distance(n, *(pos_x+1), true, min_dist, min_node);
    }

    auto pos_y = lower_bound(nodes_y.begin(), nodes_y.end(), n, compare_y);
    if(pos_y != nodes_y.begin()) {
        update_distance(n, *(pos_y-1), false, min_dist, min_node);
    }
    if(pos_y != nodes_y.end()-1) {
        update_distance(n, *(pos_y+1), false, min_dist, min_node);
    }

    return min_node;
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

    // Dijkstra
    distance_from_first[start_id] = 0;
    Q.emplace(0, start_id);
    while (!Q.empty()) {
        auto current_data = Q.top();
        Q.pop();
        node current_node = nodes[current_data.second];
        node closest = closest_update_distance(current_node);
        if(closest.first == end_id) {
            break;
        }
    }

    cout << distance_from_first[end_id] << endl;
    return 0;
}
