from collections import deque, defaultdict

def bfs(graph, start, end):
    # Empezamos la lista con el nodo de inicio, la lista de nodos visitados y la lista de pesos
    queue = deque([(start, [start], [0])])
    visited = set()

    # Mientras la cola no este vacia
    while queue:
        # Sacamos el nodo actual, el camino y los pesos
        current_node, path, weights = queue.popleft()
        if current_node in visited:
            continue

        visited.add(current_node)

        if current_node == end:
            return path, weights[1:]

        for neighbor, weight in graph[current_node]:
            if neighbor not in visited:
                queue.append((neighbor, path + [neighbor], weights + [weight]))

    return [], []

# grafo prueba:
graph_prueba = {
    'A': [('B', 1), ('C', 4)],
    'B': [('C', 2), ('D', 5)],
    'C': [('D', 1)],
    'D': []
}
# lectura de un archivo
graph = defaultdict(list)
with open('txt/grapf.txt') as file:
    #ignorar la primera linea
    # en la primera linea se encuentra el numero de nodos
    nodos = file.readline()
    for i in range(int(nodos)):
        graph[str(i)] = []


    # formato de la linea nodo nodo peso
    for line in file:
        start, end, weight = line.split()
        weight = int(weight)
        graph[start].append((end, weight))

start = '0'
end = '5'
path, weights = bfs(graph, start, end)
print("Path:", path)
print("Weights:", weights)