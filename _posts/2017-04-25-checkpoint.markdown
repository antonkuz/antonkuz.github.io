---
layout: default
title:  "Checkpoint"
date:   2017-04-25 04:20:00
categories: main
---

# Checkpoint

## Progress
We started off by checking out Facebook API. Facebook provides the so-called Graph API, with which you communicate via RESTful API and get JSON formatted data with friends, posts, likes, etc. As we pulled the friends data, however, we discovered that it only displays the friends that have used the API. Facebook changed privacy settings for the API a couple years ago and now this is the restriction which app developers have no way to go around. We observed that on average less than 10% of friends are available. On top of the privacy issue, pulling the data took a long time: it took 10 minutes to get 1000 vertices (users). We haven't benchmarked to figure out what the exact slowdown cause was (script was written in Python), but it's likely the the API's bandwidth limit. 

Considering alternative options, we have also checked Twitter API. This API had its own drawbacks: while providing all followers, it limits the API calls to 15 requests per 15 minutes which makes it unfeasible for large graph mining.

These discoveries made us reconsider Facebook API plans (see next section).

Next, we looked into closeness centrality algorithm. Closeness centrality values are per vertex, and to get one we need the vertex's shortest path distances to all other vertices in a graph. For social network graphs, we would like to get a table of users sorted by closeness centrality, so we need to find shortest path distances for all pairs, a problem called All Pairs Shortest Paths (APSP). 

We chose two main approaches to APSP: Floyd-Warshall (F-W) and Dijkstra's. Dijkstra's is Single Source Shortest Path, but repeating it V times can still be faster than F-W which is specifically for APSP.

We've implemented simple sequential versions of F-W and Dijkstra's, and have started optimizing Dijkstra's, where implementation details make a big difference in speed. 

We've also started parallelizing F-W using the approach described in the [Phased Floyd-Warshall paper](http://www.cse.psu.edu/~huv101/files/papers/sbgv_2007_icpads.pdf) that uses MPI. This approach utlilizes blocking techniques to improve cache performance and at the same time ensuring lesser communication overheard when running in a distributed setting.

## Goals and Deliverables
We've decided to lower our ambitions related to Facebook Graphs due to scarcity of data and bandwidth limitations. We still aim for applying our program to Facebook graphs, but the graph size will be limited by what we can mine in a minute with a single account, around 500 vertices. This way we can live demo the algorithm on any user's friends. 

Mining a large facebook graph will be our stretch goal, as it can be an interesting challenge to combine from several accounts in efforts to mine together in parallel.

As for Closeness Centrality algorithm, the goal stays the same: we're expecting our parallel implementation to have significant speedup over a sequential one.

More specifically, the goals are the following:
1. Implement parallel versions of both F-W and Dijkstra's with OpenMP.
2. Implement parallel F-W with MPI.
3. Test all on large graphs from online libraries.
3. Select one to compete with implementations available online.
4. Compile graphs with runtime vs. # of cores.
5. Provide a feature to get closeness centrality for given user's small Facebook network.

## Challenges
We don't know yet which algorithm will work best and how well it will compare against the implementations available online. We have 2 approaches - F-W and Dijkstra's - and parallelization technologies - OpenMP and MPI. Using OpenMP isn't going to be as trivial as just adding a pragma, there's synchronization needed as threads will have R-W conflicts. Dijkstra has 2 axises of parallelization: within itself and outside, as it's a SSSP algorithm.

There are two other aspects whose implementation details we don't know yet: benchmarking and ranking facebook graph edges (friendship). For benchmarking the challenges are adapting online graphs (they're usually not weighted), figuring out the impacts of cache and locality. For ranking facebook edges, we need to develop a metric that would capture friendship closeness. 

## Schedule

| Time Period  | Things to do |
| ------------ | ------------- |
| 4/24-4/29 | Aditya: Find best parallel Floyd-Warshall implementation; Anton: Find best parallel Dijkstra's implementation. |
| 4/30-5/1  | Exam 2.
| 5/2-5/5   | Both: Compare all approaches, select which approaches to proceed with; |
| 5/6-5/8   | Aditya: Optimize selected approach. Anton: Implement edge weighing and graph mining for Facebook. |
| 5/9-5/12  | Both: Generate final results, write final report. |

[jekyll-gh]: https://github.com/mojombo/jekyll
[jekyll]:    http://jekyllrb.com
