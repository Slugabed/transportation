#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>

using namespace std;

typedef pair<unsigned , unsigned> Edge;

Edge make_edge(unsigned v1, unsigned v2){
    return minmax(v1,v2);
}
struct Info{
    int time;
    int bandwidth;
};
typedef map<Edge, Info> Graph;

void sort_and_unique(vector<int>& v){
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
}

bool IfPathExists(const Graph& graph, const unsigned& count_vertex, const unsigned& max_weight){
    map<unsigned, unsigned> distance;
    vector<Edge> heap;
//    while(!heap.empty()){
//
//    }
    return false;
}

int main() {
    Graph graph;
    vector<int> bandwidths;
    unsigned N, M;
    cin >> N >> M;
    for(unsigned i=0; i<M; i++){
        int v1, v2, t, b;
        cin >> v1 >> v2 >> t >> b;
        graph[make_edge(v1, v2)] = {t, b};
        bandwidths.push_back(b);
    }
    sort_and_unique(bandwidths);

    unsigned left = 0;
    unsigned right = bandwidths.size() - 1;
    unsigned currentWeightIndex = bandwidths.size() / 2;
    unsigned previousWeightIndex;
    while(true){
        if (IfPathExists(graph, N, bandwidths[currentWeightIndex])){
            left = currentWeightIndex;
            previousWeightIndex = currentWeightIndex;
            currentWeightIndex = ceil((right + left) / 2.0);
            if (previousWeightIndex == currentWeightIndex) {
                break;
            }
        }else{
            right = currentWeightIndex;
            previousWeightIndex = currentWeightIndex;
            currentWeightIndex = ceil((right + left) / 2.0);
            if (previousWeightIndex == currentWeightIndex) {
                currentWeightIndex = currentWeightIndex - 1;
                break;
            }
        }
    }
    if (currentWeightIndex >= 0 and bandwidths[currentWeightIndex]-3000000 > 0) {
        cout << floor((bandwidths[currentWeightIndex] - 3000000) / 100);
    } else{
        cout << 0;
    }

    return 0;
}