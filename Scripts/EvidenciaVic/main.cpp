#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <unordered_map>
#include <cmath>
#include <iomanip>
#include <cfloat>

#define INF INT_MAX

using namespace std;

struct Colonia {
    int x, y;
    string name;
    int central;
    Colonia() : x(0), y(0), name(""), central(0) {}
    Colonia(int x, int y, string name, int central) : x(x), y(y), name(name), central(central) {}
};

struct Graph {
    string check; //es el texto del archivo final

    int n, m, k, q; //n_colonias, m_arcos, k_CableadoNuevo, q_FuturasColonias
    
    vector<Colonia> colonias; //vector de colonias

    unordered_map<string, int> colonyIndex; // colonia:índice
    unordered_map<int, string> indexColony; // índice:colonia

    vector<pair<int, pair<int, int>>> arcos; // costo, colonia1 colonia2
    vector<pair<int, int>> nuevoCableado; //es un vector con los arcos con cableado nuevo de u a v
    
    int costoMST; //el costo de kruskal
    vector<tuple<string, string, int>> mstEdges; //arcos seleccionados [colonia1, colonia2, costo]

    vector<Colonia> coloniasFuturas; //vector de futuras colonias
    vector<pair<string, string>> conexionesFuturas; //nuevaColonia, a cual conectarse

    //es la matriz de adyacencias para floyd,
    vector<vector<int>> matAdj;
    vector<vector<bool>> strictly_connected; //matriz de adyacencia para saber si dos nodos estan conectados
    vector<vector<int>> distancias_directas; //matriz de distancias
    vector<vector<int>> next; //matriz para encontrar las colonias visitadas
    vector<int> centrales; //vector de colonias centrales (sus índices)

    Graph(int n, int m, int k, int q) {
        
        this->n = n;
        this->m = m;
        this->k = k;
        this->q = q;
        
        costoMST = 0;
        check = "";
        matAdj.resize(n, vector<int>(n));
        next.resize(n, vector<int>(n));
        strictly_connected.resize(n, vector<bool>(n));
        distancias_directas.resize(n, vector<int>(n));

        //inicializa matAdj con INF
        for(int i=0; i<n; i++) {
            
            matAdj[i][i] = 0;
            
            for(int j=i+1; j<n; j++) {
                matAdj[i][j] = matAdj[j][i] = INF;
            }
        }

        // Inicializa distancias_directas con INF
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
            if (i != j) {
                distancias_directas[i][j] = INF;
            } else {
                distancias_directas[i][j] = -1;
            }
            }
        }
        
    }

    double bestDistance = DBL_MAX; //mejor dist para nuevas colonias
    string closestColonyName;
    //saca la dist euclidiana
    static double dist(const Colonia &p1, const Colonia &p2) {
        return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    }
    
    bool isConnected(int nodo1, int nodo2);
    
    void tspBacktracking(
                            int nodoActual,
                            int nodoInicial,
                            vector<bool> &visitados,
                            const vector<vector<int>> &matrizAdyacencia,
                            vector<int> &rutaActual,
                            int costoActual,
                            vector<int> &rutaOptima,
                            int &costoOptimo,
                            int &noCentralVisitados,
                            int totalNoCentrales);

                         
    
    bool ruta_valida(vector<int>ruta);

    void load();
    void kruskalMST();
    void floyd();
    
    vector<int> getPath(int u, int v); //para encontrar el camino de floyd
    
    void updateClosestColony(const Colonia &existing, const Colonia &future);
    string findClosestColony(const Colonia &future);

    void PROBLEMA1();
    void PROBLEMA3();
    void PROBLEMA2();
    void PROBLEMA4();
    
};

//Disjoint set (Union-Find)
struct DisjointSets {
    int *parent, *rank;
    int cant;
    DisjointSets(int cant) {
        this->cant = cant;
        parent = new int[cant+1];
        rank = new int[cant+1];
        for(int i=0; i<=cant; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }
    int find(int u); //Encontrar el parent de 'u'
    void merge(int u, int v); //Union de dos conjuntos dependiendo del rank
};

int DisjointSets::find(int u) {
    if(u != parent[u]) {
        parent[u] = find(parent[u]);
    }
    return parent[u];
}

void DisjointSets::merge(int u, int v) {
    u = find(u);
    v = find(v);
    if(rank[u] > rank[v]) {
        parent[v] = u;
    }
    else {
        parent[u] = v;
    }
    if(rank[u] == rank[v]) {
        rank[v]++;
    }
}

void Graph::load() {
    //leer colonias
    for(int i=0; i<n; i++) {
        string nombre;
        int x, y, central;
        cin>>nombre>>x>>y>>central;
        Colonia aux(x, y, nombre, central);
        colonias.push_back(aux);
        colonyIndex[nombre] = i;
        indexColony[i] = nombre;
        if(central) {
            centrales.push_back(i);
        }
    }

    //leer conexiones
    for(int i=0; i<m; i++) {
        string c1, c2;
        int costo;
        cin>>c1>>c2>>costo;
        int u = colonyIndex[c1];
        int v = colonyIndex[c2];
        arcos.push_back({costo, {u, v}});
        matAdj[u][v] = matAdj[v][u] = costo;
        strictly_connected[u][v] = strictly_connected[v][u] = true;
        distancias_directas[u][v] = distancias_directas[v][u] = costo;
    }

    //lee las conexiones con cableado nuevo
    for(int i=0; i<k; i++) {
        string c1, c2;
        cin>>c1>>c2;
        int u = colonyIndex[c1];
        int v = colonyIndex[c2];
        nuevoCableado.emplace_back(u, v);
    }

    //leer colonias futuras
    for(int i=0; i<q; i++) {
        string nombre;
        int x, y;
        cin>>nombre>>x>>y;
        Colonia aux(x, y, nombre, 0);
        coloniasFuturas.push_back(aux);

        //por cada colonia nueva encuentra con que colonia es mas cercana
        string closestColony = findClosestColony(coloniasFuturas[i]);
        conexionesFuturas.emplace_back(nombre, closestColony);
    }
}
//O(e log e)
void Graph::kruskalMST() {
    
    sort(arcos.begin(), arcos.end()); // ordenar los arcos en forma ascendente del costo
    
    DisjointSets ds(n);

    //escoger los arcos de las colonias con cableado nuevo
    for(auto it:nuevoCableado) {
        ds.merge(it.first, it.second);
    }

    for(auto it:arcos) {
        //cada casilla con {w,{u,v}}
        int p1 = ds.find(it.second.first);
        int p2 = ds.find(it.second.second);
        if(p1 != p2) {
            costoMST += it.first;
            mstEdges.emplace_back(indexColony[it.second.first], indexColony[it.second.second], it.first); //[colonia1, colonia2, costo]
            ds.merge(it.second.first, it.second.second);
        }
    }


}

//si encuentra una colonia mas cerca, se la manda la que existe y la futura
void Graph::updateClosestColony(const Colonia &existing, const Colonia &future) {
    double d = dist(existing, future);
    if (d < bestDistance) { //si es mejor que la dist maxima se actualiza el nombre y la dist
        bestDistance = d;
        closestColonyName = existing.name;
    }
}

//encuentra la colonia mas cercana
//O(n)
string Graph::findClosestColony(const Colonia &future) {
    bestDistance = DBL_MAX;
    for (const auto &colony : colonias) { //por cada punto en colonia lo compara con el futuro
        updateClosestColony(colony, future);
    }
    return closestColonyName;
}

//Complejidad: O(n^3)
void Graph::floyd() {
    
    //inicializa next
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if (i != j) {
                next[i][j] = j; // Inicializa con el destino
            } else {
                next[i][j] = -1; // No hay camino hacia uno mismo
            }
        }
    }

    //algoritmo ya de floyd
    for(int k=0; k<n; k++) {
        
        for(int i=0; i<n; i++) {
            
            for(int j=0; j<n; j++) {
                
                if((matAdj[i][k] != INF) && (matAdj[k][j] != INF) && ((matAdj[i][k]+matAdj[k][j]) < matAdj[i][j])) {
                    
                    matAdj[i][j] = matAdj[i][k] + matAdj[k][j];
                    next[i][j] = next[i][k]; //actualiza la matriz de caminos
                    }
            }
        }
    }
}

//Complejidad: O(n) en caso de que tenga que recorrer todos los nodos
vector<int> Graph::getPath(int u, int v) {
    if (next[u][v] == -1) return {}; //por si no hay camino

    vector<int> path; //no incluye el nodo origen
    int current = next[u][v];

    while (current!= v) {
        path.push_back(current);
        current = next[current][v];
        if (current == -1) return {}; //para evitar ciclos infinitos
    }
    path.push_back(v);
    return path;
}

//O(e log e)
void Graph::PROBLEMA1() {
    kruskalMST();

    check += "-------------------\n";
    check += "1 – Cableado óptimo de nueva conexión.\n\n";
    for(const auto &[colony1, colony2, cost] : mstEdges) {
        check += colony1 + " - " + colony2 + " " + to_string(cost) + "\n";
    }
    check += "\nCosto Total: " + to_string(costoMST) + "\n";
}

//O(n^3)
void Graph::PROBLEMA3() {
    floyd();

    check += "\n-------------------\n3 – Caminos más cortos entre centrales.\n";

    int n = centrales.size(); //cantidad de colonias centrales
    //para recorrer todas las combinaciones
    for(int i=0; i<n; i++) {
        check += "\n";
        for (int j=i+1; j<n; j++) {
            check += indexColony[centrales[i]];
            vector<int> camino = getPath(centrales[i],centrales[j]);
            for(int k : camino) {
                check += " - " + indexColony[k];
            }
            check += " (" + to_string(matAdj[centrales[i]][centrales[j]]) + ")\n";
        }
    }

    //por si solo hay una colonia central
    if(n==1) {
        check += indexColony[centrales[0]] + " - " +  indexColony[centrales[0]] + " (0)\n";
    }
}

// FUNCIONES PARA EL PROBLEMA 2
// Funcion para saber si 2 nodos estan directamente conectados (falta cambiar matAdj por la nueva matris)
// O(1)
bool Graph::isConnected(int nodo1, int nodo2){
    return strictly_connected[nodo1][nodo2];
}

// Funcion para revisar si una ruta completa es valida
// O(n)
bool Graph::ruta_valida(vector<int>ruta){
    
    for(int nodo_a = 0; nodo_a < ruta.size()-1; nodo_a++){
        int nodo_b = ruta[nodo_a+1];
        if(!isConnected(nodo_a, nodo_b)){
            return false;
        }
    }
    return isConnected(ruta[ruta.size()-2], ruta[ruta.size()-1]);
}
void Graph::tspBacktracking(
    int nodoActual,
    int nodoInicial,
    vector<bool> &visitados,
    const vector<vector<int>> &matrizAdyacencia,
    vector<int> &rutaActual,
    int costoActual,
    vector<int> &rutaOptima,
    int &costoOptimo,
    int &noCentralVisitados,
    int totalNoCentrales
) {
    // Caso base: si hemos visitado todos los nodos no centrales
    if (noCentralVisitados == totalNoCentrales) {
        if (matrizAdyacencia[nodoActual][nodoInicial] != INF) {  // Verificar conexión de regreso
            int costoTotal = costoActual + matrizAdyacencia[nodoActual][nodoInicial];
            if (costoTotal < costoOptimo) {
                costoOptimo = costoTotal;
                rutaOptima = rutaActual;
                rutaOptima.push_back(nodoInicial); // Cerrar el ciclo
            }
        }
        return;
    }

    // Recorrer los vecinos
    for (int i = 0; i < visitados.size(); i++) {
        if (!visitados[i] && matrizAdyacencia[nodoActual][i] != INF) { // Nodo no visitado y hay conexión
            visitados[i] = true;
            noCentralVisitados++;
            rutaActual.push_back(i);

            tspBacktracking(
                i, nodoInicial, visitados, matrizAdyacencia, rutaActual, 
                costoActual + matrizAdyacencia[nodoActual][i], rutaOptima, 
                costoOptimo, noCentralVisitados, totalNoCentrales
            );

            // Backtracking: deshacer cambios
            rutaActual.pop_back();
            visitados[i] = false;
            noCentralVisitados--;
        }
    }
}

void Graph::PROBLEMA2() {
    // Filtrar las colonias no centrales
    vector<int> noCentralColonies, allNodes;
    for (int i = 0; i < n; i++) {
        allNodes.push_back(i); // Todos los nodos
        if (!colonias[i].central) {
            noCentralColonies.push_back(i); // Nodos no centrales
        }
    }

    // Si no hay nodos no centrales, terminar
    if (noCentralColonies.empty()) {
        check += "\n-------------------\n2 – La ruta óptima.\n";
        check += "No hay colonias no centrales para visitar.\n";
        return;
    }

    // Variables para almacenar la mejor ruta y costo
    vector<int> mejorRuta;
    int mejorCosto = INF;

    // Probar todas las combinaciones de nodos
    for (int nodoInicial : noCentralColonies) {
        vector<int> rutaActual;
        int costoActual = 0;

        // Generar subconjuntos de nodos que incluyan todos los no centrales
        vector<vector<int>> subconjuntos;
        for (int mask = 1; mask < (1 << n); mask++) { // Todas las combinaciones de nodos
            vector<int> subconjunto;
            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    subconjunto.push_back(i);
                }
            }
            // Asegurar que el subconjunto incluye todos los nodos no centrales
            bool incluyeTodosNoCentrales = all_of(noCentralColonies.begin(), noCentralColonies.end(),
                                                  [&](int nodo) { return find(subconjunto.begin(), subconjunto.end(), nodo) != subconjunto.end(); });
            if (incluyeTodosNoCentrales) {
                subconjuntos.push_back(subconjunto);
            }
        }

        // Generar permutaciones de cada subconjunto
        for (const auto &subconjunto : subconjuntos) {
            vector<int> permutacion = subconjunto;
            do {
                // Crear ruta completa: nodoInicial -> permutación -> nodoInicial
                rutaActual.clear();
                rutaActual.push_back(nodoInicial);
                for (int nodo : permutacion) {
                    if (nodo != nodoInicial) {
                        rutaActual.push_back(nodo);
                    }
                }
                rutaActual.push_back(nodoInicial);

                // Verificar validez de la ruta usando strictly_connected y calcular costo
                bool esValida = true;
                costoActual = 0;
                for (int i = 0; i < rutaActual.size() - 1; i++) {
                    int desde = rutaActual[i];
                    int hasta = rutaActual[i + 1];
                    if (!strictly_connected[desde][hasta]) { // Usar strictly_connected para validar
                        esValida = false;
                        break;
                    }
                    costoActual += distancias_directas[desde][hasta]; // Calcular costo de la ruta
                }

                // Si es válida, comparar con la mejor solución actual
                if (esValida && costoActual < mejorCosto) {
                    mejorCosto = costoActual;

                    // Guardar correctamente la ruta óptima
                    mejorRuta = rutaActual; // rutaActual ya incluye nodoInicial al principio y al final
                }
            } while (next_permutation(permutacion.begin(), permutacion.end()));
        }
    }

    // Guardar resultados
    check += "\n-------------------\n2 – La ruta óptima.\n";
    if (mejorCosto == INF) {
        check += "No se encontró un camino válido entre las colonias no centrales.\n";
    } else {
        for (size_t i = 0; i < mejorRuta.size() - 1; i++) {
            int desde = mejorRuta[i];
            int hasta = mejorRuta[i + 1];
            int costo = matAdj[desde][hasta];
            check += indexColony[desde] + " -> (" + to_string(costo) + ") -> ";
        }
        check += indexColony[mejorRuta.back()] + "\n"; // Último nodo (cierra el ciclo)
        check += "La Ruta Óptima tiene un costo total de: " + to_string(mejorCosto) + ".\n";
    }
}




//O(q) donde q son las futuras colonias
void Graph::PROBLEMA4() {
    check += "\n-------------------\n4 – Conexión de nuevas colonias.\n\n";
    for(auto &conexion:conexionesFuturas) {
        check += conexion.first + " debe conectarse con " + conexion.second + "\n";
    }
    check += "\n-------------------\n";
}

int main()
{
    int n, m, k, q; //ncolonias, mconexiones, kconCableadoNuevo, qFuturasColonias
    cin>>n>>m>>k>>q;

    Graph ciudad(n,m,k,q);
    ciudad.load();
    ciudad.PROBLEMA1();
    ciudad.PROBLEMA3();
    ciudad.PROBLEMA2();
    ciudad.PROBLEMA4();

    cout<<ciudad.check;

    return 0;
}



/*
INPUT

5 8 1 2
LindaVista 200 120 1
Purisima 150 75 0
Tecnologico -50 20 1
Roma -75 50 0
AltaVista -50 40 0
LindaVista Purisima 48
LindaVista Roma 17
Purisima Tecnologico 40
Purisima Roma 50
Purisima AltaVista 80
Tecnologico Roma 55
Tecnologico AltaVista 15
Roma AltaVista 18
Purisima Tecnologico
Independencia 180 -15
Roble 45 68

5 8 1 2
LindaVista 200 120 0
Purisima 150 75 0
Tecnologico -50 20 0
Roma -75 50 0
AltaVista -50 40 0
LindaVista Purisima 48
LindaVista Roma 17
Purisima Tecnologico 40
Purisima Roma 50
Purisima AltaVista 80
Tecnologico Roma 55
Tecnologico AltaVista 15
Roma AltaVista 18
Purisima Tecnologico
Independencia 180 -15
Roble 45 68
 
 
 
 5 8 1 2
 LindaVista 200 120 0
 Purisima 150 75 0
 Tecnologico -50 20 0
 Roma -75 50 0
 AltaVista -50 40 0
 LindaVista Purisima 48
 LindaVista Roma 17
 Purisima Tecnologico 40
 Purisima Roma 50
 Purisima AltaVista 80
 Tecnologico Roma 55
 Tecnologico AltaVista 85
 Roma AltaVista 18
 Purisima Tecnologico
 Independencia 180 -15
 Roble 45 68
 */

/*
 -------------------
 1 – Cableado óptimo de nueva conexión.
 Tecnologico - AltaVista 15
 LindaVista - Roma 17
 Roma - AltaVista 18
 Costo Total: 50
 -------------------
 2 – La ruta óptima.
 Purisima - Roma - AltaVista - Tecnologico - Purisima
 La Ruta Óptima tiene un costo total de: 123
 -------------------
 3 – Caminos más cortos entre centrales.
 LindaVista - Roma - AltaVista - Tecnologico (50)
 -------------------
 4 – Conexión de nuevas colonias.
 Independencia debe conectarse con Purisima
 Roble debe conectarse con AltaVista
 -------------------
 */
