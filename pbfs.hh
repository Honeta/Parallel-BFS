#ifndef _PBFS_HH
#define _PBFS_HH

#include "bag.hh"
#include <thread>
#include <stdlib.h>
#include <omp.h>
#include <vector>
using namespace std;
const int GRAINSIZE = 2, INF = 0x7fffffff;

void process_pennant(pennant in_pennant, bag &out_bag, const vector<int> (&g)[], vector<int> &dis) {
    if (in_pennant.size() < GRAINSIZE) {
        vector<int> content = in_pennant.content();
        for (auto u : content)
            #pragma omp parallel for
            for (auto v : g[u])
                if (dis[v] == INF) {
                    dis[v] = dis[u] + 1;
                    out_bag.insert(v);
                }
        return;                
    }
    pennant new_pennant = in_pennant.split();
    thread th0(process_pennant, new_pennant, ref(out_bag), ref(g), ref(dis)), th1(process_pennant, in_pennant, ref(out_bag), ref(g), ref(dis));
    th0.join();
    th1.join();
}

void process_layer(bag in_bag, bag &out_bag, vector<int> (&g)[], vector<int> &dis) {
    #pragma omp parallel for
    for (int k = 0; k < 10; ++k)
        if (in_bag[k])
            process_pennant(in_bag[k], out_bag, g, dis);
}

vector<int> pbfs(vector<int> (&g)[], int start, int n) {
    vector<int> dis(n, INF);
    dis[start] = 0;
    vector<bag> v;
    v.emplace_back();
    v[0].insert(start);
    for (int d = 0; v[d].size(); ++d) {
        v.emplace_back();
        process_layer(v[d], v[d + 1], g, dis);
    }
    return dis;
}

#endif