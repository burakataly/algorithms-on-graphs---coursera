#include<stdio.h>
#include<stdlib.h>

/*
Correctness of Kosaraju's SCC algortihm:

Bir directed graphta SCC'ler birbirlerine ya bir y�nl� path'la ba�l�lard�r ya da hi� ba�l� de�illerdir. �ki y�nl� bir path'la ba�l� olsalard�
bunlar SCC de�il bir SCC'nin iki tane strongly connected subgraph'� olurlard�. �imdi bir graphtaki her bir SCC'yi kendi i�inde bir b�t�n
olarak d���n�rsek ortaya bir DAG ��kar (metagraph). DAG olmamas� i�in bir cycle olmas� gerekirdi. Yani iki SCC bir directed cycle'�n i�inde
olmal�yd�. Ama b�yle bir durum bunlar�n SCC olmad���n� g�sterir bir �eli�ki yarat�r. 

�imdi DAG'larla ilgili �zel duruma gelelim: Bildi�in gibi her DAG bir Topological Ordering (TO) ile ifade edilebilir. DAG, TO �eklinde ifade
edildi�inde bildi�in gibi en soldaki node her node'a ula��r ama en sa�daki node hi�bir node'a ula�amaz. E�er bu graph'�n transpozunu al�rsak
yani arclar�n y�n�n� ters �evirirsek bu durumda az �nce hi�bir node'a ula�amayan en sa�daki node her node'a ula��rken en soldaki hi�birine
ula�amaz. TO'nun simetri�i al�nm�� olur asl�nda. Metagraph i�in d���n�rsek transpoz i�lemi SCC'leri etkilemez. ��nk� SCC'lerin herbiri bir directed
cycle asl�nda. Transpoz sonras� sadece bu cycle'�n y�n� de�i�ir.

�imdi DFS yard�m�yla graphtaki SCC'leri bulma k�sm�na gelirsek elimizde ��yle bir s�k�nt� var: Bir undirected graph�n CC'lerini bulurken
izledi�imiz yolu burada uygularsak, TO'da en soldaki SCC'nin bir node'unu ba�lang�� node'u alarak verirsen bu node kendi ait oldu�u SCC'nin
elemanlar�na ek olarak graphtaki her node'a ula��r ve etiketler. Ve sonu�ta hatal� olarak elimizdeki graph tek bir SCC'den olu�uyormu� sonucu
��kar. Yani s�radan DFS'yi kullanamay�z. Peki ne yap�caz?

E�er ki en sa�dan s�ral� �ekilde TO'daki SCC'lerin node'lar�ndan birini DFS'ye versek sonu� do�ru ��kard�. ��nk� sa�daki etiketlenirken
solundaki SCC'ye ula�amazd� ve i�i bitince soldakinin node'u i�in DFS yapt���m�zda sa��ndaki SCC'ler etiketlendi�inden oraya gidemezdi ve
solundaki SCC'ye de gidemedi�inden do�ru �ekilde etiketleme yapard�. Bunu ba�armak i�in ��yle bir yol izleyebiliriz: Graph �zerinde klasik bir
DFS search yap�p bu DFS'lerin sonunda g�nderilen ba�lang�� node'unu bir stack'e eklersek, t�m graph gezildikten sonra stack'in en �st�nde 
TO'nun en solundaki SCC'den bir node olur. ��nk� en soldaki SCC'den bir node verildi�i zaman bu node graph'taki her node'u dola�t�ktan sonra
DFS'nin sonunda stack'e eklenir. Bu arada klasik DFS'ye nerden ba�land���n�n �nemi yok. Yani ilk TO'nun en sa��ndaki SCC'nin node'unu da versen
t�m graph tarand���nda stack'�n en �st�nde TO'nun en solundaki SCC'nin bir eleman� bulunur.

�imdi en tatl� noktaya geldik: T�m node'lar� bu �ekilde stack'e kaydettikten sonra elimizdeki graph�n transpozunu ald���mda TO'nun simetrisini
alm�� olurum ve stack'in en �st�ndeki node'un bulundu�u SCC art�k TO'nun en sa��ndaki SCC olur. Ve ben bu transpoz graph i�in bir DFS
ba�latt�rsam ve DFS'ye verece�im ba�lang�� node'lar�n� stack'ten verirsem �stteki paragraf�n ba��ndaki olay� ba�arm�� olurum.

Yani �� a�ama var: 1- transpoz alma, 2- stack'e node eklemek i�in bir DFS, 3- SCC'leri etiketlemek i�in bir DFS
A�amalar�n karma��kl�klar� s�ras�yla O(E), O(V + E), O(V + E) yani totalde O(V + E)...

NOT: 2. a�amadaki DFS'ye transpoz graph�, 3. a�amadakine normal graph'� versen de sonu� de�i�mez ��nk� transpoz i�lemi SCC'leri etkilemez.
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
	
	//stacki doldurmak i�in olan DFS. O(V + E) 
	for(i=0;i<graph->numOfNodes;i++){
		if(label[i] == 0){
			depthFirstSearch1(graph, label, i, &head);
		}
	}
	
	for(i=0;i<graph->numOfNodes;i++) label[i] = 0;
	
	//etiketlemek i�in olan DFS. Ba�lang�� nodelar�n� burada stackten veriyoruz ama recursive �a�r�larda normalde CC bulurken yapt���m�z gibi
	//t�m kom�ular�n� geziyoruz. O(V + E) 
	while(head != NULL){
		i = pop(&head);
		if(label[i] == 0){
			depthFirstSearch2(trans, label, i, labeler++);
		}
	}
	
	free(label);
	
	return labeler-1; // graphta ka� tane SCC oldu�unu d�nd�r�r.
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
