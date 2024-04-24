#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
	int u;
	int v;
	int weight;
}EDGE;

int bellmanFord(EDGE* edges, int E, int V, int src);

int main(){
	int V, E, i;
	EDGE* edges;
	
	scanf("%d %d", &V, &E);
	
	edges = (EDGE*) malloc(E * sizeof(EDGE));
	
	for(i=0;i<E;i++){
		scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].weight);
		edges[i].u--;
		edges[i].v--;
	}
	
	printf("%d", bellmanFord(edges, E, V, 0));
	
	return 0;
}

int bellmanFord(EDGE* edges, int E, int V, int src){
	long long *dist = (long long*) malloc(V * sizeof(long long));
	int maxDist = 1000001;
	int i, j;
	
	for(i=0;i<V;i++){
		dist[i] = maxDist;
	}
	
	dist[src] = 0;
	
	for(i=0;i<V-1;i++){
		for(j=0;j<E;j++){
			if(dist[edges[j].v] > dist[edges[j].u] + edges[j].weight){
				dist[edges[j].v] = dist[edges[j].u] + edges[j].weight;
			}
		}
	}
	
	//to look whether there is negative cycle
	for(i=0;i<E;i++){
		if(dist[edges[i].v] > dist[edges[i].u] + edges[i].weight){
			free(dist);
			return 1;
		}
	}
	
	free(dist);
	return 0;
}
