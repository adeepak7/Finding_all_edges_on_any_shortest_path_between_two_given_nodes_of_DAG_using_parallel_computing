// OpenMP header
#include <omp.h>
#include <iostream>
#include <vector>
#include <utility>
#include <limits.h>
#include <map>
#include <queue>
#include <set>
#include <cmath>
#include <iterator>
#include <assert.h>
#include <fstream>
#include <utility>
#include <string>

using namespace std;

const int MAX_NODES = (int)(1e4);
const int INF = INT_MAX;

int N, M;
    
typedef struct graph{
    
    map<pair<int, int>, int> edges;
    vector<int> adjacency_list[MAX_NODES+1];
    
}Graph;

int DIST_S[MAX_NODES+1] = {INF};
int DIST_D[MAX_NODES+1] = {INF};

bool visited[MAX_NODES] = {false};

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

priority_queue<node> sharedQueue;  

set<pair<int, int> > EDGES_ON_ANY_SHORTEST_PATH;


inline void Dijkstra(int S, int* Dist, Graph graph, int number_of_cores){
    
    for(int i=1; i<=N; i++){

		Dist[i] = INF;
		visited[i] = false;
	}
	
    
    while(!sharedQueue.empty()){
		sharedQueue.pop();
	}

	node sourceNode;
	sourceNode.U = S;
	sourceNode.distance = 0;
	Dist[S] = 0;
	sharedQueue.push(sourceNode);

	while(!sharedQueue.empty()){

		node poppedNode = sharedQueue.top();
		sharedQueue.pop();

		if(visited[poppedNode.U] == true){
			continue;
		}

		visited[poppedNode.U] = true;


		priority_queue<node> privateQueue;
		int tid;
    	
    	//printf("Cores: %d\n", number_of_cores);
    	
		#pragma omp parallel num_threads(number_of_cores) default(shared) private(tid, privateQueue)
		{		
			tid = omp_get_thread_num();
			
			int adj_size = graph.adjacency_list[poppedNode.U].size();
			
			int step = (int)ceil((double)adj_size / (double)number_of_cores);
            
            int parts = (int)ceil((double)adj_size / (double)step);
            
            int from = tid*step;
            int to = (tid+1)*step;
            
            if(tid+1 == parts){
                to = adj_size;
            }
            
            //printf("TID: %d, From: %d, to: %d\n", tid, from, to);
            
			for(int i=from;i<to;i++){
				
				if(visited[graph.adjacency_list[poppedNode.U][i]] == false){
					
					node newNode;
					newNode.U = graph.adjacency_list[poppedNode.U][i];
					
					int edge_weight = graph.edges[make_pair(poppedNode.U, newNode.U)];
					
					if(Dist[newNode.U] > (Dist[poppedNode.U] + edge_weight)){
						
						Dist[newNode.U] = Dist[poppedNode.U] + edge_weight;
						newNode.distance = Dist[newNode.U];
						privateQueue.push(newNode);
					}
				}
			}
			
			#pragma omp critical
			{
				while(!privateQueue.empty()){
					
					node topNode = privateQueue.top();
					privateQueue.pop();
					sharedQueue.push(topNode);
				}
			}
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
    
    omp_set_nested(1);
    
    t1 = omp_get_wtime();
    
	    Graph original_graph;
	    Graph transpose_graph;
	    
	    initialize_original_graph(&original_graph, &original_graph, argv[1]);
	    
	    
	    int tid;
	    int number_of_cores = stoi(argv[2]);
    
        
        #pragma omp parallel num_threads(2) default( shared ) private ( tid ) 
        {
            
            tid = omp_get_thread_num();
        
            if(tid == 0){
                Dijkstra(1, &DIST_S[0], original_graph, number_of_cores);
            }
            else{
                Dijkstra(N, &DIST_D[0], transpose_graph, number_of_cores);
            }
        }

        
        #pragma omp parallel num_threads(number_of_cores) default( shared ) private ( tid ) 
        {
            
            tid = omp_get_thread_num();
            
            int number_of_edges = original_graph.edges.size();
            
            int step = (int)ceil((double)number_of_edges / (double)number_of_cores);
            
            int parts = (int)ceil((double)number_of_edges / (double)step);
            
            map<pair<int, int>, int> :: iterator FROM = original_graph.edges.begin();

		    advance(FROM, (tid * step));
            
            map<pair<int, int>, int> :: iterator TO = original_graph.edges.begin();
            
            advance(TO, (tid+1) * step);
            
            if(tid+1 == parts){
                TO = original_graph.edges.end();
            }
            
            set<pair<int, int> > RESPECTIVE_PROCESSED_EDGES;
            
            for(map<pair<int, int>, int> :: iterator it = FROM; it != TO ;it++){
            
                if((DIST_S[(it->first).first] + it->second + DIST_D[(it->first).second]) == DIST_S[N]){
                    
                    pair<int,int> requiredEdge = make_pair((it->first).first, (it->first).second);
                    RESPECTIVE_PROCESSED_EDGES.insert(requiredEdge);
                }
            }
            
            //Checking equal distribution of work (work = number of edges processed by particular thread) by assert statement
            
            assert(RESPECTIVE_PROCESSED_EDGES.size() <= step);
            
            #pragma omp critical
            {
                
                for(set<pair<int, int> > :: iterator it = RESPECTIVE_PROCESSED_EDGES.begin(); it!=RESPECTIVE_PROCESSED_EDGES.end();it++){
                    
                    EDGES_ON_ANY_SHORTEST_PATH.insert(*it);
                }    
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





