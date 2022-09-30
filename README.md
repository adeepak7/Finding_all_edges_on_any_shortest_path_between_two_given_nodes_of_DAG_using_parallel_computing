# Finding all edges on any shortest path between two given nodes of DAG using parallel computing

Implementation of the algorithm proposed in the research article **"Finding all edges on any smallest route connecting two given nodes of DAG using parallel computing"**. This artcile is accepted at the **9th International Conference on Innovations in Computer Science and Engineering (ICICSE) 2021**, Hyderabad.

The parallel version of Dijkstra's algorithm which is implemented using the adjacency list and the OpenMP is also proposed in the article **"An Autonomous Intelligent System to Leverage the PostHarvest Agricultural Process using Localization and Mapping"**. This article is accepted at the **International Conference Intelligent Systems and Sustainable Computing (ICISSC) 2021**, Hyderabad.


### Citation (Bibtex format)

```
@InProceedings{10.1007/978-981-16-8987-1_73,
author="Ahire, Deepak
and Kamble, Kiran
and Jadhav, Omkar
and Katakdhond, Shubham",
editor="Saini, H. S.
and Sayal, Rishi
and Govardhan, A.
and Buyya, Rajkumar",
title="Finding All Edges on any Smallest Route Connecting Two Nodes of a Directed Acyclic Graph Using Parallel Computing",
booktitle="Innovations in Computer Science and Engineering",
year="2022",
publisher="Springer Singapore",
address="Singapore",
pages="683--695",
abstract="Algorithms proposed in this article aim to find all edges on any smallest route connecting two nodes of a directed acyclic graph (DAG). However, for a DAG with N nodes, the number of routes possible connecting any two nodes can be exponential. Hence, it is unfeasible to compute all the routes to attain the smallest ones in the polynomial-time and eventually produce a set of all edges that contribute or make any of the smallest routes. This article presents all the existing approaches to solve the problem, and we propose a new technique to compute the required edges by taking advantage of parallel computing using the C++ OpenMP API by parallelizing one of the existing approaches. We present a step-by-step procedure to convert a sequential algorithm into a parallel algorithm by identifying the parallelizable tasks. We also took advantage of parallel Dijkstra's by using the nested parallelism feature of OpenMP and presented a technique for deciding the optimal number of threads to run the tasks that involve unbounded parallelism. Using parallel computing, we could achieve a speedup of over 13{\%} for a complete graph. The techniques proposed here are not confined only to this particular use case. However, these have a more extensive scope in dynamic programming (DP), graph theory, and counting problems pf.",
isbn="978-981-16-8987-1"
}
```
