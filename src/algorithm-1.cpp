// OpenMP header
#include <omp.h>
#include <iostream>
#include <vector>
#include <utility>
#include <limits.h>
#include <map>
#include <set>
#include <queue>
#include <fstream>


using namespace std;

const int MAX_NODES = (int)(1e4);

int N, M;

const int INF = INT_MAX;

typedef struct graph{
    
    map<pair<int, int>, int> edges;
    vector<int> adjacency_list[MAX_NODES+1];
    
}Graph;

int DIST_S[MAX_NODES +1] = {INF};
int DIST_D[MAX_NODES +1] = {INF};

bool visited[MAX_NODES] = {false};

set<pair<int, int> > EDGES_ON_ANY_SHORTEST_PATH;

inline void add_edge(Graph graph, int U, int V, int weight){
    
    graph.adjacency_list[U].push_back(V);
    graph.adjacency_list[V].push_back(U);
    
    graph.edges[make_pair(U,V)] = weight;
}

typedef struct node{
    
    int U;
    int distance;
    
    bool operator < (const node& n)const{
        
        if(n.distance < distance){
            return true;
        }    
        return false;
    }
}node;

priority_queue<node> que;  

inline void Dijkstra(int S, int* Dist, Graph graph){
    
    while(!que.empty()){
        que.pop();
        
    }
    for(int i=0;i<=N;i++){
        
        Dist[i] = INF;
        visited[i] = false;
    }
    
    node sourceNode;
    sourceNode.U = S;
    sourceNode.distance = 0;
    Dist[S] = 0;
    que.push(sourceNode);
    
    while(!que.empty()){
        
        node poppedNode = que.top();
        que.pop();
        
        if(visited[poppedNode.U] == true){
            continue;
        }
        
        visited[poppedNode.U] = true;
        
        for(size_t i=0;i<graph.adjacency_list[poppedNode.U].size();i++){
            
            if(visited[graph.adjacency_list[poppedNode.U][i]] == false){
                
                node newNode;
                newNode.U = graph.adjacency_list[poppedNode.U][i];
                
                int edge_weight = graph.edges[make_pair(poppedNode.U, newNode.U)];
                
                if(Dist[newNode.U] > (Dist[poppedNode.U] + edge_weight)){
                    
                    Dist[newNode.U] = Dist[poppedNode.U] + edge_weight;
                    newNode.distance = Dist[newNode.U];
                    que.push(newNode);
                }
            }
        }
    }
}




inline void initialize_transpose_graph(Graph* transpose_graph, Graph* original_graph){
    
    map<pair<int, int>, int> :: iterator it = original_graph->edges.begin();
    
    for(it;it!=original_graph->edges.end();it++){
        
        transpose_graph->edges[make_pair((it->first).second, (it->first).first)] = it->second;
    }
    
    for(int i=1;i<=N;i++){
        
        for(int j=0;j<original_graph->adjacency_list[i].size();j++){
            
            transpose_graph->adjacency_list[original_graph->adjacency_list[i][j]].push_back(i);
        }
    }  
}

//Node 1 = Source (S)
//Node N = Destination (D)
inline void initialize_original_graph(Graph* original_graph, Graph* transpose_graph, string filename){
    ifstream  fin;
    fin.open(filename);
    
    fin >> N >> M;
    
    int U, V, W;
    for(int i=1;i<=M;i++){
        
        fin >> U >> V >> W;
        
        original_graph->adjacency_list[U].push_back(V);
        original_graph->edges[make_pair(U, V)] = W;
        
        transpose_graph->adjacency_list[U].push_back(V);
        transpose_graph->edges[make_pair(U, V)] = W;  
    }
}




int main(int argc, char** argv){
    
    double t1, t2;
    
    t1 = omp_get_wtime();

		Graph original_graph;
    	Graph transpose_graph;
    
    	initialize_original_graph(&original_graph, &transpose_graph, argv[1]);
    	    
        Dijkstra(1, &DIST_S[0], original_graph);
        Dijkstra(N, &DIST_D[0], transpose_graph);
    
        map<pair<int, int>, int> :: iterator it = original_graph.edges.begin();
    
        for(it;it!=original_graph.edges.end();it++){
        
            if((DIST_S[(it->first).first] + it->second + DIST_D[(it->first).second]) == DIST_S[N]){
            
                EDGES_ON_ANY_SHORTEST_PATH.insert(make_pair((it->first).first, (it->first).second));
            }
        }
    
    t2 = omp_get_wtime();
    
    for(set<pair<int, int> > :: iterator it = EDGES_ON_ANY_SHORTEST_PATH.begin(); it!=EDGES_ON_ANY_SHORTEST_PATH.end();it++){
        
        int U = get<0>(*it);
        int V = get<1>(*it);
        int edge_weight = original_graph.edges[make_pair(U, V)];
        
        cout<<U << " " << V << " " << edge_weight << endl;       
    }
    printf("Time = %g\n", t2-t1);
    
    return 0;
}

