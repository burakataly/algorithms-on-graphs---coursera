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
		addEdges(graph, u-1, v-1, w);
	}
	scanf("%d %d", &src, &dest);
	
	dijkstra(graph, src-1, dest-1);
	
	return 0;
}

void dijkstra(GRAPH* graph, int src, int dest){
	int n = graph -> numOfNodes, i;
	int* dist = (int*) malloc(n * sizeof(int));
	
	for(i=0;i<n;i++) dist[i] = INT_MAX;
	dist[src] = 0;
	
	DIST* heap = makeHeap(n, dist);
	DIST u;
	u = heap[src];
	heap[src] = heap[0];
	heap[0] = u;
	NODE* temp;
	
	while(n){
		u = extractMin(heap, &n);
		temp = graph -> adjList[u.vertex];
		while(temp != NULL && u.d != INT_MAX){
			if(dist[temp -> vertex] > dist[u.vertex] + temp -> weight){
				dist[temp -> vertex] = dist[u.vertex] + temp -> weight;
				changePriority(heap, n, temp -> vertex, dist[temp -> vertex]);
			}
			temp = temp -> next;
		}
	}
	
	if(dist[dest] == INT_MAX) printf("%d", -1);
	else printf("%d", dist[dest]);
	
	free(dist);
}

DIST* makeHeap(int n, int* dist){
	DIST* heap = (DIST*) malloc(n * sizeof(DIST));
	int i;
	for(i=0;i<n;i++){
		heap[i].d = dist[i];
		heap[i].vertex = i;
	}
	
	return heap;
}

void siftUp(DIST* heap, int i){
	DIST temp;
	while(i){
		if(heap[i].d < heap[(i - 1) / 2].d){
			temp = heap[i];
			heap[i] = heap[(i - 1) / 2];
			heap[(i - 1) / 2] = temp;
		}
		i = (i - 1) / 2;
	}
}

void siftDown(DIST* heap, int n, int i){
	int index = i;
	DIST temp;
	
	do{
		i = index;
		if(2 * i + 1 < n && heap[2 * i + 1].d < heap[index].d){
			index = 2 * i + 1;
		}
		if(2 * i + 2 < n && heap[2 * i + 2].d < heap[index].d){
			index = 2 * i + 2;
		}
		temp = heap[i];
		heap[i] = heap[index];
		heap[index] = temp;
	} while(i != index);
}

DIST extractMin(DIST* heap, int* n){
	DIST result = heap[0];
	heap[0] = heap[*n - 1];
	(*n)--;
	siftDown(heap, *n, 0);
	return result;
}

void changePriority(DIST* heap, int n, int vertex, int p){
	int i = 0;
	while(i < n && heap[i].vertex != vertex) i++;
	if(i == n || heap[i].d == p) return;
	int oldP = heap[i].d;
	heap[i].d = p;
	if(p < oldP) siftUp(heap, i);
	else siftDown(heap, n, i);
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
	if(src == dest || src >= graph -> numOfNodes || dest >= graph -> numOfNodes) return;
	if(hasEdge(graph, src, dest)) return;
	
	// Add edge from src to dest
	NODE* newNode = createNode(dest, weight);
	newNode->next = graph->adjList[src];
	graph->adjList[src] = newNode;
	
	// Add edge from dest to src
//	newNode = createNode(src, weight);
//	newNode->next = graph->adjList[dest];
//	graph->adjList[dest] = newNode;
}

int hasEdge(GRAPH* graph, int src, int dest){
	NODE* temp = graph -> adjList[src];
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
	        printf("(%d, w: %d), ", temp -> vertex + 1, temp -> weight);
	        temp = temp -> next;
	    }
	    printf("\n");
	}
}
