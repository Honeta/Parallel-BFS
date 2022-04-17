#include "pbfs.hh"
#include <iostream>
using namespace std;

int main() {
    vector<int> g[9];
    g[0] = {1};
    g[1] = {0, 2, 3, 4, 5, 6};
    g[2] = {1};
    g[3] = {1};
    g[4] = {1, 7};
    g[5] = {1, 8};
    g[6] = {1};
    g[7] = {4};
    g[8] = {5};
    auto dis = pbfs(g, 0, 9);
    for (auto x : dis)
        std::cout << x << ' ';
    return 0;
}