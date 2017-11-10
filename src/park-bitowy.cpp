#include <iostream>
#include <vector>

using namespace std;

/*
 * Dostajemy gigantyczne drzewo binarne
 * chcemy dla danego wierzchołka znajdować wierzchołek oddalony od niego o daną odległość.
 * BFS jest za wolny, chcemy chodzić szybciej niż liniowo
 *
 * Trick1;
 * Część naszych problemów rozwiązuje pamiętanie dla każdego wierzchołka jaki jest od niego najbardziej oddalony:
 * - (-1) możemy zwracać od razu
 * - wpp możemy ograniczyć się tylko do ścieżki pomiędzy tym wierzchołkiem a danym:
 *  a) najdalszy jest moim synem
 *  b) najdalszy jest synem któregoś z moich przodków
 *
 *  Rozwiązanie:
 *  1x BFS do policzenia najbardziej oddalonego w lewo i w prawo (synów) --> preprocessing O(n)
 *  czy uda nam się policzyć najbardziej oddalony w górę?
 *      Nie do końca:
 *      znowu idziemy BFS-em i sprawdzamy jakie są najbardziej oddalone od ojca w przeciwną stronę niż my
 *      i najbardziej oddalony w górę -> porównujemy te 2 wielkości no i mamy nasz najdalszy w górę
 *  Efekt:
 *  mamy już (-1) w czasie stałym, przy 6 pomocniczych tablicach
 *  mamy też 3 możliwości pozycji najbardziej oddalonego:
 *  a) wspólny przodek
 *  b) pod nami
 *  c) nad nami
 *  Będziemy chcieli skorzytać z tego gdzie jest najdalszy żeby to obliczyć:
 *  - możemy pamiętać jeszcze poziom każdego wierzchołka i pamiętać wspólnego przodka z między nim a najdalszym
 *  - możemy też mieć strukture która pozwala nam skakać:
 *      chcemy mieć odnośniki do wierzchołków odległych o 2,4,8,16,32,itd (wtedy mamy O(log(n)))
 *      tablica n x log(n) mieści się w pamięci (dla intów ok. 40MB)
 *      budujemy ją w czasie O(nlog(n)) po BFS-ach, przy jej budowaniu korzystamy sobie z poprzednich wartości
 *      (dla danego wierzchołka przy obliczniu 4 korzystamy z tego że mamy obliczone 2)
 *      --> zaczynamy dla policzenia odległości o 2 od każdego z wierzchłków, potem 4 itd.
 *  - ponieważ mamy głębokość i odległość (dla nas i najdalszego wierzchołka) więc możemy ocenić które z a/b/c zachodzi
 */

/*
 * Takie structy trzymamy sobie w tablicy i w bfs-ie tylko używamy wskaźników
 */
struct Node {
    int parent; //for traversal
    int left; //for traversal
    int right; //for traversal
    int left_range; //calc in bfs1
    int left_range_length; //calc in bfs1
    int right_range; //calc in bfs1
    int right_range_length; //calc in bfs1
    int top_range; //calc in bfs2
    int top_range_length; //calc in bfs2
    int level; //calc in bfs
};

Node tree[500000];

//find any node d edges away from a
int find_node(int a, int d) {

}

int main() {
    int n;
    cin >> n;

    //0 is a root
    for(int i=0; i<n; i++) {
        int l_path, r_path;
        cin >> l_path >> r_path;
        // normalize input to [0...n-1]
        l_path = l_path>0 ? l_path-1 : l_path;
        r_path = r_path>0 ? r_path-1 : r_path;

        tree[i].left = l_path;
        tree[i].right = r_path;
        // (-1) means there is no path

        if(l_path > 0) {
            tree[l_path].parent = i;
        }
        if(r_path > 0) {
            tree[r_path].parent = i;
        }
    }

    //bfs1
    vector<int> current_nodes;
    current_nodes.push_back(0);
    while(!current_nodes.empty()) {
//        int i = current_nodes.front();
//        current_nodes.erase(current_nodes.begin());

    }

    int m;
    cin >> m;
    for(int i=0; i<m; i++) {
        int a, d;
        cin >> a >> d;
        auto found_node = find_node(a,d);
        found_node = found_node>=0 ? found_node+1 : found_node;
        cout << found_node << endl;
    }

    return 0;
}
