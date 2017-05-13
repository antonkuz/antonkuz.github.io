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

#include <omp.h>

#define INF INT_MAX
#define MAX_CHARS_PER_LINE 80

typedef pair<int, int> iPair;

list< iPair > *adj;
int vertices_num;

void SSSP(int src, vector<int>* distP) {
    if (adj[src].size() == 0) return;

    //Determines whether the node has been added to final
    vector<bool> finalized(vertices_num, false);

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

bool mycomparator (pair<int, double> p, pair<int, double> r) {
    return p.second>r.second; 
}


int main(int argc, char **argv) {
    if (argc < 6) {
        printf("pass at least 5 args\n");
        return 0;
    }
    char* filename = argv[1];
    vertices_num = atoi(argv[2]);
    bool is_undirected = atoi(argv[3]);
    char* index_to_name_filename = argv[4];
    char* centrality_results_filename = argv[5];
    if (argc == 7) omp_set_num_threads(atoi(argv[6]));

    adj = new list< iPair >[vertices_num];

    ifstream fin;
    fin.open(filename);
    if (!fin.good()) {
        printf("file not found\n");
        return 1; // exit if file not found
    }

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

    vector< pair<int, double> > scores(vertices_num);

    #pragma omp parallel for
    for (int src = 0; src < vertices_num; src++) {
        vector<int> dist(vertices_num, INF);
        SSSP(src, &dist);
        double sum = 0.0;
        for (int dst = 0; dst < vertices_num; dst++) {
            if (dst == src) continue;
            sum += double(dist[dst])/10.0;
        }
        double centrality = vertices_num/sum;
        scores[src] = make_pair(src, centrality);
    }

    sort(scores.begin(), scores.end(), mycomparator);

    fin.open(index_to_name_filename);
    if (!fin.good()) {
        printf("index_to_name file not found\n");
        return 1; // exit if file not found
    }

    vector< string > index_to_name(vertices_num);

    // read each line of the file
    while (!fin.eof()) {
        char buf[MAX_CHARS_PER_LINE];
        fin.getline(buf, MAX_CHARS_PER_LINE);
        int index;
        char name[MAX_CHARS_PER_LINE];

        sscanf(buf,"%d;%[^\n]",&index,name);

        index_to_name[index] = name;
    }
    fin.close();

    ofstream myfile;
    myfile.open(centrality_results_filename);

    for (int i = 0; i < vertices_num; i++) {
        myfile << i+1 << ". " << index_to_name[scores[i].first] << ": " << scores[i].second << "\n";
    }

    myfile.close();

    printf("see results in %s\n", centrality_results_filename);
    return 0;
}