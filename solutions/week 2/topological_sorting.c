#include<stdio.h>
#include<stdlib.h>

struct node{
	int vertex;
	struct node* next;
};

typedef struct node NODE;

struct graph{
	int numOfNodes;
	NODE** adjList;
};

typedef struct graph GRAPH;

NODE* createNode(int);
GRAPH* createGraph(int);
void addEdges(GRAPH*, int, int);
int hasEdge(GRAPH*, int, int);

int* topologicalOrdering(GRAPH* graph);
int depthFirstSearch(GRAPH* graph, int* label, int* ordering, int start, int index);

int main(){
	int n, m, i, u, v;
	scanf("%d %d", &n, &m);
	GRAPH* graph = createGraph(n);
	int* label = (int*) calloc(n, sizeof(int));
	for(i=0;i<m;i++){
		scanf("%d %d", &u, &v);
		addEdges(graph, u-1, v-1);
	}
	int* ordering = topologicalOrdering(graph);
	
	for(i=0;i<n;i++) printf("%d ", ordering[i]);
	return 0;
}

int* topologicalOrdering(GRAPH* graph){
	int n = graph->numOfNodes;
	int* label = (int*) calloc(n, sizeof(int));
	int* ordering = (int*) malloc(n * sizeof(int));
	int i, index = n-1;
	
	for(i=0;i<n;i++){
		if(!label[i]){
			index = depthFirstSearch(graph, label, ordering, i, index);
		}
	}
	free(label);
	return ordering;
}

int depthFirstSearch(GRAPH* graph, int* label, int* ordering, int start, int index){
	if(index < 0) return;
	label[start] = 1;
	NODE* temp = graph->adjList[start];
	 
	while(temp != NULL){
		if(label[temp -> vertex] == 0){
			index = depthFirstSearch(graph, label, ordering, temp -> vertex, index);
		}
		temp = temp -> next;
	}
	ordering[index--] = start+1;
	return index;
}

NODE* createNode(int vertex){
	NODE* node = (NODE*) malloc(sizeof(NODE));
	node -> vertex = vertex;
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

void addEdges(GRAPH* graph, int src, int dest){
	if(src == dest || src >= graph->numOfNodes || dest >= graph->numOfNodes) return;
	if(hasEdge(graph, src, dest)) return;
	
	NODE* newNode = createNode(dest);
	newNode->next = graph->adjList[src];
	graph->adjList[src] = newNode;
}

int hasEdge(GRAPH* graph, int src, int dest){
	NODE* temp = graph -> adjList[src];
	while(temp != NULL){
		if(temp -> vertex == dest) return 1;
		temp = temp -> next;
	}
	return 0;
}
