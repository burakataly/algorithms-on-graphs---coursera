#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct node{
	int vertex;
	int weight;
	struct node* next;
};

typedef struct node NODE;

struct graph{
	int numOfNodes;
	NODE** adjList;
};

typedef struct graph GRAPH;

typedef struct n{
	int x;
	struct n* next;
}STACK;

void push(STACK**, int);
int pop(STACK**);

NODE* createNode(int vertex, int weight);
GRAPH* createGraph(int);
void addEdges(GRAPH*, int, int, int);

void depthFirstSearch1(GRAPH*, int*, int, STACK**);
void depthFirstSearch2(GRAPH*, int*, int, int);
GRAPH* transpose(GRAPH*);
int stronglyConnectedComponents(GRAPH*, int*);
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
	long long *dist = (long long*) malloc(V * sizeof(long long));
	int *label = (int*) calloc(V, sizeof(int));
	NODE* temp;
	stronglyConnectedComponents(graph, label);
	
	for(i=0;i<V;i++){
		dist[i] = INT_MAX;
	}
	
	dist[src] = 0;
	
	for(i=0;i<V-1;i++){
		for(j=0;j<V;j++){
			temp = graph->adjList[j];
			while(temp != NULL){
				if(dist[temp->vertex] > dist[j] + temp->weight){
					dist[temp->vertex] = dist[j] + temp->weight;
				}
				temp = temp->next;
			}
		}
	}
	
	//to look whether there is negative cycle
	for(j=0;j<V;j++){
		temp = graph->adjList[j];
		while(temp != NULL){
			if(dist[temp->vertex] > dist[j] + temp->weight){
				dist[temp->vertex] = INT_MIN;
			}
			temp = temp->next;
		}
	}
	
	for(i=0;i<V;i++){
		if(label[i] != label[src]) printf("*\n");
		else if(dist[i] == INT_MIN) printf("-\n");
		else printf("%lld\n", dist[i]);
	}
	
	free(dist);
}

int stronglyConnectedComponents(GRAPH* graph, int* label){
	int i, labeler = 1;
	STACK* head = 0;
	GRAPH* trans = transpose(graph);
	
	//stacki doldurmak için olan DFS. O(V + E) 
	for(i=0;i<graph->numOfNodes;i++){
		if(label[i] == 0){
			depthFirstSearch1(graph, label, i, &head);
		}
	}
	
	for(i=0;i<graph->numOfNodes;i++) label[i] = 0;
	
	//etiketlemek için olan DFS. Baþlangýç nodelarýný burada stackten veriyoruz ama recursive çaðrýlarda normalde CC bulurken yaptýðýmýz gibi
	//tüm komþularýný geziyoruz. O(V + E) 
	while(head != NULL){
		i = pop(&head);
		if(label[i] == 0){
			depthFirstSearch2(trans, label, i, labeler++);
		}
	}
	
	free(label);
	
	return labeler-1; // graphta kaç tane SCC olduðunu döndürür.
}

void depthFirstSearch1(GRAPH* graph, int* label, int start, STACK** head){
	NODE* temp = graph -> adjList[start];
	
	label[start] = 1;
	while(temp != NULL){
		if(label[temp -> vertex] == 0){
			depthFirstSearch1(graph, label, temp -> vertex, head);
		}
		temp = temp -> next;
	}
	
	push(head, start);
}

void depthFirstSearch2(GRAPH* graph, int* label, int start, int labeler){
	NODE* temp = graph -> adjList[start];
	
	label[start] = labeler;
	while(temp != NULL){
		if(label[temp -> vertex] == 0){
			depthFirstSearch2(graph, label, temp -> vertex, labeler);
		}
		temp = temp -> next;
	}
}

// O(E)
GRAPH* transpose(GRAPH* graph){
	GRAPH* trans = createGraph(graph->numOfNodes);
	NODE* temp;
	int i;
	for(i=0;i<graph->numOfNodes;i++){
		temp = graph->adjList[i];
		while(temp != NULL){
			addEdges(trans, temp->vertex, i, temp->weight);
			temp = temp->next;
		}
	}
	return trans;
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

void push(STACK **head, int i){
	STACK *temp = (STACK*) malloc(sizeof(STACK));
	temp -> x = i;
	temp -> next = *head;
	*head = temp;
}

int pop(STACK **head){
	if(*head != NULL){
		STACK *temp = *head;
		*head = (*head) -> next;
		int a = temp -> x;
		free(temp);
		return a;
	}
}

