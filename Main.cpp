//https://www.geeksforgeeks.org/greedy-algorithms-set-7-dijkstras-algorithm-for-adjacency-list-representation/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits.h>
using namespace std;

struct Node {
	int destination;
	int weight;
	Node* next;
};

struct AdjacencyList {
	Node* head;
};

struct Graph {
	int vertexCount;
	AdjacencyList* list;
};

struct MinHeapNode {
	int vertex;
	int distance;
};

struct MinHeap {
	int size;
	int capacity;
	int* position;
	MinHeapNode **list;
};

Node* newAdjacencyNode(int destination, int weight);
Graph* createGraph(int vertexCount);
void addEdge(Graph* graph, int source, int destination, int weight);
MinHeapNode* newMinHeapNode(int vertex, int distance);
MinHeap* createMinHeap(int capacity);
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b);
void minHeapify(MinHeap* minHeap, int index);
int isEmpty(MinHeap* minHeap);
MinHeapNode* extractMin(MinHeap* minHeap);
void decreaseKey(MinHeap* minHeap, int vertex, int dist);
bool isInMinHeap(MinHeap * minHeap, int vertex);
void printList(int distance[], int n);
void dijkstra(Graph* graph, int source);

int main() {
  // create the graph given in above fugure
    int vertexCount = 9;
    Graph* graph = createGraph(vertexCount);
    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 7, 8);
    addEdge(graph, 1, 2, 8);
    addEdge(graph, 1, 7, 11);
    addEdge(graph, 2, 3, 7);
    addEdge(graph, 2, 8, 2);
    addEdge(graph, 2, 5, 4);
    addEdge(graph, 3, 4, 9);
    addEdge(graph, 3, 5, 14);
    addEdge(graph, 4, 5, 10);
    addEdge(graph, 5, 6, 2);
    addEdge(graph, 6, 7, 1);
    addEdge(graph, 6, 8, 6);
    addEdge(graph, 7, 8, 7);
	cout << "before djikstras" << endl;
    dijkstra(graph, 0);
 
    return 0;	
}

Node* newAdjacencyNode(int destination, int weight) {
	Node* node = new Node;
	node->destination = destination;
	node->weight = weight;
	node->next = NULL;
	return node;
}

Graph* createGraph(int vertexCount) {
	Graph* graph = new Graph;
	graph->vertexCount = vertexCount;
	graph->list = new AdjacencyList[vertexCount];

	for(int i = 0; i < vertexCount; i++) {
		graph->list[i].head = NULL;
	}
	return graph;
}
	
//Adds an edge to an undirected graph
void addEdge(Graph* graph, int source, int destination, int weight) {
	Node* node = newAdjacencyNode(destination, weight);
	node->next = graph->list[destination].head;
	graph->list[source].head = node;
	node = newAdjacencyNode(source,weight);
	node->next = graph->list[destination].head;
	graph->list[destination].head = node;
}

MinHeapNode* newMinHeapNode(int vertex, int distance) {
	MinHeapNode* minHeapNode = new MinHeapNode;
	minHeapNode->vertex = vertex;
	minHeapNode->distance = distance;
	return minHeapNode;
}
	
MinHeap* createMinHeap(int capacity) {
	MinHeap* minHeap = new MinHeap;
	minHeap->position = new int[capacity];
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->list = new MinHeapNode*[capacity];
	return minHeap;
}
	
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
	MinHeapNode* temp = *a;
	*a = *b;
	*b = temp;
}

void minHeapify(MinHeap* minHeap, int index) {
	int smallest, left,right;
	smallest = index;
	left = 2 * index + 1;
	right = 2 * index + 2;
	
	if(left < minHeap->size && minHeap->list[left]->distance < minHeap->list[smallest]->distance) {
		smallest = left;
	}
	
	if(right < minHeap->size && minHeap->list[left]->distance < minHeap->list[smallest]->distance) {
		smallest = right;
	}
	
	if(smallest != index) {
		//The nodes to be swapped in min heap
		MinHeapNode* smallestNode = minHeap->list[smallest];
		MinHeapNode * indexNode = minHeap->list[index];
		
		//swap positions
		minHeap->position[smallestNode->vertex] = index;
		minHeap->position[indexNode->vertex] = smallest;
		
		//swap nodes
		swapMinHeapNode(&minHeap->list[smallest], &minHeap->list[index]);
		minHeapify(minHeap, smallest);
	}
}

int isEmpty(MinHeap* minHeap) {
	return minHeap->size == 0;
}

MinHeapNode* extractMin(MinHeap* minHeap) {
	if(isEmpty(minHeap))
		return NULL;
	//Store the root node
	MinHeapNode* root = minHeap->list[0];
	
	//Replace root node with last node
	MinHeapNode* lastNode = minHeap->list[minHeap->size - 1];
	minHeap->list[0] = lastNode;
	
	//Update position of last node
	minHeap->position[root->vertex] = minHeap->size-1;
	minHeap->position[lastNode->vertex] = 0;
	
	//Reduce heap size and heapify root
	--minHeap->size;
	minHeapify(minHeap,0);
	
	return root;
}

//Decreases the distance value of a given vertex vertex
void decreaseKey(MinHeap* minHeap, int vertex, int distance) {
	//Get the index of vertex in heap list
	int i = minHeap->position[vertex];
	
	//Get the node and update its distance value
	minHeap->list[i]->distance = distance;
	
	while (i && minHeap->list[i]->distance < minHeap->list[(i-1)/2]->distance) {
		
		//Swap the node with its parent
		minHeap->position[minHeap->list[i]->vertex] = (i-1)/2;
		minHeap->position[minHeap->list[(i-1)/2]->vertex] = i;
		swapMinHeapNode(&minHeap->list[i], &minHeap->list[(i-1)/2]);
		
		//Move to parent Index
		i = (i-1)/2;
	}
}

bool isInMinHeap(MinHeap * minHeap, int vertex) {
	if (minHeap->position[vertex] < minHeap->size) {
		return true;
	}
	else {
		return false;
	}
}

void printList(int distance[], int n) {
	cout << "Vertex Distance from Source\n" << endl;
	for (int i = 0; i <n; ++i) {
		cout << i << " " << distance[i] << endl;
	}
}

void dijkstra(Graph* graph, int source) {
	int vertexCount = graph->vertexCount;
	int distance[vertexCount];
	
	MinHeap* minHeap = createMinHeap(vertexCount);
	
	for(int vertex = 0; vertex < vertexCount; ++vertex ) {
		distance[vertex] = INT_MAX;
		minHeap->list[vertex] = newMinHeapNode(vertex, distance[vertex]);
		minHeap->position[vertex] = vertex;
	}
	//Make distance value of source vertex 0 so that it is extracted first
	minHeap->list[source] = newMinHeapNode(source, distance[source]);
	minHeap->position[source] = source;
    distance[source] = 0;
    decreaseKey(minHeap, source, distance[source]);
	
	// Initially size of min heap is equal to the vertex count
    minHeap->size = vertexCount;
	while (!isEmpty(minHeap)) {
        // Extract the vertex with minimum distance value
        MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->vertex; // Store the extracted vertex number
 
        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values
        Node* pCrawl = graph->list[u].head;
        while (pCrawl != NULL)
        {
            int vertex = pCrawl->destination;
            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (isInMinHeap(minHeap, vertex) && distance[u] != INT_MAX && 
                pCrawl->weight + distance[u] < distance[vertex]) {
                distance[vertex] = distance[u] + pCrawl->weight;
				
                // update distance value in min heap also
                decreaseKey(minHeap, vertex, distance[vertex]);
            }
            pCrawl = pCrawl->next;
        }
    }
    // print the calculated shortest distances
    printList(distance, vertexCount);
}
	
		