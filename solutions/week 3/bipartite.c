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

int breadthFirstSearch(GRAPH*, int*, int);

int main(){
	int n, m, i, u, v, ctrl = 1;
	scanf("%d %d", &n, &m);
	GRAPH* graph = createGraph(n);
	int* label = (int*) calloc(n, sizeof(int));
	
	for(i=0;i<m;i++){
		scanf("%d %d", &u, &v);
		addEdges(graph, u-1, v-1);
	}
	
	i=0;
	while(i < n && ctrl){
		if(!label[i]){
			if(!breadthFirstSearch(graph, label, i)) ctrl = 0;
		}
		i++;
	}
	
	(ctrl) ? printf("1") : printf("0");
	
	return 0;
}

int breadthFirstSearch(GRAPH* graph, int* label, int start){
	NODE* temp;
	int head = 0, tail = 0, current, n = graph->numOfNodes, size, i, color = 1;
	int* queue = (int*) malloc(n*sizeof(int));
	queue[tail++] = start;
	label[start] = color;
	while(head < tail){
		size = tail - head;
		for(i=0;i<size;i++){
			current = queue[head++];
			temp = graph -> adjList[current];
			while(temp != NULL){
				if(label[temp -> vertex] == 0){
					label[temp -> vertex] = color + 1;
					queue[tail++] = temp -> vertex;
				}
				else if(label[temp -> vertex] == label[current]) return 0;
				temp = temp -> next;
			}
		}
		color = (color + 1) % 2;
	}
	free(queue);
	return 1;
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
	
	// Add edge from dest to src
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
