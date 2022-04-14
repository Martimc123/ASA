#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>

using namespace std;

// indexes for X and Y vertexes in the graph
int X;
int Y;

int max(int n1, int n2)
{
	if (n1 >= n2)
		return n1;
	return n2;
}

int min(int n1, int n2)
{
	if (n1 <= n2)
		return n1;
	return n2;
}

void print_matrix(int **&mtx, int lin, int col)
{
	for (int i = 0; i < lin; i++)
	{
		printf("%d: ", i); // lines numbers
		for (int j = 0; j < col; j++)
		{
			printf("%d ", mtx[i][j]);
		}
		printf("\n");
	}
}

void make_matrix(int **&mtx, int lin, int col)
{
	mtx = (int **)calloc(lin, sizeof(int *));
	for (int i = 0; i < col; i++)
	{
		mtx[i] = (int *)calloc(col, sizeof(int));
	}
}

void process_input(int **&graph, int &V, int &E)
{
	int n; // number of processes, n >= 0
	int k; // number of non-zero comunication cost between processes, k > 0	

	// First line, gives n and k
	int tokens = scanf( "%d %d", &n, &k);

	// Error check
	if (tokens < 2 || n < 2 || k <= 0)
	{
		fprintf(stderr, "Input error\n");
		exit(EXIT_FAILURE);
	}
	V = n + 2; // processes plus X and Y processors
	
	// set X and Y
	X = 0;
	Y = V-1;

	// init graph
	make_matrix(graph, V, V);

	// reads cost of processes in X and Y
	int cost_x, cost_y;
	for(int p = 1; p < Y; p++)
	{
		if (scanf("%d %d", &cost_x, &cost_y) != 2 )
		{
			printf("Input error: cost of processes in X and Y");
			exit(EXIT_FAILURE);
		}
		graph[X][p] = graph[p][X] = cost_x;
		graph[Y][p] = graph[p][Y] = cost_y;
	}

	// reads communication costs between processes
	int p1, p2, cost;
	for(int i = 1; i <= k; i++)
	{
		if (scanf("%d %d %d", &p1, &p2, &cost) != 3)
		{
			printf("Input error: communication costs between processes");
			exit(EXIT_FAILURE);
		}
		graph[p1][p2] = graph[p2][p1] = cost;
	}
}

bool BFS(int **&rGraph, int &V, int *&parent, int &max_c_f)
{
	int u;
	int visited[V];
	for (int i = 0; i < V; i++)
	{
		visited[i] = 0;
	}

	queue<int> Q;
	Q.push(X);
	visited[X] = 1;
	parent[X] = -1;

	while (!Q.empty())
	{
		u = Q.front(); 
		Q.pop();

		for (int v = 0; v < V; v++)
		{
			if (!visited[v] && rGraph[u][v] > 0)
			{
				max_c_f = max(max_c_f, rGraph[u][v]);
				if (v == Y)
				{
					parent[v] = u;
					return true;
				}
				Q.push(v);
				parent[v] = u;
				visited[v] = 1;
			}
		}
	}

	return false;
}

int Edmonds_Karp(int **&graph, int &V)
{
	int u, v;

	int** rGraph;
	make_matrix(rGraph, V, V);
	for (u = 0; u < V; u++)
	{
		for (v = 0; v < V; v++)
		{
			rGraph[u][v] = graph[u][v];
		}
	}

	int *parent = (int *)malloc(V * sizeof(int));

	int max_flow = 0;
	int c_f = 0;
	while (BFS(rGraph, V, parent, c_f))
	{
		for (v = Y; v != X; v = parent[v])
		{
			u = parent[v];
			c_f = min(c_f, rGraph[u][v]);
		}

		for (v = Y; v != X; v = parent[v])
		{
			u = parent[v];
			rGraph[u][v] -= c_f;
			rGraph[v][u] += c_f; 
		}

		max_flow += c_f;
	}
	
	//for(v = 1; v < Y; v++)
	//{
	//	max_flow += rGraph[v][X];
	//}

	free(parent);
	free(rGraph);
	return max_flow;
}

int main(int argc, char **argv)
{
	int V, E;
	int **graph;
	process_input(graph, V, E);
	//print_matrix(graph, V, V);

	printf("%d\n", Edmonds_Karp(graph, V));

	free(graph);
	exit(0);
}