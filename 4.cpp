#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
class vertex {
	friend class Graph;
private :
	int key = 99999;
	int parent = -1;
	bool isused = false;
public :
	vertex() {
		key = 99999;
		parent = -1;
		isused = false;
	}
	vertex(int k, int p) {
		key = k;
		parent = p;
		isused = false;
	}
};

class Queue{
	friend class Graph;
private:
	int n;
	int front;
	int rear;
	int * queue;

public:
	Queue(int vertices) {
		if (vertices < 1) cout << "vertex가 존재하지 않습니다." << endl;
		else {
			n = vertices;
			queue = new int[n];
			front = 0;
			rear = 0;
		}
	}
	bool isEmpty() {
		return front == rear;
	}
	void Push(int x) {
		if ((rear + 1) % n == front) cout << "Error";
		else {
			rear = (rear + 1) % n;
			queue[rear] = x;
		}
	}
	void Pop() {
		if (isEmpty()) cout << "Error";
		else {
			front = (front + 1) % n;
			queue[front] = 0;
		}
}
};
class List {
	friend class Graph;
private:
	int weight = 0;
	int data = 0;
	List* link = nullptr;
public:
	List(int w = 0, int d = 0, List * next = nullptr) {
		weight = w;
		data = d;
		link = next;
	}
};

class Graph {

private:
	int r = 0;
	int n = 0;
	int e = 0;
	List * HeadNodes;
	bool * visited;
	int ** length=nullptr;
public:
	Graph(int vertices, int edges, int root=0) {
		n = vertices;
		e = edges;
		r = root;
		HeadNodes = new List[vertices];
		length = new  int * [vertices];
		for (int i = 0; i < vertices; i++) {
			HeadNodes[i] = *new List();
		}
		for (int i = 0; i < n; i++) {
			length[i] = new int[n];
			for (int j = 0; j < n; j++) {
				length[i][j] = 99999;
			}
		}
	}
	void insert_wmat(int u, int v, int weight) {length[u][v] = weight;}
	void printG();
	void insertEdge(int head, int weight, int data);
	void DFS();
	void DFShorse(int vertex);
	void BFS();
	void BFShorse(int vertex);
	void MST();
	void ShortestPath();
};

void Graph::printG() {
	List* temp;
	cout << "Adjacency list) " << endl;
	for (int i = 0; i < n; i++) {
		cout << i;
		temp = HeadNodes[i].link;
		while (temp != nullptr) {
			cout << " -> " << temp->data;
			temp = temp->link;
		}
		cout << endl;
	}
	cout << endl;
}
void Graph::insertEdge(int weight, int head, int data) {
	List* next = new List(weight, data, nullptr);
	List* temp = &HeadNodes[head];
	while (temp->link != nullptr) {
		temp = temp->link;
	}
	(*temp).link = next;
	return;
}
Graph AdjList(bool isDirected) {
	ifstream file("input.txt");
	string line;
	string from;
	string to;
	string weight;
	
	getline(file, line);
	int root = stoi(line);
	getline(file, line);
	int n = stoi(line);
	getline(file, line);
	int e = stoi(line);
	Graph G(n, e, root);
	while (file.peek() != EOF) {
		getline(file, line);
		int temp = line.find(",");
		while (temp != string::npos) {
			line.replace(temp, 1, " ");
			temp = line.find(",");
		}
		stringstream ss;
		ss.str(line);
		ss >> weight;
		ss >> from;
		ss >> to;
		if (isDirected == false) {
			G.insertEdge(stoi(weight), stoi(from), stoi(to));
			G.insertEdge(stoi(weight), stoi(to), stoi(from));
			G.insert_wmat(stoi(from), stoi(to), stoi(weight));
			G.insert_wmat(stoi(to), stoi(from), stoi(weight));
		}
		else {
			G.insertEdge(stoi(weight), stoi(from), stoi(to));
			G.insert_wmat(stoi(from), stoi(to), stoi(weight));
		}
	}
	file.close();
	return G;
}
void Graph::DFS() {
	visited = new bool[n];
	cout << "DFS )" << endl;
	for (int i = 0; i < n; i++) {
		visited[i] = false;
	}
	int i = 1;
	for (int j = r; j < r+n; j++) {
		if (visited[j%n] == false) {
			cout << i << ") ";
			cout << j%n;
			DFShorse(j%n);
			cout << "   ";
			i++;
		}
	}
	cout << endl<<endl;
	delete[] visited;
};
void Graph::DFShorse(int vertex) {
	
	visited[vertex] = true;
	List * temp = &HeadNodes[vertex];
	while (temp->link != nullptr) {
		temp = temp->link;
		if (visited[temp->data]==false) {
			cout << " -> " << temp->data;
			DFShorse(temp->data);
		}
	}
}
void Graph::BFS() {
	visited = new bool[n];
	cout << "BFS )" << endl;
	for (int i = 0; i < n; i++) {
		visited[i] = false;
	}
	cout << r;
	BFShorse(r);
	cout << endl<<endl;
	delete[] visited;
};
void Graph::BFShorse(int vertex) {
	Queue q(n);
	visited[vertex] = true;
	q.Push(vertex);
	List * temp;
	while (!q.isEmpty()) {
			int v = q.queue[((q.front + 1) % q.n)];
			q.Pop();
			temp = &HeadNodes[v];
			while (temp->link != nullptr) {
				temp = temp->link;
				if (visited[temp->data] == false) {
					cout << " -> " << temp->data;
					q.Push(temp->data);
					visited[temp->data] = true;
				}
			}
	}
}

void Graph::ShortestPath() {
	cout << "Shortest Path )" << endl;
	vertex * vertices = new vertex[n];
	vertices[r].key = 0;
	int min;
	int temp_par=-1;
	for (int j = 0; j < n; j++) {
		min = -1;
		for (int i = 0; i < n; i++) {
			if (min == -1 && vertices[i].key < 99999) {
				if (vertices[i].isused == false) min = i;
			}
			else if (vertices[i].key < vertices[min].key && vertices[i].isused == false) {
				min = i;
			}
		}
			vertices[min].isused = true;

			if (min == -1)break;
			for (int w = 0; w < n; w++){
				if (vertices[w].isused == false && vertices[min].key + length[min][w] < vertices[w].key) {
					vertices[w].key = vertices[min].key + length[min][w];
					vertices[w].parent = min;
				}
			}
		
	}
	vertex temp;
	for (int i = 0; i < n; i++) {
		cout << "Vertice " << i << " : ";
		if (vertices[i].key == 99999) { cout << "Doesn't connected" << endl; }
		else {
			temp = vertices[i];
			cout << i;
			while (temp.parent != -1) {
				cout << " <- " << temp.parent;
				temp = vertices[temp.parent];
			}
			cout << " , Total Cost (" << r << " -> " << i << ") : " << vertices[i].key;
			cout << endl;
		}
	}
	cout << endl;
}

void Graph::MST() {
	cout << "MST)" << endl;
	int * tree = new int[n];
	for (int i = 0; i < n; i++) {
		tree[i] = -1;
	}
	tree[0] = r;
	int index = 1;
	vertex * vertices = new vertex[n];
	vertices[r].key = 0;
	vertices[r].isused = true;
	int min;
	while (index < n) {
		int now = tree[index - 1];
		List * temp = &HeadNodes[now];
		while (temp->link != nullptr) {
			temp = temp->link;
			if (vertices[temp->data].isused == false && temp->weight < vertices[temp->data].key) {
				vertices[temp->data].parent = now;
				vertices[temp->data].key = temp->weight;
			}
		}
			min = -1;
			for (int i = 0; i < n; i++) {
				if (min == -1 && vertices[i].key < 99999) {
					if (vertices[i].isused == false) {
						min = i;
					}
				}
				else if (vertices[i].key < vertices[min].key && vertices[i].isused == false) {
					min = i;
				}
			}
			if (min < 0) {
				cout << "no spanning tree" << endl;
				
			}
			else {
				tree[index] = min;
				vertices[min].isused = true; //Extract-Min
			}
			index++;
	}
	int total_cost = 0;
	for (int i = 1; i < n; i++) {
		{
			total_cost += vertices[tree[i]].key;
			cout << "("<< vertices[tree[i]].parent <<", "<< tree[i] <<") ";
		}
	}
	cout <<endl<< "Total cost : " << total_cost << endl;
	cout << endl<<endl;
}
int main() {
	cout << "Undirected :" << endl << endl;
	Graph G = AdjList(false);
	G.printG();
	G.DFS();
	G.BFS();
	G.MST();
	cout << "Directed :" << endl << endl;
	Graph G_directed = AdjList(true);
	G_directed.printG();
	G_directed.ShortestPath();
	cout << endl;
	return 0;
}