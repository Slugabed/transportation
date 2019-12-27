#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>

using namespace std;

typedef pair<unsigned, unsigned> Edge;
unsigned N, M;
unsigned DAY_IN_MINUTES = 24 * 60;
unsigned MAX_TIME = 10 * DAY_IN_MINUTES;

Edge make_edge(unsigned v1, unsigned v2) {
    return make_pair(v1, v2);
}

struct Info {
    int time;
    int weight;
};
typedef map<Edge, Info> Graph;

void sort_and_unique(vector<int> &v) {
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
}


struct VertexDistance {
    unsigned distance;
    unsigned vertex;
};

bool operator<(const VertexDistance &left, const VertexDistance &right) {
    return left.distance < right.distance;
};

const auto &HeapComparator = [](const VertexDistance &left, const VertexDistance &right) {
    return left.distance >= right.distance;
};
//const auto &FindVertexDistanceComparator = [](const VertexDistance &left, const VertexDistance &right) {
//    return
//}; TODO

unsigned CountDistance(const Graph &graph, const unsigned &count_vertex, const unsigned &max_weight) {
    map<unsigned, unsigned> distance;
    vector<VertexDistance> heap = {{0, 1}};
    make_heap(heap.begin(), heap.end(), HeapComparator);
    while (!heap.empty()) {
        pop_heap(heap.begin(), heap.end(), HeapComparator);
        VertexDistance current = *(heap.end() - 1);
        const unsigned &top_vertex = current.vertex;
        heap.pop_back();
        // find all adjacent edges to top_vertex from heap
        const auto &beginIt = graph.lower_bound(make_edge(top_vertex, 1));
        const auto &endIt = graph.upper_bound(make_edge(top_vertex, N));

        for (auto it = beginIt; it != endIt; it++) {
            const Edge &edge = it->first;
            const Info &info = it->second;
            const unsigned &v2 = edge.second;
            if (info.weight < max_weight) {
                continue;
            }
            if (distance.count(v2) == 0) {
                distance[v2] = distance[top_vertex] + info.time;
                heap.push_back({distance[v2], v2});
                push_heap(heap.begin(), heap.end());
                continue;
            }
            if (distance[top_vertex] + info.time < distance[v2]) {
                distance[v2] = distance[top_vertex] + info.time;
//                binary_search(heap.begin(), heap.end(), {0, v2}, ) TODO
            }
            if (v2 == N and distance[v2] <= MAX_TIME) {
                break;
            }
        }
    }
    return distance[N];
}

bool IfPathExists(const Graph &graph, const unsigned &count_vertex, const unsigned &max_weight){
    return CountDistance(graph, count_vertex, max_weight) <= DAY_IN_MINUTES;
}


int main() {
    Graph graph;
    vector<int> weights;

    cin >> N >> M;
    for (unsigned i = 0; i < M; i++) {
        int v1, v2, t, b;
        cin >> v1 >> v2 >> t >> b;
        graph[make_edge(v1, v2)] = {t, b};
        graph[make_edge(v2, v1)] = {t, b};
        weights.push_back(b);
    }
    if (N == 1 and M == 0) {
        cout << 10000000;
        return 0;
    }
    sort_and_unique(weights);

    unsigned left = 0;
    unsigned right = weights.size() - 1;
    unsigned currentWeightIndex = weights.size() / 2;
    unsigned previousWeightIndex;
    while (true) {
        if (IfPathExists(graph, N, weights[currentWeightIndex])) {
            left = currentWeightIndex;
            previousWeightIndex = currentWeightIndex;
            currentWeightIndex = ceil((right + left) / 2.0);
            if (previousWeightIndex == currentWeightIndex) {
                break;
            }
        } else {
            right = currentWeightIndex;
            previousWeightIndex = currentWeightIndex;
            currentWeightIndex = ceil((right + left) / 2.0);
            if (previousWeightIndex == currentWeightIndex) {
                if (currentWeightIndex != weights.begin()){

                }
                currentWeightIndex = currentWeightIndex - 1;
                break;
            }
        }
    }
    if (currentWeightIndex >= 0 and weights[currentWeightIndex] - 3000000 > 0) {
        cout << floor((weights[currentWeightIndex] - 3000000) / 100);
    } else {
        cout << 0;
    }

    return 0;
}