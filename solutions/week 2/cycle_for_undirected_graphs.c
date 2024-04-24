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

int depthFirstSearch(GRAPH*, int*, int, int);

int main(){
	int n, m, i, u, v, ctrl = 0;
	scanf("%d %d", &n, &m);
	GRAPH* graph = createGraph(n);
	int* label = (int*) calloc(n, sizeof(int));
	for(i=0;i<m;i++){
		scanf("%d %d", &u, &v);
		addEdges(graph, u-1, v-1);
	}
	
	i=0;
	while(i < n && !ctrl){
		if(!label[i]){
			if(depthFirstSearch(graph, label, i, -1)) ctrl = 1;
		}
		i++;
	}
	
	(ctrl) ? printf("1") : printf("0");
	
	return 0;
}

int depthFirstSearch(GRAPH* graph, int* label, int start, int parent){
	NODE* temp = graph -> adjList[start];
	
	label[start] = 1;
	
	while(temp != NULL){
		if(!label[temp -> vertex]){
			if(depthFirstSearch(graph, label, temp -> vertex, start)) return 1;
		}
		else if(temp->vertex != parent) return 1;
		temp = temp -> next;
	}
	return 0;
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
	
	// Add edge from src to dest
	NODE* newNode = createNode(dest);
	newNode->next = graph->adjList[src];
	graph->adjList[src] = newNode;
	
	newNode = createNode(src);
	newNode->next = graph->adjList[dest];
	graph->adjList[dest] = newNode;
}

int hasEdge(GRAPH* graph, int src, int dest){
	NODE* temp = graph -> adjList[src];
	while(temp != NULL){
		if(temp -> vertex == dest) return 1;
		temp = temp -> next;
	}
	return 0;
}
