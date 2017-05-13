---
layout: default
title:  "Final Report"
date:   2017-05-12 11:11:20
categories: main
---

# Final Report

## Summary
We parallelized closeness centrality calculation for weighted graphs and applied the algorithm to graphs that we mined via Facebook Graph API. We developed several implementations using OpenMP, MPI, and achieved 15x speedup compared to a C++ boost implementation. We also implemented a mining script that allows us to apply the algorithm to real Facebook data.

## Backround
**Closeness centrality** is a metric expresses the average social distance from each individual to every other individual in the network. To calculate it, we divide 1 by the _average shortest path_ from an individual to all other individuals in the network. Closeness centrality tends to give high scores to individuals who are near the center of local clusters in an overall larger network. High closeness centrality individuals tend to be important influencers within their local network community.

**Problem setup**. We wanted to apply closeness centrality to social graphs, and present the results by outputting a table with vertices (individuals) and their closeness centrality scores. As a constraint we decided to work with _weighted_ graphs only. This eliminated the option for using BFS.

The most computationally expensive part of closeness centrality calculation is getting _all pairs shortest paths_ (APSP). The 2 main conventional approaches for that are Floyd-Warshall algorithm and Dijkstra's. Floyd-Warshall operates on adjacency matrix and returns a matrix with APSP distances. Dijkstra's is for single source shortest path, so it'd need to be repeated for each vertex. Despite this, sequential Dijkstra's is faster for sparse graphs: complexity is O(V^2*logV) [VlogV repeated V times], while Floyd-Warshall's is O(V^3). Social network graphs are sparse, so sequential Dijkstra's is faster than Floyd-Warshall for our use case.

The 2 parallelization technologies we focused on were OpenMP and MPI. Baseline we compared against was C++ boost implementation.

Finally, we applied closeness centrality computation to real Facebook graphs. We used a Python API which has a function to provide connections for a given user. The graph had to be weighted, so we added a metric to assign weights to friendships.

## Approach
We analyzed 4 combinations of 2 algorithms(FW and Dijkstra's) and 2 technologies (OpenMP and MPI).

**Floyd-Warshall + OpenMP**. Only inner loops are parallelizable, pragma not good for that.

**Floyd-Warshall + MPI**. ~~~~INSERT HERE~~~~

**Dijkstra's + OpenMP**. As mentioned before, the end result we needed was closeness centrality scores for all individuals, so we needed shortest path distances for each vertex in a graph. Therefore, we applied OpenMP to run the searches in parallel with each other. Unlike Floyd-Warshall, there is a lot of implementation details left out in the algorithm pseudocode.
![Dijkstra's pseudocode](INSERT LINK)
We went through a couple iterations of speeding up sequential Dijkstra's. We started with the [implementation from GeeksforGeeks](http://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/). For fast extract-min, it uses C++ standard library's priority queue. The priority queue, however, doesn't provide decrease-key function (part of `relax` in pseudocode). The online implementation deals with it by inserting the vertex in the priority queue again. This adds overhead in the end of the computation. We optimized the online implementation by adding a `finalized` array and tweaking the data scope (e.g. got rid of OOP).

For mining, the challenges were working with Facebook graph structure and weighing the edges. 
The mining basically consists of the following steps:
1. We initially don't know the size of the graph, so store data in adjacency list.
2. Note: For weighing the edges, we decided to use the number of mutual friends as a metric. 
3. Make an adjacency matrix and move the graph data.
4. Convert adjacency matrix to distance matrix by counting mutual friends for each pair.
5. Store the graph in text file by listing all edges.

## Results
**Floyd-Warshall + MPI**. ~~~~INSERT HERE~~~~

**Dijkstra's + OpenMP**.
 - our sequential Dijkstra's was slightly faster than C++ boost library function. 
 - used boost as a baseline, as it's considered high quality, commonly used, and is available for free
 - ![Plot1](https://scontent.fagc2-1.fna.fbcdn.net/v/t35.0-12/18471761_654383788099442_1537955144_o.png?oh=cf139bb12b17b52ef5bbb5316cf3b452&oe=59189324)

 - discuss the plot in detail: measured against boost, sequential divided by wall-clock.
 - size and properties of test graph. mention that it's connected
 - representation of graph and script to create them
 - wall clock graph
 - sssp is followed by sequential addition... nested openmp.
 - speedup limited at 24 cores.
 - bandwidth bound: memory accessees to priority queue, adj list, dist vector. we know it's not amdahl's law. also load imbalance.
 - NEW: time runtime against graph size on same number of cores
 - NEW: time how long it spends in sum section

**Dijkstra's + MPI **.
No need to parallelize within since #ofNodes >> #ofCores

**Mining**. 
 - algo supports directed/undirected
 - Pipelined mining into Centrality code.
 - Extra stuff to convert names to numbers and back

## References
 * http://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/
 * http://konect.uni-koblenz.de/networks/munmun_twitter_social
 * https://facebook-sdk.readthedocs.io/en/latest/index.html
 * http://www.boost.org/doc/libs/1_64_0/libs/graph/doc/index.html

## List of work by each student

