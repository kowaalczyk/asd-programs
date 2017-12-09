#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
#include <cassert>

using namespace std;

/* (c) Krzysztof Kowalczyk k.kowaalczyk@gmail.com
 *
 * Problem:
 * Given n coordinates <x, y>, each pair representing an island on a map, find an optimal route between
 * 1st and last island for a ship captain, with following assumptions:
 * - ship can only swim along x or y axis
 * - each time captain selects a person steering the ship: himself or a first officer
 * - each time the ship turns, steering person changes
 * - optimal route is the one that minimizes time of captain steering (assuming ship sails at constant speed)
 *
 * Solution:
 * - represent each island as a node within a graph
 * - for each island calculate closest islands on each axis (in both directions) and create a graph edge between them
 * - find an optimal route within such graph using standard Dijkstra algorithm:
 *   this will minimize time of captain steering as we are always adding node with closest distance along one axis
 * Time complexity: O(n*log(n))
 */

const int N_MAX = 200000;

typedef pair<int, int> coords; /// <x, y> - coordinates
typedef pair<int, coords> node; /// <id, coords> - node
typedef pair<int, int> node_with_distance; /// <distance, id> - container for Dijkstra queue

node nodes[N_MAX+1]; /// nodes[i] := node with id i
int distance_from_first[N_MAX+1]; /// distance_from_first[i] := distance between 1st and i-th node
vector<node> nodes_x; /// nodes sorted by x coordinate
vector<node> nodes_y; /// nodes sorted by y coordinate

priority_queue<node_with_distance, vector<node_with_distance>, greater<node_with_distance>> Q; /// queue for Dijkstra algorithm

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

/**
 * Updates distance from source node to to_update, along axis depending on along_x boolean.
 * If distance between nodes decreased, updated node will be added to Dijkstra queue.
 * If calculated distance is smaller than min_distance, min_distancce and min_node will be updated to match updated node
 */
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

/// updates distance to all neighbours and adds them to Dijkstra queue if necessary
node closest_update_distance(node n) {
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
        distance_from_first[i] = INT_MAX;
    }

    // load data
    int n;
    cin >> n; // # of islands
    const int start_id = 1;
    const int end_id = n;
    for (int i=1; i<=n; i++) {
        int x, y;
        cin >> x >> y; // # coordinates of i-th island
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
    }

    cout << distance_from_first[end_id] << endl;
    return 0;
}
