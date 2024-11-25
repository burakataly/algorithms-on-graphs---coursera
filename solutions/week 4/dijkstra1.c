#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct node{
	int vertex;
	int weight;
	struct node* next;
}NODE;

typedef struct graph{
	int numOfNodes;
	NODE** adjList;
}GRAPH;

typedef struct dist{
	int d;
	int vertex;
}DIST;

NODE* createNode(int, int);
GRAPH* createGraph(int);
void addEdges(GRAPH*, int, int, int);
int hasEdge(GRAPH*, int, int);
void printAdjList(GRAPH* graph);
DIST* makeHeap(int n, int* dist);
void siftUp(DIST* heap, int i);
void siftDown(DIST* heap, int n, int i);
DIST extractMin(DIST* heap, int* n);
void changePriority(DIST* heap, int n, int i, int p);
void dijkstra(GRAPH* graph, int src, int dest);

int main(){
	int n, m, i, u, v, w, src, dest;
	
	scanf("%d %d", &n, &m);
	GRAPH* graph = createGraph(n);
	
	for(i=0;i<m;i++){
		scanf("%d %d %d", &u, &v, &w);
		addEdges(graph, u, v, w);
	}
	scanf("%d %d", &src, &dest);
	
	dijkstra(graph, src, dest);
	
	return 0;
}

void dijkstra(GRAPH* graph, int src, int dest){
	int n = graph -> numOfNodes, i, j, u, minDist;
	
	int* label = (int*) calloc(n, sizeof(int));
	int* dist = (int*) malloc(n * sizeof(int));
	for(i=0;i<n;i++) dist[i] = INT_MAX;
	dist[src-1] = 0;
	
	NODE* temp; 
	
	//finding the minimum distance node out of unknown region
	for(i=0;i<n;i++){
		minDist = INT_MAX;
		for(j=0;j<n;j++){
			if(!label[j] && dist[j] < minDist){
				minDist = dist[j];
				u = j;
			}
		}
		label[u] = 1; //adding found node to known region because it is its correct distance. it cannot be less than that
		
		temp = graph -> adjList[u];
		while(temp != NULL){
			if(dist[temp -> vertex - 1] > dist[u] + temp -> weight){
				dist[temp -> vertex - 1] = dist[u] + temp -> weight;
			}
			temp = temp -> next;
		}
	}
	
	if(dist[dest-1] == INT_MAX) printf("%d", -1);
	else printf("%d", dist[dest -1]);
	
	free(label);
	free(dist);
}

//her bir node için yalnýzca son durak olan nodelarý kaydettiðimizden -1 görene kadar, yani srcye gelene kadar, ilerleyip sonra yazdýrmaya baþlýyoruz.
void printRoute(int* route, int dest){
	if(route[dest] == -1){
		printf("%d", dest);
		return;
	}
	printRoute(route, route[dest]);
	printf(" -> %d", dest);
}

NODE* createNode(int vertex, int weight){
	NODE* node = (NODE*) malloc(sizeof(NODE));
	node -> vertex = vertex;
	node -> weight = weight;
	node -> next = NULL;
	return node;
}

GRAPH* createGraph(int n){
	int i;
	GRAPH* graph = (GRAPH*) malloc(sizeof(GRAPH));
	graph -> numOfNodes = n;
	graph -> adjList = (NODE**) calloc(n, sizeof(NODE*));
	return graph;
}

void addEdges(GRAPH* graph, int src, int dest, int weight){
	if(src == dest || src > graph -> numOfNodes || dest > graph -> numOfNodes) return;
	if(hasEdge(graph, src, dest)) return;
	
	// Add edge from src to dest
	NODE* newNode = createNode(dest, weight);
	newNode->next = graph->adjList[src-1];
	graph->adjList[src-1] = newNode;
	
	// Add edge from dest to src
//	newNode = createNode(src, weight);
//	newNode->next = graph->adjList[dest-1];
//	graph->adjList[dest] = newNode;
}

int hasEdge(GRAPH* graph, int src, int dest){
	NODE* temp = graph -> adjList[src-1];
	while(temp != NULL){
		if(temp -> vertex == dest) return 1;
		temp = temp -> next;
	}
	return 0;
}

void printAdjList(GRAPH* graph){
	int i, n = graph -> numOfNodes;
	
	for(i=0; i<n; i++) {
	    printf("%d -> ", i+1); // hangi node'un komþularý.
	    NODE* temp = graph -> adjList[i];
	    while(temp != NULL) {
	        printf("(%d, w: %d), ", temp -> vertex, temp -> weight);
	        temp = temp -> next;
	    }
	    printf("\n");
	}
}
