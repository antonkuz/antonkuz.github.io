#include <vector>
#include <utility>
#include <iostream>
#include <list>
#include <climits>
#include <cstring>
#include <map>
#include <queue>
#include <functional>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
using namespace std;

#include "CycleTimer.h"
#include <omp.h>

#define INF INT_MAX
#define MAX_CHARS_PER_LINE 80

typedef pair<int, int> iPair;

list< iPair > *adj;
int vector_size;

void SSSP(int src, vector<int>* distP) {
    if (adj[src].size() == 0) return;

    //Determines whether the node has been added to final
    vector<bool> finalized(vector_size, false);

    // weight-vertex pairs, lowest weight on top
    priority_queue< iPair, vector <iPair> , greater<iPair> > pq;

    // init source
    pq.push(make_pair(0, src));
    (*distP)[src] = 0;

    while(!pq.empty()) { 
        // get closest node
        iPair closest = pq.top();
        int u = closest.second;
        int u_dist = closest.first;
        pq.pop();

        if (finalized[u]) continue;
        finalized[u] = true;

        // go thru neighbors
        list< iPair >::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); i++) {
            // pair: dest and weight
            int v = (*i).first;
            int weight = (*i).second;

            // if there's a shorter path
            if (!finalized[v] && (*distP)[v] > u_dist + weight) {
                // update distance
                (*distP)[v] = u_dist + weight;
                // make it a candidate
                pq.push(make_pair((*distP)[v], v));
            }
        }
    }
}

bool mycomparator (pair<int, float> p, pair<int, float> r) {
    return p.second>r.second; 
}


int main(int argc, char **argv) {
    if (argc < 3) {
        printf("pass more args\n");
        return 0;
    }
    char* filename = argv[1];
    bool is_undirected = atoi(argv[2]);
    if (argc == 4) omp_set_num_threads(atoi(argv[3]));

    ifstream fin;
    fin.open(filename);
    if (!fin.good()) {
        printf("file not found\n");
        return 1; // exit if file not found
    }

    char buf[MAX_CHARS_PER_LINE];
    fin.getline(buf, MAX_CHARS_PER_LINE);
    int max_v;
    int vertices_num;
    int edges_num;
    sscanf(buf,"%d %d %d",&max_v, &vertices_num, &edges_num);

    vector_size = max_v+1;

    adj = new list< iPair >[vector_size];

    // read each line of the file
    while (!fin.eof()) {
        char buf[MAX_CHARS_PER_LINE];
        fin.getline(buf, MAX_CHARS_PER_LINE);
        int src;
        int dst;
        int w;

        sscanf(buf,"%d %d %d",&src,&dst,&w);

        adj[src].push_back(make_pair(dst, w));
        if (is_undirected) adj[dst].push_back(make_pair(src, w));
    }
    fin.close();
    printf("loaded graph for centrality calculation\n");

    vector< pair<int, float> > scores(vector_size);

    double start = CycleTimer::currentSeconds();
    #pragma omp parallel for
    for (int src = 0; src < vector_size; src++) {
        vector<int> dist(vector_size, INF);
        SSSP(src, &dist);
        float sum = 0.0;
        for (int dst = 0; dst < vector_size; dst++) {
            if (dst == src) continue;
            sum += float(dist[dst])/10.0;
        }
        float centrality = vertices_num/sum;
        scores[src] = make_pair(src, centrality);
    }
    cout << "time: " <<  CycleTimer::currentSeconds() - start << "s" << endl;

    return 0;
}