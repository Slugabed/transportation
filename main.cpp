#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

typedef pair<int, int> Road;

Road make_vertex(int v1, int v2){
    return minmax(v1,v2);
}
struct Info{
    int time;
    int bandwidth;
};

int main() {
    map<Road, Info> graph;

    return 0;
}