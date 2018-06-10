//Program Created by Jermaine Lara
//6/10/18
//Program creates a graph by entering a vertexes and creating edges between those vertices
//Implements Djikstra's algorithm to find the shortest path between two vertices 
//Based off the c++ program of Djisksta algorithm from: https://www.geeksforgeeks.org/greedy-algorithms-set-7-dijkstras-algorithm-for-adjacency-list-representation/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits.h>
#include <string.h>
using namespace std;

struct AdjacencyNode {
	int destination;
	int weight;
	AdjacencyNode* next;
};

struct AdjacencyList {
	AdjacencyNode* head;
};

struct Graph {
	string labels[20];
	AdjacencyList list[20];
};

struct MinHeapNode {
	int vertex;
	int distance;
};

struct MinHeap {
	int size;
	int capacity;
	int* position;
	MinHeapNode **nodes;
};

AdjacencyNode* createAdjacencyNode(int destination, int weight);
Graph* createGraph();
void addEdge(Graph* graph, int source, int destination, int weight);
void removeEdge(Graph* graph, int source, int destination);
void removeVertex(Graph* graph, int source);
MinHeapNode* createMinHeapNode(int vertex, int distance);
MinHeap* createMinHeap(int capacity);
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b);
void minHeapify(MinHeap* minHeap, int index);
int isEmpty(MinHeap* minHeap);
MinHeapNode* extractMin(MinHeap* minHeap);
void decreaseKey(MinHeap* minHeap, int vertex, int dist);
bool isInMinHeap(MinHeap * minHeap, int vertex);
void printAdjacencyTable(Graph* graph);
int* dijkstra(Graph* graph, int source);
void removeVertex();
void removeEdge();
int getVertexIndex(string &label, Graph* graph);
int getShortestDistance(Graph* graph, int vertexOneIndex, int vertexTwoIndex);

int main() {
	int choice;
	Graph* graph = createGraph();
	do {
		cout << "Enter option: " << endl;
		cout << "1 to add a vertex" << endl;
		cout << "2 to add an edge" << endl;
		cout << "3 to remove a vertex from the graph" << endl;
		cout << "4 to remove and edge from the graph" << endl;
		cout << "5 to search for the shortest path between two vertices"  << endl;
		cout << "6 to print the adjacency table of the graph" << endl;
		cout << "7 to exit" << endl;
		cin >> choice;
	
		if(choice == 1) {
			cout << "Enter Vertex Label" << endl;
			string label;
			cin >> label;
			//Use the first available (empty slot)
			for(int i = 0; i < 20; i++) {
				if(graph->labels[i].empty()) {
					graph->labels[i] = label;
					break;
				}
			}
		}
		if(choice == 2) {
			cout << "Enter the first vertex you want the edge to be between" << endl;
			string vertexOne;
			cin >> vertexOne;
			int vertexOneIndex = getVertexIndex(vertexOne, graph);
			if (vertexOneIndex == -1) {
				cout << "Label not found" << endl;
				continue;
			}
			cout << "Enter the second vertex you want the edge to be between" << endl;
			string vertexTwo;
			cin >> vertexTwo;
			int vertexTwoIndex = getVertexIndex(vertexTwo, graph);
			if (vertexTwoIndex == -1) {
				cout << "Label not found" << endl;
				continue;
			}
		
			cout << "Enter the weight you want the edge to have" << endl;
			int edgeWeight;
			cin >> edgeWeight;
			addEdge(graph, vertexOneIndex, vertexTwoIndex, edgeWeight);
		}
	
		if(choice == 3) {
			cout << "Enter the label of the vertex you want to remove" << endl;
			string removedVertexLabel;
			cin >> removedVertexLabel;
			int removedVertexIndex = getVertexIndex(removedVertexLabel, graph);
			if (removedVertexIndex == -1) {
				cout << "Label not found" << endl;
				continue;
			}
			removeVertex(graph, removedVertexIndex);
		}
	
		if(choice == 4) {
			cout << "Enter the first vertex label you want to remove" << endl;
			string firstVertexRemoved;
			cin >> firstVertexRemoved;
			int firstVertexIndex = getVertexIndex(firstVertexRemoved, graph);
			if (firstVertexIndex == -1) {
				cout << "Label not found" << endl;
				continue;
			}
			cout << "Enter the second vertex label you want to remove" << endl;
			string secondVertexRemoved;
			cin >> secondVertexRemoved;
			int secondVertexIndex = getVertexIndex(secondVertexRemoved, graph);
			if (secondVertexIndex == -1) {
				cout << "Label not found" << endl;
				continue;
			}
			removeEdge(graph, firstVertexIndex, secondVertexIndex);
		}
	
		if(choice == 5) {
			cout << "Enter the vertex label to start at" << endl;
			string startVertex;
			cin >> startVertex;
			int startVertexIndex = getVertexIndex(startVertex, graph);
			if (startVertexIndex == -1) {
				cout << "Label not found" << endl;
				continue;
			}
			cout << "Enter the vertex label to end at" << endl;
			string endVertex;
			cin >> endVertex;
			int endVertexIndex = getVertexIndex(endVertex, graph);
			if (endVertexIndex == -1) {
				cout << "Label not found" << endl;
				continue;
			}
			int distance = getShortestDistance(graph, startVertexIndex, endVertexIndex);
			cout << "The shortest distance: " << distance << endl;
		}
		
		//Prints graph in the form of an adjacency table
		if(choice == 6) {
			printAdjacencyTable(graph);
		}
	}
	while(choice != 7);
}

//Create a node that represents the terminating point of an edge
AdjacencyNode* createAdjacencyNode(int destination, int weight) {
	AdjacencyNode* node = new AdjacencyNode;
	node->destination = destination;
	node->weight = weight;
	node->next = NULL;
	return node;
}

//Create the empty graph
Graph* createGraph() {
	Graph* graph = new Graph;
	for(int i = 0; i < 20; i++) {
		graph->list[i].head = NULL;
	}
	return graph;
}
	
//Adds an edge to an undirected graph
void addEdge(Graph* graph, int source, int destination, int weight) {
	AdjacencyNode* node = createAdjacencyNode(destination, weight);
	node->next = graph->list[source].head;
	graph->list[source].head = node;
	
	node = createAdjacencyNode(source, weight);
	node->next = graph->list[destination].head;
	graph->list[destination].head = node;
}

//Removes the edge between two vertices
void removeEdge(Graph* graph, int source, int destination) {
	AdjacencyNode* prevNode = NULL;
	for(AdjacencyNode* node = graph->list[source].head; node != NULL; node = node->next) {
		if(node->destination == destination) {
			if(prevNode != NULL) {
				prevNode->next = node->next;
			}
			else {
				graph->list[source].head = NULL;
			}
			delete node;
			break;
		}
		prevNode = node;
	}
	prevNode = NULL;
	for(AdjacencyNode* node = graph->list[destination].head; node != NULL; node = node->next) {
		if(node->destination == source) {
			if(prevNode != NULL) {
				prevNode->next = node->next;
			}
			else {
				graph->list[destination].head = NULL;
			}
			delete node;
			break;
		}
		prevNode = node;
	}
}

//Removes a vertex and all edges connected to that vertex
void removeVertex(Graph* graph, int source) {
	for(int i = 0; i < 20; i++) {
		if(i != source && !graph->labels[i].empty()) {
			for(AdjacencyNode* node = graph->list[i].head; node != NULL; node = node->next) {
				if(node->destination == source) {
					removeEdge(graph, source, i);
				}
			}
		}
	}
	graph->labels[source].clear();
}
		
//Node that contains the distance of each vertex
MinHeapNode* createMinHeapNode(int vertex, int distance) {
	MinHeapNode* minHeapNode = new MinHeapNode;
	minHeapNode->vertex = vertex;
	minHeapNode->distance = distance;
	return minHeapNode;
}
	
//Create a heap that contains each MinHeapNode	
MinHeap* createMinHeap(int capacity) {
	MinHeap* minHeap = new MinHeap;
	minHeap->position = new int[capacity];
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->nodes = new MinHeapNode*[capacity];
	return minHeap;
}
	
//Swap two MinHeapNodes
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
	MinHeapNode* temp = *a;
	*a = *b;
	*b = temp;
}

// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(MinHeap* minHeap, int index) {
	int smallest, left,right;
	smallest = index;
	left = 2 * index + 1;
	right = 2 * index + 2;
	
	if(left < minHeap->size && minHeap->nodes[left]->distance < minHeap->nodes[smallest]->distance) {
		smallest = left;
	}
	
	if(right < minHeap->size && minHeap->nodes[left]->distance < minHeap->nodes[smallest]->distance) {
		smallest = right;
	}
	
	if(smallest != index) {
		//The nodes to be swapped in min heap
		MinHeapNode* smallestNode = minHeap->nodes[smallest];
		MinHeapNode * indexNode = minHeap->nodes[index];
		
		//swap positions
		minHeap->position[smallestNode->vertex] = index;
		minHeap->position[indexNode->vertex] = smallest;
		
		//swap nodes
		swapMinHeapNode(&minHeap->nodes[smallest], &minHeap->nodes[index]);
		minHeapify(minHeap, smallest);
	}
}

//Function to check if the minHeap is empty
int isEmpty(MinHeap* minHeap) {
	return minHeap->size == 0;
}

//Finds and removes the minNode from heap
MinHeapNode* extractMin(MinHeap* minHeap) {
	if(isEmpty(minHeap))
		return NULL;
	//Store the root node
	MinHeapNode* root = minHeap->nodes[0];
	
	//Replace root node with last node
	MinHeapNode* lastNode = minHeap->nodes[minHeap->size - 1];
	minHeap->nodes[0] = lastNode;
	
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
	minHeap->nodes[i]->distance = distance;
	
	while (i && minHeap->nodes[i]->distance < minHeap->nodes[(i-1)/2]->distance) {
		
		//Swap the node with its parent
		minHeap->position[minHeap->nodes[i]->vertex] = (i-1)/2;
		minHeap->position[minHeap->nodes[(i-1)/2]->vertex] = i;
		swapMinHeapNode(&minHeap->nodes[i], &minHeap->nodes[(i-1)/2]);
		
		//Move to parent Index
		i = (i-1)/2;
	}
}

//Checks if a given vertex is in the minHeap or not
bool isInMinHeap(MinHeap * minHeap, int vertex) {
	return minHeap->position[vertex] < minHeap->size;
}

//Displays each nodes adjacency table
void printAdjacencyTable(Graph* graph) {
	for(int i = 0; i < 20; ++i) {
		if(graph->labels[i].empty()) {
			continue;
		}
		int* distances = dijkstra(graph, i);
		for (int j = 0; j < 20; ++j) {
			if(distances[j] != INT_MAX) {
				cout << graph->labels[i] << " -> " << graph->labels[j] << "  " << distances[j] << endl;
			}
		}
	}
}

// The main function that calulates distances of shortest paths from source to all
// vertices
int* dijkstra(Graph* graph, int source) {
	int vertexCount = 20;
	int* distances = new int[vertexCount];
	
	MinHeap* minHeap = createMinHeap(vertexCount);
	
	for(int vertex = 0; vertex < vertexCount; ++vertex ) {
		distances[vertex] = INT_MAX;
		minHeap->nodes[vertex] = createMinHeapNode(vertex, distances[vertex]);
		minHeap->position[vertex] = vertex;
	}
	//Make distance value of source vertex 0 so that it is extracted first
	minHeap->nodes[source] = createMinHeapNode(source, distances[source]);
	minHeap->position[source] = source;
    distances[source] = 0;
    decreaseKey(minHeap, source, distances[source]);
	
	// Initially size of min heap is equal to the vertex count
    minHeap->size = vertexCount;
	while (!isEmpty(minHeap)) {
        // Extract the vertex with minimum distance value
        MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->vertex; // Store the extracted vertex number
 
        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values
        AdjacencyNode* crawl = graph->list[u].head;
        while (crawl != NULL)
        {
            int vertex = crawl->destination;
            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (isInMinHeap(minHeap, vertex) && distances[u] != INT_MAX && 
                crawl->weight + distances[u] < distances[vertex]) {
                distances[vertex] = distances[u] + crawl->weight;
                // update distance value in min heap also
                decreaseKey(minHeap, vertex, distances[vertex]);
            }
            crawl = crawl->next;
        }
    }
    return distances;
}

//Finds the shortest distance between two vertices
int getShortestDistance(Graph* graph, int startVertexIndex, int endVertexIndex) {
	int* distances = dijkstra(graph, startVertexIndex);
	return distances[endVertexIndex];
}
	
//Look up the index of a vertex given its label
int getVertexIndex(string &label, Graph* graph) {
	for(int i = 0; i < 20; i++) {
		if(graph->labels[i] == label) {
			return i;
		}
	}
	return -1;
}


			
	
		