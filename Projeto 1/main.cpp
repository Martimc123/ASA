#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stack>

#define ISOLATED 0
#define SOURCE 1
#define SYNC 2
#define OTHER 3
#define INFINITY -1

using namespace std;

typedef struct vertex
{
	int type;
	int d;
	vector<int> adj;
} vertex;

int process_input(vector<vertex> &graph, int *V, int *E)
{
	int m, n, k, tokens;

	/*Gets the first line in order to know the number of vertices and arcs of the graph*/
	tokens = fscanf(stdin, "%d %d", &n, &m);

	/*Verifies if the length of the 1st line is lower than 3 and if n and m are correct*/
	if (tokens != 2 || n < 2 || m < 0)
	{
		fprintf(stderr, "Input error\n");
		exit(EXIT_FAILURE);
	}

	*V = n;
	*E = m;

	k = n;

	// init graph
	graph = vector<vertex>(n+1, vertex()); 
	for (int i = 1; i < n+1; i++)
	{
		// assume that every node is ISOLATED, therefore it starts with 1
		graph[i].type = ISOLATED;
		graph[i].d = 1; 
	}

	
	for (int i = 1; i <= m; i++)
	{
		int u, v;
		if (fscanf(stdin, "%d %d\n", &u, &v) < 2)
		{
			exit(EXIT_FAILURE);
		}

		graph[u].adj.push_back(v);

		if (graph[u].type == ISOLATED)
		{
			graph[u].type = SOURCE;
		}
		else if (graph[u].type == SYNC)
		{
			graph[u].type = OTHER;
		}

		if (graph[v].type < SYNC)
		{
			k--;
			if (graph[v].type == ISOLATED)
				graph[v].type = SYNC;
			else if (graph[v].type == SOURCE)
				graph[v].type = OTHER;
		}

	}
	return k;
}

vector<int> inverted_topo_sort(int V, vector<vertex> &g)
{
	
	// visited vertices, inicially none
	vector<int> reversed_topo_order;
	vector<int> visited(V+1, 0);
	vector<int> finished(V+1, 0);

	stack<int> stack;

	int v_id;
	
	// go through all non visited and non isolated vertexes
	for (int i = 1; i <= V; i++)
	{
		if (g[i].type == ISOLATED || visited[i])
		{
			continue;
		}
		stack.push(i);
		while (!stack.empty())
		{
			v_id = stack.top();

			if (finished[v_id])
			{
				stack.pop();
				continue;
			}

			// mark as visited
			if (!visited[v_id])
			{
				visited[v_id] = 1;
			}
			
			// deal with adj
			finished[v_id] = 1; // when adj are all visited or doesnt have any
			for (int j = 0; j < int(g[v_id].adj.size()); j++)
			{
				if (!visited[g[v_id].adj[j]])
				{
					stack.push(g[v_id].adj[j]);
					if (finished[v_id])
						finished[v_id] = 0;
				}
			}
			if (finished[v_id])
			{
				reversed_topo_order.push_back(v_id);
				stack.pop();
			}

		}

	}

	return reversed_topo_order;
}

int compute_l(vector<vertex> &graph, vector<int> &topo_order)
{
	int l = 1;
	int aux;

	for (int i = 0; i < int(topo_order.size()); i++) // iterate , O(V)
	{
		for(int j = 0; j < int(graph[topo_order[i]].adj.size()); j++) // iterate adj, O(E)
		{
			if (graph[topo_order[i]].d < (aux = graph[graph[topo_order[i]].adj[j]].d + 1))
			{ 
				graph[topo_order[i]].d = aux;
			}
			if (graph[topo_order[i]].d > l)
			{
				l = graph[topo_order[i]].d;
			}
		}
	}	

	return l;
}

int main(int argc, char **argv)
{
	int V, E;
	int k;
	int l;
	vector<vertex> graph;
	k = process_input(graph, &V, &E);
	vector<int> topo_order = inverted_topo_sort(V, graph);
	l = compute_l(graph, topo_order);
	printf("%d %d\n", k, l);
	exit(0);
}