#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <queue>
#include <iostream>

using namespace std;

int X,Y;

void make_matrix(int **&mtx, int lin, int col)
{
	mtx = (int **)calloc(lin, sizeof(int *));
	for (int i = 0; i < col; i++)
	{
		mtx[i] = (int *)calloc(col, sizeof(int));
	}
}

void process_input(int** &graph, int &V, int &E)
{
    int n,k;
    char line[10];
    int count = 0, edges = 0;

    fgets(line,10,stdin);
    int tokens = sscanf(line, "%d %d", &n, &k);
    

    V = n+2;

    make_matrix(graph, V-2, V);

    int x,y,w = 0;
    while (fgets(line,10,stdin) != NULL)
    {
        tokens = sscanf(line, "%d %d %d", &x,&y,&w);
        if (tokens ==2)
        {
            edges+=2;
            graph[V-2][count] = x;
            graph[V-1][count] = y;
        }
        else if (tokens == 3)
        {
            edges+=1;
            graph[x-1][y-1] = graph[y-1][x-1] = w;
        }
        count++;
    }
    E = edges;
}

void print_matrix(int **&mtx, int lin, int col)
{
	for (int i = 0; i < lin; i++)
	{
		for (int j = 0; j < col; j++)
		{
			printf("%d ", mtx[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{
    int V, E;
    int **graph;
    process_input(graph,V,E);
    print_matrix(graph,V,V-2);
    free(graph);
    exit(0);
}