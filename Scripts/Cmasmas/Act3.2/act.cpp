#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
#include <cstring>

using namespace std;

#define INF INT_MAX

// Función para realizar la búsqueda en amplitud (BFS) en el grafo residual
// Complejidad: O(V + E) donde V es el número de vértices y E el número de aristas
bool bfs(vector<vector<int>> &residualGraph, int source, int sink, vector<int> &parent)
{
    int n = residualGraph.size();
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; ++v)
        {
            if (!visited[v] && residualGraph[u][v] > 0)
            { // Si no hemos visitado el nodo y hay capacidad en la arista
                q.push(v);
                parent[v] = u;
                visited[v] = true;
                if (v == sink)
                {
                    return true; // Si llegamos al nodo final (sink)
                }
            }
        }
    }

    return false; // No hay camino de aumento
}

// Implementación del algoritmo de Edmonds-Karp
// Complejidad: O(V * E^2) donde V es el número de vértices y E el número de aristas
int edmondsKarp(vector<vector<int>> &graph, int source, int sink)
{
    int u, v;
    int n = graph.size();
    vector<vector<int>> residualGraph = graph; // Grafo residual (con las aristas inversas)
    vector<int> parent(n);
    int maxFlow = 0;

    while (bfs(residualGraph, source, sink, parent))
    {

        // Encuentra el bottleneck del camino encontrado
        int pathFlow = INF;
        for (v = sink; v != source; v = parent[v])
        {
            u = parent[v];
            pathFlow = min(pathFlow, residualGraph[u][v]);
        }

        // Actualiza el camino encontrado
        for (v = sink; v != source; v = parent[v])
        {
            u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        // Incrementa el flujo máximo por el valor del flujo encontrado
        maxFlow += pathFlow;
    }

    return maxFlow;
}

// Función principal
// Complejidad: O(V * E^2) donde V es el número de vértices y E el número de aristas
int main()
{
    ifstream inputFile("grafo.txt");

    if (!inputFile)
    {
        cerr << "Error al abrir el archivo" << endl;
        return 1;
    }

    int n;
    inputFile >> n;

    vector<vector<int>> graph(n, vector<int>(n, 0));

    int u, v, capacity;
    while (inputFile >> u >> v >> capacity)
    {
        // Añadimos la capacidad de la arista
        graph[u][v] = capacity;
    }

    inputFile.close();

    int source = 0;   // El nodo fuente es el primero (0)
    int sink = n - 1; // El nodo destino es el último (n-1)

    int maxFlow = edmondsKarp(graph, source, sink);

    cout << "===============================================\n\nEl flujo máximo posible es: " << maxFlow<<"\n\n===============================================" << endl;

    return 0;
}
/*
6
0 1 2
0 3 3
1 4 3
1 2 5
2 5 2
3 2 1
4 5 4

6
0 1 11
0 2 12
1 3 12
2 1 1
2 4 11
3 5 19
4 3 7
4 5 4

6
0 1 16
0 2 13
1 3 12
1 2 10
2 1 4
2 4 14
3 2 9
3 5 20
4 3 7
4 5 4
*/