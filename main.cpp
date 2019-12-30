#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

typedef pair<int, int> Edge;

const int &DAY_IN_MINUTES = 24 * 60;
const int &WEIGHT_OF_TRACKER = 3'000'000;

Edge make_edge(int v1, int v2) {
    return make_pair(v1, v2);
}

struct EdgeInfo {
    int time;
    int weight;
};
typedef map<Edge, EdgeInfo> Graph;

void sort_and_unique(vector<int> &v) {
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
}


struct VertexDistance {
    int vertex;
    int distance;
};

void RemoveLessThanTracker(vector<int> &weights);

const auto &HeapComparator = [](const VertexDistance &left, const VertexDistance &right) {
    return left.distance >= right.distance;
};

template<class Iter, class Predicate>
Iter FindLastTrue(const Iter &beginIt, const Iter &endIt, const Predicate &predicate) {
    if (endIt - beginIt == 0) {
        return endIt;
    }
    if (endIt - beginIt == 1) {
        if (predicate(*beginIt)) {
            return beginIt;
        } else {
            return endIt;
        }
    }
    if (endIt -beginIt == 2){
        if (predicate(*prev(endIt))){
            return prev(endIt);
        } else if (predicate(*beginIt)){
            return beginIt;
        } else {
            return endIt;
        }
    }
    Iter result = endIt;
    auto left = beginIt;
    auto right = prev(endIt);
    bool last_true = false;
    while (right - left > 1) {
        Iter mid = left + (right - left) / 2;
        if (predicate(*mid)) {
            left = mid;
            last_true = true;
        } else {
            right = mid;
            last_true = false;
        }
    }
    if (last_true) {
        if (predicate(*right)) {
            result = right;
        } else {
            result = left;
        }
    } else {
        if (predicate(*left)) {
            result = left;
        }
    }
    return result;
}

int CountDistance(const Graph &graph, const int &min_weight, const int &max_vertex) {
    map<int, int> VertexToDistance = {{1,          0},
                                      {max_vertex, 10 * DAY_IN_MINUTES}};
    vector<VertexDistance> heap = {{1, 0}};
    while (!heap.empty()) {
        pop_heap(heap.begin(), heap.end(), HeapComparator);
        const int current_vertex = heap.back().vertex;
        heap.pop_back();
        // find all adjacent edges to top_vertex from heap
        const auto &beginIt = graph.lower_bound(make_edge(current_vertex, 1));
        const auto &endIt = graph.upper_bound(make_edge(current_vertex, max_vertex));

        for (auto AdjacentEdgeIt = beginIt; AdjacentEdgeIt != endIt; AdjacentEdgeIt++) {
            const EdgeInfo &info = AdjacentEdgeIt->second;
            const int v2 = AdjacentEdgeIt->first.second;
            if (info.weight < min_weight or VertexToDistance[current_vertex] + info.time > DAY_IN_MINUTES) {
                continue;
            }
            if (VertexToDistance.count(v2) == 0) {
                VertexToDistance[v2] = VertexToDistance[current_vertex] + info.time;
                heap.push_back({v2, VertexToDistance[v2]});
                push_heap(heap.begin(), heap.end(), HeapComparator);
                continue;
            }
            if (VertexToDistance[current_vertex] + info.time < VertexToDistance[v2]) {
                VertexToDistance[v2] = VertexToDistance[current_vertex] + info.time;
                const auto &heap_changed = find_if(heap.begin(), end(heap), [v2](const VertexDistance &vd) {
                    return vd.vertex == v2;
                });
                heap_changed->distance = VertexToDistance[v2];
                make_heap(begin(heap), end(heap), HeapComparator);
            }
            if (v2 == max_vertex and VertexToDistance[v2] <= DAY_IN_MINUTES) {
                return VertexToDistance[max_vertex];
            }
        }
    }
    return VertexToDistance[max_vertex];
}

bool IfPathExists(const Graph &graph, const int &min_weight, const int &max_vertex) {
    return CountDistance(graph, min_weight, max_vertex) <= DAY_IN_MINUTES;
}

Graph ReadGraph(istream &is, int &count_vertex, int &count_edges, vector<int> &weights) {
    Graph graph;
    is >> count_vertex >> count_edges;
    for (int i = 0; i < count_edges; i++) {
        int v1, v2, t, w;
        is >> v1 >> v2 >> t >> w;
        graph[make_edge(v1, v2)] = {t, w};
        graph[make_edge(v2, v1)] = {t, w};
        weights.push_back(w);
    }
    return graph;
}

int DoTask(istream &is, ostream &os) {
    vector<int> weights;
    int max_vertex, M;
    Graph graph = ReadGraph(is, max_vertex, M, weights);
    if (max_vertex == 1 and M == 0) {
        os << 10000000;
        return 0;
    }
    sort_and_unique(weights);

    RemoveLessThanTracker(weights);

    if (weights.empty()) {
        os << 0;
        return 0;
    }
    const auto &predicate = [graph, max_vertex](const int &arg) {
        return IfPathExists(graph, arg, max_vertex);
    };
    const auto &found = FindLastTrue(weights.begin(), weights.end(), predicate);
    if (found == weights.end()) {
        os << 0;
        return 0;
    }
    os << ((*found - 3000000) / 100);

    return 0;
}

void RemoveLessThanTracker(vector<int> &weights) {
    const auto &pivot_tracker = stable_partition(weights.begin(), weights.end(), [](const int &w) {
        return w >= WEIGHT_OF_TRACKER;
    });
    weights.erase(pivot_tracker, weights.end());
}

int main() {
    DoTask(cin, cout);
}


#include "test_runner.h"

void TestAll();


int main() {
    TestAll();
}

void TestFindLastTrue() {
    const auto &predicate = [](const int &arg) {
        return arg < 4;
    };
    {
        vector<int> a;
        const auto &iter = FindLastTrue(a.begin(), a.end(), predicate);
        Assert(a.end() == iter, "TestFindLastTrue 1");
    }
    {
        vector<int> a = {1};
        const auto &iter = FindLastTrue(a.begin(), a.end(), predicate);
        Assert(1 == *iter, "TestFindLastTrue 2");
    }
    {
        vector<int> a = {6};
        const auto &iter = FindLastTrue(a.begin(), a.end(), predicate);
        Assert(a.end() == iter, "TestFindLastTrue 3");
    }
    {
        vector<int> a = {6, 9, 10};
        const auto &iter = FindLastTrue(a.begin(), a.end(), predicate);
        Assert(a.end() == iter, "TestFindLastTrue 4");
    }
    {
        vector<int> a = {1, 2, 4};
        const auto &iter = FindLastTrue(a.begin(), a.end(), predicate);
        AssertEqual(2, *iter, "TestFindLastTrue 5");
    }
    {
        vector<int> a = {1, 2};
        const auto &iter = FindLastTrue(a.begin(), a.end(), predicate);
        AssertEqual(2, *iter, "TestFindLastTrue 6");
    }
    {
        vector<int> a = {1, 2, 4, 6};
        const auto &iter = FindLastTrue(a.begin(), a.end(), predicate);
       AssertEqual(2, *iter, "TestFindLastTrue");
    }
}

void TestDistance() {
    {
        stringstream ss;
        ss << "4 5\n"
              "1 4 1000 3001000\n"
              "1 2 440 3001200\n"
              "1 3 449 3001500\n"
              "3 2 1 3001500\n"
              "2 4 990 3001500";
        vector<int> weights;
        int max_vertex, M;
        Graph graph = ReadGraph(ss, max_vertex, M, weights);
        int distance = CountDistance(graph, 3'001'050, max_vertex);
        AssertEqual(1430, distance, "dist 1");
        distance = CountDistance(graph, 3'001'500, max_vertex);
        AssertEqual(1440, distance, "dist 1");
        distance = CountDistance(graph, 3'001'501, max_vertex);
        AssertEqual(10 * DAY_IN_MINUTES, distance, "dist 1");
    }
    {
        stringstream ss;
        ss << "4 5\n"
              "1 4 1000 3001000\n"
              "1 2 440 3001200\n"
              "1 3 450 3001500\n"
              "3 2 1 3001500\n"
              "2 4 990 3001500";
        vector<int> weights;
        int max_vertex, M;
        Graph graph = ReadGraph(ss, max_vertex, M, weights);
        int distance = CountDistance(graph, 3'001'050, max_vertex);
        AssertEqual(1430, distance, "dist 1");
        distance = CountDistance(graph, 3'001'250, max_vertex);
        AssertEqual(1441, distance, "dist 1");
        distance = CountDistance(graph, 3'001'550, max_vertex);
        AssertEqual(10 * DAY_IN_MINUTES, distance, "dist 1");
    }
    {
        stringstream ss;
        ss << "5 6\n"
              "1 2 10000 3001000\n"
              "1 3 1 3001000\n"
              "2 3 1 3001000\n"
              "2 4 1 3001000\n"
              "3 4 1500 3001000\n"
              "4 5 1 3001000";
        vector<int> weights;
        int max_vertex, M;
        Graph graph = ReadGraph(ss, max_vertex, M, weights);
        int distance = CountDistance(graph, 3'000'050, max_vertex);
        AssertEqual(4, distance, "dist heap incorrect");
    }
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
}

void SortAndUniqueTest() {
    vector<int> arr = {1, 5, 3, 7, 4, 9, 1, 4, 3, 3, 3};
    vector<int> expected = {1, 3, 4, 5, 7, 9};
    sort_and_unique(arr);
    AssertEqual(expected, arr, "sort and unique");
}

void RemoveLessThanTrackerTest() {
    vector<int> weights = {WEIGHT_OF_TRACKER + 100, WEIGHT_OF_TRACKER - 100, WEIGHT_OF_TRACKER + 100,
                           WEIGHT_OF_TRACKER - 1, WEIGHT_OF_TRACKER, WEIGHT_OF_TRACKER + 1};
    vector<int> expected = {WEIGHT_OF_TRACKER + 100, WEIGHT_OF_TRACKER + 100, WEIGHT_OF_TRACKER, WEIGHT_OF_TRACKER + 1};
    RemoveLessThanTracker(weights);
    AssertEqual(expected, weights, "RemoveLessThanTrackerTest");
}

void GetLeast(vector<VertexDistance> &heap, int &result) {
    pop_heap(heap.begin(), end(heap), HeapComparator);
    result = heap.back().distance;
    heap.pop_back();
}

void HeapComparatorTest() {
    vector<VertexDistance> heap = {{1, 1},
                                   {1, 1},
                                   {1, 3},
                                   {1, 7},
                                   {1, 2},
                                   {1, 9},
                                   {1, 4},
                                   {1, 6},
                                   {1, 3},
                                   {1, 2},
                                   {1, 7}};
    make_heap(heap.begin(), end(heap), HeapComparator);
    int least;
    heap.push_back({5, 5});
    push_heap(heap.begin(), heap.end(), HeapComparator);
    GetLeast(heap, least);
    AssertEqual(1, least, "Heap");
    GetLeast(heap, least);
    AssertEqual(1, least, "Heap");
    GetLeast(heap, least);
    AssertEqual(2, least, "Heap");
    GetLeast(heap, least);
    AssertEqual(2, least, "Heap");
    GetLeast(heap, least);
    AssertEqual(3, least, "Heap");
    GetLeast(heap, least);
    AssertEqual(3, least, "Heap");
    GetLeast(heap, least);
    AssertEqual(4, least, "Heap");
    GetLeast(heap, least);
    AssertEqual(5, least, "Heap");
    GetLeast(heap, least);
    AssertEqual(6, least, "Heap");
    GetLeast(heap, least);
    AssertEqual(7, least, "Heap");
    GetLeast(heap, least);
    AssertEqual(7, least, "Heap");
    GetLeast(heap, least);
    AssertEqual(9, least, "Heap");
    AssertEqual(true, heap.empty(), "Empty heap");
}

void TestGraphBoundaries() {
    Graph graph = {{make_edge(1, 2), {10, 2}},
                   {make_edge(1, 3), {1,  5}},
                   {make_edge(1, 5), {1,  11}},
                   {make_edge(0, 0), {10, 2}},
                   {make_edge(2, 0), {5,  3}}};
    const auto &beginIt = graph.lower_bound(make_edge(1, 1));
    const auto &endIt = graph.upper_bound(make_edge(1, 5));
    int count = 0;
    for (auto it = beginIt; it != endIt; it++) {
        AssertEqual(1, it->first.first, "First vertex 1");
        count++;
    }
    AssertEqual(3, count, "Count 3");
}

void TestAll() {
    TestRunner tr;
    tr.RunTest(Test, "Test");
    tr.RunTest(SortAndUniqueTest, "SortAndUniqueTest");
    tr.RunTest(RemoveLessThanTrackerTest, "RemoveLessThanTrackerTest");
    tr.RunTest(HeapComparatorTest, "HeapComparatorTest");
    tr.RunTest(TestGraphBoundaries, "TestGraphBoundaries");
    tr.RunTest(TestDistance, "TestDistance");
    tr.RunTest(TestFindLastTrue, "TestFindLastTrue");
}
