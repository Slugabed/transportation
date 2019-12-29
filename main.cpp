#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "test_runner.h"

using namespace std;

typedef pair<int, int> Edge;
int N, M;
const int &DAY_IN_MINUTES = 24 * 60;
const int &N_TIME = 10 * DAY_IN_MINUTES;
const int &WEIGHT_OF_TRACKER = 3'000'000;

Edge make_edge(int v1, int v2) {
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
    int vertex;
    int distance;
};

void RemoveLessThanTracker(vector<int> &weights);

void OnlyFasterThanDay(const Graph &graph, vector<int> &weights);


const auto &HeapComparator = [](const VertexDistance &left, const VertexDistance &right) {
    return left.distance >= right.distance;
};

int CountDistance(const Graph &graph, const int &count_vertex, const int &max_weight) {
    map<int, int> VertexToDistance = {{1, 0},
                                      {N, N_TIME}};
    vector<VertexDistance> heap = {{1, 0}};
    while (!heap.empty()) {
        pop_heap(heap.begin(), heap.end(), HeapComparator);
        const int &top_vertex = heap.back().vertex;
        heap.pop_back();
        // find all adjacent edges to top_vertex from heap
        const auto &beginIt = graph.lower_bound(make_edge(top_vertex, 1));
        const auto &endIt = graph.upper_bound(make_edge(top_vertex, N));

        for (auto EdgeInfoIt = beginIt; EdgeInfoIt != endIt; EdgeInfoIt++) {
            const Info &info = EdgeInfoIt->second;
            const int &v2 = EdgeInfoIt->first.second;
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

bool IfPathExists(const Graph &graph, const int &count_vertex, const int &max_weight) {
    return CountDistance(graph, count_vertex, max_weight) <= DAY_IN_MINUTES;
}


int DoTask(istream &is, ostream &os) {
    Graph graph;
    vector<int> weights;

    is >> N >> M;
    for (int i = 0; i < M; i++) {
        int v1, v2, t, b;
        is >> v1 >> v2 >> t >> b;
        graph[make_edge(v1, v2)] = {t, b};
        graph[make_edge(v2, v1)] = {t, b};
        weights.push_back(b);
    }
    if (N == 1 and M == 0) {
        os << 10000000;
        return 0;
    }
    sort_and_unique(weights);

    RemoveLessThanTracker(weights);

    if (weights.empty()) {
        os << 0;
        return 0;
    }
    OnlyFasterThanDay(graph, weights);
    if (weights.empty()) {
        os << 0;
        return 0;
    }
    sort(weights.begin(), weights.end());
    os << ((weights.back() - 3000000) / 100);

    return 0;
}

void OnlyFasterThanDay(const Graph &graph, vector<int> &weights) {// TODO can use binary search
    const auto &pivot = stable_partition(weights.begin(), weights.end(), [graph](const int &w) {
        return IfPathExists(graph, N, w);
    });
    weights.erase(pivot, weights.end());
}

void RemoveLessThanTracker(vector<int> &weights) {
    const auto &pivot_tracker = stable_partition(weights.begin(), weights.end(), [](const int &w) {
        return w >= WEIGHT_OF_TRACKER;
    });
    weights.erase(pivot_tracker, weights.end());
}

//int main() {
//    DoTask(cin, cout);
//}

void TestAll();
int main() {
    TestAll();
}

void Test() {
    {
        stringstream ss;
        stringstream os;
        ss << "4 5\n"
              "1 4 1000 3001000\n"
              "1 2 440 3001200\n"
              "1 3 449 3001500\n"
              "3 2 1 3001500\n"
              "2 4 990 3001500";
        DoTask(ss, os);
        string result;
        getline(os, result);
        AssertEqual("15", result, "Test 1");
    }
    {
        stringstream ss;
        stringstream os;
        ss << "4 5\n"
              "1 4 1000 3001000\n"
              "1 2 440 3001200\n"
              "1 3 450 3001500\n"
              "3 2 1 3001500\n"
              "2 4 990 3001500";
        DoTask(ss, os);
        string result;
        getline(os, result);
        AssertEqual("12", result, "Test 2");
    }
    //TEST 3
    {
        stringstream ss;
        stringstream os;
        ss << "2 1\n"
              "1 2 100 3000000";
        DoTask(ss, os);
        string result;
        getline(os, result);
        AssertEqual("0", result, "Test 3");
    }
    {
        stringstream ss;
        stringstream os;
        ss << "2 1\n"
              "1 2 100 2500000";
        DoTask(ss, os);
        string result;
        getline(os, result);
        AssertEqual("0", result, "Test 4");
    }
    {
        stringstream ss;
        stringstream os;
        ss << "3 2\n"
              "1 2 720 3000100\n"
              "2 3 721 3000200";
        DoTask(ss, os);
        string result;
        getline(os, result);
        AssertEqual("0", result, "Test 5");
    }
    {
        stringstream ss;
        stringstream os;
        ss << "1 0";
        DoTask(ss, os);
        string result;
        getline(os, result);
        AssertEqual("10000000", result, "Test 6");
    }
    {
        stringstream ss;
        stringstream os;
        ss << "3 2\n"
              "1 2 720 3000100\n"
              "2 3 720 3000200";
        DoTask(ss, os);
        string result;
        getline(os, result);
        AssertEqual("1", result, "Test 7");
    }
    {
        stringstream ss;
        stringstream os;
        ss << "3 3\n"
              "1 3 1440 3000299\n"
              "1 2 720 3000100\n"
              "2 3 720 3000200";
        DoTask(ss, os);
        string result;
        getline(os, result);
        AssertEqual("2", result, "Test 8");
    }
//    {
//        stringstream ss;
//        stringstream os;
//        ss << "";
//        DoTask(ss, os);
//        string result;
//        getline(os, result);
//        AssertEqual("12", result, "Test 2");
//    }

}

void TestAll() {
    TestRunner tr;
    tr.RunTest(Test, "Test");
}
