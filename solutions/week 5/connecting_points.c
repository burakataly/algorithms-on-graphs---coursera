#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	int parent;
	int rank;
}node;

typedef struct {
	int u;
	int v;
	double weight;
}EDGE;

void makeSet(node* nodes, int n, int i);
int find(node* nodes, int n, int i);
int setUnion(node* nodes, int n, int i, int j);
void buildHeap(EDGE* heap, int n);
EDGE extractMin(EDGE* heap, int* n);
void siftDown(EDGE* heap, int n, int i);
void kruskal(EDGE* edges, int E, int V);
double dist(int x1, int y1, int x2, int y2);

int main(){
	int V, E, i, j, k, *x, *y;
	EDGE* edges;
	
	scanf("%d", &V);
	x = (int*) malloc(V * sizeof(int));
	y = (int*) malloc(V * sizeof(int));
	E = (V * (V - 1)) / 2;
	edges = (EDGE*) malloc(E * sizeof(EDGE));
	
	for(i=0;i<V;i++){
		scanf("%d %d", &x[i], &y[i]);
	}
	
	k=0;
	for(i=0;i<V-1;i++){
		for(j=i+1;j<V;j++){
			edges[k].u = i;
			edges[k].v = j;
			edges[k].weight = dist(x[i], y[i], x[j], y[j]);
			k++;
		}
	}
	
	kruskal(edges, E, V);
	
	return 0;
}

double dist(int x1, int y1, int x2, int y2){
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

void kruskal(EDGE* edges, int E, int V){
	int i;
	double totalWeight = 0.0;
	EDGE temp;
	node* nodes = (node*) malloc(V * sizeof(node));
	
	for(i=0;i<V;i++) makeSet(nodes, V, i);
	buildHeap(edges, E);
	
	i = 0;
	while(i < V - 1){ // MST has V - 1 edges so when we select the V - 1 th edge we are done 
		temp = extractMin(edges, &E);
		if(setUnion(nodes, V, temp.u, temp.v)){
			totalWeight += temp.weight;
			i++;
		}
	}
	
	printf("%.9lf", totalWeight);
}

void makeSet(node* nodes, int n, int i){
	if(i < 0 || i >= n) return;
	nodes[i].parent = i;
	nodes[i].rank = 0;
}

int find(node* nodes, int n, int i){
	if(i < 0 || i >= n) return;
	if(i != nodes[i].parent) nodes[i].parent = find(nodes, n, nodes[i].parent);
	return nodes[i].parent;
}

int setUnion(node* nodes, int n, int i, int j){
	if(i < 0 || i >= n || j < 0 || j >= n) return;
	int id1 = find(nodes, n, i);
	int id2 = find(nodes, n, j);
	if(id1 == id2) return 0;
	if(nodes[id1].rank > nodes[id2].rank){
		nodes[id2].parent = id1;
	}
	else{
		nodes[id1].parent = id2;
		if(nodes[id1].rank == nodes[id2].rank){
			nodes[id2].rank++;
		}
	}
	return 1;
}

void buildHeap(EDGE* heap, int n){
	int i;
	for(i=n/2;i>=0;i--) siftDown(heap, n, i);
}

EDGE extractMin(EDGE* heap, int* n){
	EDGE result = heap[0];
	heap[0] = heap[*n - 1];
	(*n)--;
	siftDown(heap, *n, 0);
	return result;
}

void siftDown(EDGE* heap, int n, int i){
	int index = i;
	EDGE temp;
	
	do{
		i = index;
		if(2 * i + 1 < n && heap[2 * i + 1].weight < heap[index].weight){
			index = 2 * i + 1;
		}
		if(2 * i + 2 < n && heap[2 * i + 2].weight < heap[index].weight){
			index = 2 * i + 2;
		}
		temp = heap[i];
		heap[i] = heap[index];
		heap[index] = temp;
	} while(i != index);
}
