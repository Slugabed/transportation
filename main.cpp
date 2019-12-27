#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

typedef pair<unsigned, unsigned> Edge;
unsigned N, M;
const unsigned& DAY_IN_MINUTES = 24 * 60;
const unsigned& MAX_TIME = 10 * DAY_IN_MINUTES;
const unsigned& WEIGHT_OF_TRACKER = 3'000'000;

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
    unsigned vertex;
    unsigned distance;
};

void RemoveLessThanTracker(vector<int> &weights);

const auto &HeapComparator = [](const VertexDistance &left, const VertexDistance &right) {
    return left.distance >= right.distance;
};

unsigned CountDistance(const Graph &graph, const unsigned &count_vertex, const unsigned &max_weight) {
    map<unsigned, unsigned> VertexToDistance = {{1, 0}, {N, MAX_TIME}};
    vector<VertexDistance> heap = {{1, 0}};
    while (!heap.empty()) {
        pop_heap(heap.begin(), heap.end(), HeapComparator);
        const unsigned &top_vertex = heap.back().vertex;
        heap.pop_back();
        // find all adjacent edges to top_vertex from heap
        const auto &beginIt = graph.lower_bound(make_edge(top_vertex, 1));
        const auto &endIt = graph.upper_bound(make_edge(top_vertex, N));

        for (auto EdgeInfoIt = beginIt; EdgeInfoIt != endIt; EdgeInfoIt++) {
            const Info &info = EdgeInfoIt->second;
            const unsigned &v2 = EdgeInfoIt->first.second;
            if (info.weight < max_weight) {
                continue;
            }
            if (VertexToDistance.count(v2) == 0) {
                VertexToDistance[v2] = VertexToDistance[top_vertex] + info.time;
                heap.push_back({v2, VertexToDistance[v2]});
                push_heap(heap.begin(), heap.end(), HeapComparator);
                continue;
            }
            if (VertexToDistance[top_vertex] + info.time < VertexToDistance[v2]) {
                VertexToDistance[v2] = VertexToDistance[top_vertex] + info.time;
            }
            if (v2 == N and VertexToDistance[v2] <= DAY_IN_MINUTES) {
                return VertexToDistance[N];
            }
        }
    }
    return VertexToDistance[N];
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

    RemoveLessThanTracker(weights);

    if (weights.empty()){
        cout << 0;
        return 0;
    }
    // TODO can use binary search
    const auto& pivot = stable_partition(weights.begin(), weights.end(), [graph](const unsigned& w){
        return IfPathExists(graph, N, w);
    });
    weights.erase(pivot, weights.end());
    if (weights.empty()){
        cout << 0;
        return 0;
    }
    sort(weights.begin(), weights.end());
    cout << ((weights.back() - 3'000'000) / 100);

    return 0;
}

void RemoveLessThanTracker(vector<int> &weights) {
    const auto& pivot_tracker = stable_partition(weights.begin(), weights.end(), [](const unsigned& w){
        return w >= WEIGHT_OF_TRACKER;
    });
    weights.erase(pivot_tracker, weights.end());
}
