#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Colores disponibles
const vector<string> colores = {"Rojo", "Verde", "Azul", "Amarillo", "Naranja", "Violeta", "Cyan", "Magenta"};

// Función para imprimir el resultado del coloreado junto con los caminos directos
void printColorsAndPaths(const vector<vector<int>>& graph, const vector<int>& colors) {
    int V = graph.size();

    for (int u = 0; u < V; u++) {
        cout << "\t[Nodo " << u << "] (" << colores[colors[u]] << "): \t| ";

        bool first = true;
        for (int v = 0; v < V; v++) {
            if (graph[u][v]) {
                if (!first) cout << "\t| ";
                cout << "Nodo " << v << " (Color " << colores[colors[v]] << ")";
                first = false;
            }
        }
        cout << endl;
    }
}

// Algoritmo Greedy para el coloreado de grafos
void greedyColoring(const vector<vector<int>>& graph, int V) {
    vector<int> result(V, -1); // Colores asignados a los nodos
    result[0] = 0; // Asignar el primer color al primer nodo

    vector<bool> available(V, false); // Colores disponibles

    for (int u = 1; u < V; u++) {
        // Marcar colores de los nodos adyacentes como no disponibles
        for (int i = 0; i < V; i++) {
            if (graph[u][i] && result[i] != -1) {
                available[result[i]] = true;
            }
        }

        // Encontrar el color disponible
        int cr;
        for (cr = 0; cr < V; cr++) {
            if (!available[cr]) break;
        }
        result[u] = cr; /

        fill(available.begin(), available.end(), false);
    }

    printColorsAndPaths(graph, result);
}

// Algoritmo Welsh-Powell para el coloreado de grafos
void welshPowellColoring(vector<vector<int>>& graph, int V) {
    vector<int> degree(V, 0);
    vector<int> result(V, -1); // Inicializar sin color (-1)

    // Calcular el grado 
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (graph[i][j]) degree[i]++;
        }
    }

    vector<int> nodes(V);
    for (int i = 0; i < V; i++) nodes[i] = i;
    sort(nodes.begin(), nodes.end(), [&](int a, int b) { return degree[a] > degree[b]; });

    vector<bool> available(V, false); 

    for (int i = 0; i < V; i++) {
        int u = nodes[i];

        // Marcar los colores de nodos adyacentes como no disponibles
        for (int v = 0; v < V; v++) {
            if (graph[u][v] && result[v] != -1) available[result[v]] = true;
        }

        // Encontrar el primer color disponible
        int cr;
        for (cr = 0; cr < V; cr++) {
            if (!available[cr]) break;
        }
        result[u] = cr; 

        // Resetear la disponibilidad de colores para la próxima iteración
        fill(available.begin(), available.end(), false);
    }

    printColorsAndPaths(graph, result);
}

int main() {
    
    // Grafo de ejemplo (matriz de adyacencia de un grafo no dirigido)
    vector<vector<int>> graph = {
        {0, 1, 0, 1, 0},
        {1, 0, 1, 1, 0},
        {0, 1, 0, 1, 1},
        {1, 1, 1, 0, 1},
        {0, 0, 1, 1, 0}
    };

    int V = graph.size();

    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------\nAlgoritmo GREEDY:\n" << endl;
    greedyColoring(graph, V);

    cout << "\n===================================================================================================================================================================\nAlgoritmo WELSH-POWWELL:\n" << endl;
    welshPowellColoring(graph, V);

    return 0;
    
   
}
