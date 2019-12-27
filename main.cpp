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
    unsigned vertex;
    unsigned distance;
};

const auto &HeapComparator = [](const VertexDistance &left, const VertexDistance &right) {
    return left.distance >= right.distance;
};

unsigned CountDistance(const Graph &graph, const unsigned &count_vertex, const unsigned &max_weight) {
    map<unsigned, unsigned> VertexToDistance = {{1, 0}, {N, MAX_TIME}};
    vector<VertexDistance> heap = {{1, 0}};
    while (!heap.empty()) {
        pop_heap(heap.begin(), heap.end(), HeapComparator);
        const unsigned &top_vertex = (heap.end() - 1)->vertex;
        heap.pop_back();
        // find all adjacent edges to top_vertex from heap
        const auto &beginIt = graph.lower_bound(make_edge(top_vertex, 1));
        const auto &endIt = graph.upper_bound(make_edge(top_vertex, N));

        for (auto EdgeInfo = beginIt; EdgeInfo != endIt; EdgeInfo++) {
            const Info &info = EdgeInfo->second;
            const unsigned &v2 = EdgeInfo->first.second;
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
            if (v2 == N and VertexToDistance[v2] <= MAX_TIME) {
                break;
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
                if (currentWeightIndex == 0){
                    cout << 0;
                    return 0;
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