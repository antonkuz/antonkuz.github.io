#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/graph_traits.hpp>

using namespace boost;

#include <fstream>
#include <iostream>  
#include <algorithm>
#include <vector>
#include <utility>
#include <list>
#include <climits>
#include <cstring>
#include <map>
#include <queue>          // priority_queue
#include <functional>     // greater
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#include "CycleTimer.h"

#define MAX_CHARS_PER_LINE 20
typedef adjacency_list<listS, vecS, directedS, no_property, property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef pair<int,int> E;

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("pass more args\n");
        return 0;
    }
    char* filename = argv[1];
    bool is_undirected = atoi(argv[2]);

    ifstream fin;
    fin.open(filename);
    if (!fin.good()) {
        printf("file not found\n");
        return 1;
    }

    char buf[MAX_CHARS_PER_LINE];
    fin.getline(buf, MAX_CHARS_PER_LINE);
    int max_v;
    int vertices_num;
    int edges_num;
    sscanf(buf,"%d %d %d",&max_v, &vertices_num, &edges_num);

    if (is_undirected) edges_num *= 2;

    E edges[edges_num];
    int weights[edges_num];

    // read each line of the file
    int counter = 0;
    while (!fin.eof()) {
        char buf[MAX_CHARS_PER_LINE];
        fin.getline(buf, MAX_CHARS_PER_LINE);
        int src;
        int dst;
        int weight;
        sscanf(buf,"%d %d %f",&src,&dst,&weight);

        edges[counter] = E(src, dst);
        weights[counter++] = weight;
        if (is_undirected) {
            edges[counter] = E(dst, src);
            weights[counter++] = weight;
        }
    }
    fin.close();
    printf("loaded graph for centrality calculation\n");

    Graph G(edges, edges + sizeof(edges) / sizeof(E), weights, vertices_num);

    vector< pair<int, float> > scores(vertices_num);

    double start = CycleTimer::currentSeconds();
    for (int src = 0; src < vertices_num; src++) {
        vector<int> dist(vertices_num);
        dijkstra_shortest_paths(G, src, distance_map(&dist[0]));
        float sum = 0.0;
        for (int dst = 0; dst < vertices_num; dst++) {
            if (dst == src) continue;
            sum += float(dist[dst])/10.0;
        }
        float centrality = vertices_num/sum;
        scores[src] = make_pair(src, centrality);
    }
    cout << "time: " <<  CycleTimer::currentSeconds() - start << "s" << endl;

    return 0;
}
