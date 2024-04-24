#include<stdio.h>
#include<stdlib.h>

/*
Correctness of Kosaraju's SCC algortihm:

Bir directed graphta SCC'ler birbirlerine ya bir yönlü path'la baðlýlardýr ya da hiç baðlý deðillerdir. Ýki yönlü bir path'la baðlý olsalardý
bunlar SCC deðil bir SCC'nin iki tane strongly connected subgraph'ý olurlardý. Þimdi bir graphtaki her bir SCC'yi kendi içinde bir bütün
olarak düþünürsek ortaya bir DAG çýkar (metagraph). DAG olmamasý için bir cycle olmasý gerekirdi. Yani iki SCC bir directed cycle'ýn içinde
olmalýydý. Ama böyle bir durum bunlarýn SCC olmadýðýný gösterir bir çeliþki yaratýr. 

Þimdi DAG'larla ilgili özel duruma gelelim: Bildiðin gibi her DAG bir Topological Ordering (TO) ile ifade edilebilir. DAG, TO þeklinde ifade
edildiðinde bildiðin gibi en soldaki node her node'a ulaþýr ama en saðdaki node hiçbir node'a ulaþamaz. Eðer bu graph'ýn transpozunu alýrsak
yani arclarýn yönünü ters çevirirsek bu durumda az önce hiçbir node'a ulaþamayan en saðdaki node her node'a ulaþýrken en soldaki hiçbirine
ulaþamaz. TO'nun simetriði alýnmýþ olur aslýnda. Metagraph için düþünürsek transpoz iþlemi SCC'leri etkilemez. Çünkü SCC'lerin herbiri bir directed
cycle aslýnda. Transpoz sonrasý sadece bu cycle'ýn yönü deðiþir.

Þimdi DFS yardýmýyla graphtaki SCC'leri bulma kýsmýna gelirsek elimizde þöyle bir sýkýntý var: Bir undirected graphýn CC'lerini bulurken
izlediðimiz yolu burada uygularsak, TO'da en soldaki SCC'nin bir node'unu baþlangýç node'u alarak verirsen bu node kendi ait olduðu SCC'nin
elemanlarýna ek olarak graphtaki her node'a ulaþýr ve etiketler. Ve sonuçta hatalý olarak elimizdeki graph tek bir SCC'den oluþuyormuþ sonucu
çýkar. Yani sýradan DFS'yi kullanamayýz. Peki ne yapýcaz?

Eðer ki en saðdan sýralý þekilde TO'daki SCC'lerin node'larýndan birini DFS'ye versek sonuç doðru çýkardý. Çünkü saðdaki etiketlenirken
solundaki SCC'ye ulaþamazdý ve iþi bitince soldakinin node'u için DFS yaptýðýmýzda saðýndaki SCC'ler etiketlendiðinden oraya gidemezdi ve
solundaki SCC'ye de gidemediðinden doðru þekilde etiketleme yapardý. Bunu baþarmak için þöyle bir yol izleyebiliriz: Graph üzerinde klasik bir
DFS search yapýp bu DFS'lerin sonunda gönderilen baþlangýç node'unu bir stack'e eklersek, tüm graph gezildikten sonra stack'in en üstünde 
TO'nun en solundaki SCC'den bir node olur. Çünkü en soldaki SCC'den bir node verildiði zaman bu node graph'taki her node'u dolaþtýktan sonra
DFS'nin sonunda stack'e eklenir. Bu arada klasik DFS'ye nerden baþlandýðýnýn önemi yok. Yani ilk TO'nun en saðýndaki SCC'nin node'unu da versen
tüm graph tarandýðýnda stack'ýn en üstünde TO'nun en solundaki SCC'nin bir elemaný bulunur.

Þimdi en tatlý noktaya geldik: Tüm node'larý bu þekilde stack'e kaydettikten sonra elimizdeki graphýn transpozunu aldýðýmda TO'nun simetrisini
almýþ olurum ve stack'in en üstündeki node'un bulunduðu SCC artýk TO'nun en saðýndaki SCC olur. Ve ben bu transpoz graph için bir DFS
baþlattýrsam ve DFS'ye vereceðim baþlangýç node'larýný stack'ten verirsem üstteki paragrafýn baþýndaki olayý baþarmýþ olurum.

Yani üç aþama var: 1- transpoz alma, 2- stack'e node eklemek için bir DFS, 3- SCC'leri etiketlemek için bir DFS
Aþamalarýn karmaþýklýklarý sýrasýyla O(E), O(V + E), O(V + E) yani totalde O(V + E)...

NOT: 2. aþamadaki DFS'ye transpoz graphý, 3. aþamadakine normal graph'ý versen de sonuç deðiþmez çünkü transpoz iþlemi SCC'leri etkilemez.
*/

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

typedef struct n{
	int x;
	struct n* next;
}STACK;

void push(STACK**, int);
int pop(STACK**);

NODE* createNode(int);
GRAPH* createGraph(int);
void addEdges(GRAPH*, int, int);
int hasEdge(GRAPH*, int, int);

void depthFirstSearch1(GRAPH*, int*, int, STACK**);
void depthFirstSearch2(GRAPH*, int*, int, int);
GRAPH* transpose(GRAPH*);
int stronglyConnectedComponents(GRAPH*);

int main(){
	int n, m, i, u, v;
	scanf("%d %d", &n, &m);
	GRAPH* graph = createGraph(n);
	int* label = (int*) calloc(n, sizeof(int));
	for(i=0;i<m;i++){
		scanf("%d %d", &u, &v);
		addEdges(graph, u-1, v-1);
	}
	
	printf("%d", stronglyConnectedComponents(graph));
	
	return 0;
}

int stronglyConnectedComponents(GRAPH* graph){
	int i, labeler = 1;
	int* label = (int*) calloc(graph->numOfNodes, sizeof(int));
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
			addEdges(trans, temp->vertex, i);
			temp = temp->next;
		}
	}
	return trans;
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
