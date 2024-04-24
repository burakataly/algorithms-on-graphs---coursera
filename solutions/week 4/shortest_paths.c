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

typedef struct n{
	int x;
	struct n* next;
}qnode;

typedef struct q{
    qnode* head;
    qnode* tail;
}QUEUE;

void push(QUEUE* queue, int i);
int pop(QUEUE* queue);
NODE* createNode(int vertex, int weight);
GRAPH* createGraph(int);
void addEdges(GRAPH*, int, int, int);
void bellmanFord(GRAPH* graph, int src);

int main(){
	int n, m, i, u, v, w, src;
	scanf("%d %d", &n, &m);
	GRAPH* graph = createGraph(n);
	
	for(i=0;i<m;i++){
		scanf("%d %d %d", &u, &v, &w);
		addEdges(graph, u-1, v-1, w);
	}
	
	scanf("%d", &src);
	
	bellmanFord(graph, src - 1);	
	return 0;
}

void bellmanFord(GRAPH* graph, int src){
	int i, j, V = graph->numOfNodes;
	long long maxDist = 1000000000001; //there is maximum 10^3 node and maximum weight is 10^9 so max dist can be at most (10^3 - 1) * 10^9
	long long *dist = (long long*) malloc(V * sizeof(long long));
	int *isVisited = (int*) calloc(V, sizeof(int)); //to label the vertices of any infinite arbitrage
	int* reachable = (int*) calloc(V, sizeof(int)); //to label the vertices reachable from source vertex 
	QUEUE* queue = (QUEUE*) calloc(1, sizeof(QUEUE)); //to use it in BFS
	NODE* temp;
	
	for(i=0;i<V;i++){
		dist[i] = maxDist;
	}
	
	dist[src] = 0;
	reachable[src] = 1;
	
	for(i=0;i<V-1;i++){
		for(j=0;j<V;j++){
			temp = graph->adjList[j];
			while(temp != NULL){
				if(dist[j] != maxDist && dist[temp->vertex] > dist[j] + temp->weight){
					dist[temp->vertex] = dist[j] + temp->weight;
					reachable[temp->vertex] = 1;
				}
				temp = temp->next;
			}
		}
	}
	
	//to look whether there is negative cycle
	for(j=0;j<V;j++){
		temp = graph->adjList[j];
		while(temp != NULL){
			if(dist[j] != maxDist && dist[temp->vertex] > dist[j] + temp->weight){
				push(queue, temp->vertex);
			}
			temp = temp->next;
		}
	}
	
	//BFS to find the vertices of infinite arbitrage
	while(!isEmpty(queue)){
		i = pop(queue);
		isVisited[i] = 1;
		temp = graph->adjList[i];
		while(temp != NULL){
			if(!isVisited[temp->vertex]){
				push(queue, temp->vertex);
			}
			temp = temp->next;
		}
	}
	
	for(i=0;i<V;i++){
		if(isVisited[i]) printf("-\n");
		else if(!reachable[i]) printf("*\n");
		else printf("%lld\n", dist[i]);
	}
	
	free(dist);
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
	if(src == dest || src >= graph->numOfNodes || dest >= graph->numOfNodes) return;
	
	NODE* newNode = createNode(dest, weight);
	newNode->next = graph->adjList[src];
	graph->adjList[src] = newNode;
}

void push(QUEUE* queue, int i){
	qnode *temp = (qnode*) malloc(sizeof(qnode));
	temp -> x = i;
	temp -> next = NULL;
	if(queue -> tail == NULL){
		queue -> tail = temp;
		queue -> head = temp;
	}
	else{
		queue -> tail -> next = temp;
		queue -> tail = temp;
	}
}

int pop(QUEUE* queue){
	if(queue -> head != NULL){
        int result = queue -> head -> x;
        qnode *temp = queue -> head;
        queue -> head = queue -> head -> next;
        if(queue -> head == NULL) queue -> tail = NULL;
        free(temp);
        return result;
    }
}

int isEmpty(QUEUE* queue){
	return queue->head == NULL && queue->tail == NULL;
}
