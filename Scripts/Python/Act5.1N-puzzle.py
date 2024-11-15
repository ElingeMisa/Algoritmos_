import heapq

# Función para calcular la distancia de Manhattan
def manhattan_dist(tablero, objetivo):
    dist = 0
    for i in range(len(tablero)):
        if tablero[i] == '_':
            continue
        objetivo_idx = objetivo.index(tablero[i])
        dist += abs(i // 3 - objetivo_idx // 3) + abs(i % 3 - objetivo_idx % 3)
    return dist

# Función para generar los movimientos válidos
def obtener_vecinos(estado):
    vecinos = []
    filas, columnas = 3, 3
    idx_vacio = estado.index('_')
    fila, columna = idx_vacio // columnas, idx_vacio % columnas
    movimientos = {
        "arriba": (fila - 1, columna),
        "abajo": (fila + 1, columna),
        "izquierda": (fila, columna - 1),
        "derecha": (fila, columna + 1)
    }
    for direccion, (nueva_fila, nueva_columna) in movimientos.items():
        if 0 <= nueva_fila < filas and 0 <= nueva_columna < columnas:
            nuevo_idx = nueva_fila * columnas + nueva_columna
            nuevo_estado = list(estado)
            nuevo_estado[idx_vacio], nuevo_estado[nuevo_idx] = nuevo_estado[nuevo_idx], '_'
            vecinos.append(tuple(nuevo_estado))
    return vecinos

# Algoritmo A* para resolver el N-puzzle
def aStar_puzzle(inicio, objetivo):
    open_set = []
    heapq.heappush(open_set, (0, inicio))  # (f_score, estado)
    came_from = {}
    g_score = {inicio: 0}
    f_score = {inicio: manhattan_dist(inicio, objetivo)}
    
    while open_set:
        _, current = heapq.heappop(open_set)
        
        # Si alcanzamos el estado objetivo
        if current == objetivo:
            path = []
            while current in came_from:
                path.append(current)
                current = came_from[current]
            path.append(inicio)
            return path[::-1]  # Invertimos el camino
        
        # Generar vecinos
        for vecino in obtener_vecinos(current):
            tentative_g_score = g_score[current] + 1
            if vecino not in g_score or tentative_g_score < g_score[vecino]:
                came_from[vecino] = current
                g_score[vecino] = tentative_g_score
                f_score[vecino] = g_score[vecino] + manhattan_dist(vecino, objetivo)
                heapq.heappush(open_set, (f_score[vecino], vecino))
    
    return []  # Si no se encuentra solución

# Caso de prueba
inicio = ('3', '2', '_', '7', '1', '4', '6', '5', '8')
objetivo = ('1', '2', '3', '4', '5', '6', '7', '8', '_')

camino = aStar_puzzle(inicio, objetivo)
for paso in camino:
    print("\n".join(" ".join(paso[i:i+3]) for i in range(0, len(paso), 3)))
    print("---")
