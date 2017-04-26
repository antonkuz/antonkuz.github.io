---
layout: default
title:  "Checkpoint"
date:   2017-04-25 04:20:00
categories: main
---

# Checkpoint

## Progress
We started off by checking out Facebook API. Facebook provides the so-called Graph API, with which you communicate via RESTful API and get JSON formatted data with friends, posts, likes, etc. As we pulled the friends data, however, we discovered that it only displays the friends that have used the API. Facebook changed privacy settings for the API a couple years ago and now this is the restriction which app developers have no way to go around. We observed that on average less than 10% of friends are available. On top of the privacy issue, pulling the data took a long time: it took 10 minutes to get 1000 vertices (users). We haven't benchmarked to figure out what the exact slowdown cause was (script was written in Python), but it's likely the the API's bandwidth limit. 

Considering alternative plans, we have also checked Twitter API. This API had it's own drawbacks: while providing all followers, it limits the API calls to 15 requests per 15 minutes which makes it unfeasible for large graph mining.

These discoveries made us reconsider Facebook API plans (see next section).

Next, we looked into closeness centrality algorithm. Closeness centrality values are per vertex, and to get one we need the vertex's shortest path distances to all other vertices in a graph. For social network graphs, we would like to get a table of users sorted by closeness centrality, so we need to find shortest path distances for all pairs, a problem called All Pairs Shortest Paths (APSP). 

We chose two main approaches to APSP: Floyd-Warshall (F-W) and Dijkstra's. Dijkstra's is Single Source Shortest Path, but repeating it V times can still be faster than F-W which is specifically for APSP.

We've implemented simple sequential versions of F-W and Dijkstra's. With Dijkstra's there 

3) Parallel F-W and Dijkstra's thoughts
4) Started implementing the approach described in Phased Floyd-Warshall paper that uses MPI. This approach utlilizes blocking techniques to improve cache performance and at the same time ensuring lesser communication overheard when running in a distributed setting.
<!-- 5) need connected graphs
 -->
## Goals and Deliverables
1) Lowered priority of mining large real facebook. Reasons: speed slow, got 1000 people after 5-10 minutes of waiting. Didn't try too much to figure out what the exact issue was: whether it was slow python dictionary as it gets large, or facebook limits api request latency. Dunno whether it's limited by time or number. We also realized the larger the real graph is, the further we get from our connections and the less interesting the results are
2) We will try both OpenMP and MPI, or even Hybrid. We'll keep working on parallel F-W and dijkstra
3) producing performance graphs is a big aspect. 
4) benchmarking, getting real data
5) once we're done with parallel fw and dijkstra
6) parallel some other sssp

Describe how you are doing with respect to the goals and deliverables stated in your proposal. Do you still believe you will be able to produce all your deliverables? If not, why? What about the "nice to haves"? In your checkpoint writeup we want a new list of goals that you plan to hit for the Parallelism competition.
What do you plan to show at the parallelism competition? Will it be a demo? Will it be a graph?

## Concerns
The most challenging part as of now seems to be mining a large graph out of the facebook data. The API does allow us to use the data but there is a cap on number of requests that we can send. Also they only expose the data of users who are part of the developer community. These are prohibitive factors for mining a large graph from the real world facebook data. So we will have to do more research into how we can go about achieving that. We are keeping this as our stretch goal.

## Schedule

| Time Period  | Things to do |
| ------------ | ------------- |
| 4/24-4/29 | Aditya: Find best parallel Floyd-Warshall implementation; Anton: Find best parallel Dijkstra's implementation |
| 4/30-5/1  | Exam 2
| 5/2-5/4   | Both: Parse and test on large graphs, select which approaches to proceed with |
| 5/5-5/7   | Both: Divide up the approaches and optimize |
| 5/8-5/12  | Both: Generate final results, write final report. |

[jekyll-gh]: https://github.com/mojombo/jekyll
[jekyll]:    http://jekyllrb.com
