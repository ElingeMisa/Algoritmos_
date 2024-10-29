import math
####################################################################################################
# Leer un archivo de texto con coordenadas de puntos y encontrar el par de puntos más cercanos
def lectura_puntos(filenamoe):
    with open(filenamoe) as file:
        n = int(file.readline().strip())
        coor = []
        for i in range(n):
            x, y = map(float, file.readline().split())
            coor.append((x, y))
    return coor

####################################################################################################

def print_map():
  print("=====================================\nPuntos:[Indice]\t\t[x]\t\t[y]")
  for i in mapa:
   n = mapa.index(i)
   #imprime los puntos separados por tabulaciones
   salida = "\t[" + str(n) + "]\t     [" + str(i[0]) + "]  \t\t[" + str(i[1]) + "]"
   print(salida)
  print("=====================================\n")

def distancia(p1, p2):
    return math.sqrt((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2)

def closest_pair_rec(points):
    if len(points) <= 3:
        min_dist = float('inf')
        pair = None
        for i in range(len(points)):
            for j in range(i + 1, len(points)):
                d = distancia(points[i], points[j])
                if d < min_dist:
                    min_dist = d
                    pair = (points[i], points[j])
        return pair, min_dist

    mid = len(points) // 2
    left_points = points[:mid]
    right_points = points[mid:]

    (p1, q1), dist1 = closest_pair_rec(left_points)
    (p2, q2), dist2 = closest_pair_rec(right_points)

    if dist1 < dist2:
        d = dist1
        min_pair = (p1, q1)
    else:
        d = dist2
        min_pair = (p2, q2)

    mid_x = points[mid][0]
    in_strip = [p for p in points if abs(p[0] - mid_x) < d]
    in_strip.sort(key=lambda point: point[1])

    for i in range(len(in_strip)):
        for j in range(i + 1, len(in_strip)):
            if (in_strip[j][1] - in_strip[i][1]) >= d:
                break
            dist = distancia(in_strip[i], in_strip[j])
            if dist < d:
                d = dist
                min_pair = (in_strip[i], in_strip[j])

    return min_pair, d

def closest_pair(points):
    # Ordenar los puntos por coordenada x
    points.sort()
    return closest_pair_rec(points)

instancias = [
    "/Users/misa_v/Library/CloudStorage/OneDrive-InstitutoTecnologicoydeEstudiosSuperioresdeMonterrey/Sem 5/Algoritmos_/Scripts/Python/txt/puntos-n8.txt",
    "/Users/misa_v/Library/CloudStorage/OneDrive-InstitutoTecnologicoydeEstudiosSuperioresdeMonterrey/Sem 5/Algoritmos_/Scripts/Python/txt/puntos-n10.txt",
    "/Users/misa_v/Library/CloudStorage/OneDrive-InstitutoTecnologicoydeEstudiosSuperioresdeMonterrey/Sem 5/Algoritmos_/Scripts/Python/txt/puntos-n11.txt",
    "/Users/misa_v/Library/CloudStorage/OneDrive-InstitutoTecnologicoydeEstudiosSuperioresdeMonterrey/Sem 5/Algoritmos_/Scripts/Python/txt/puntos-n15.txt",
    "/Users/misa_v/Library/CloudStorage/OneDrive-InstitutoTecnologicoydeEstudiosSuperioresdeMonterrey/Sem 5/Algoritmos_/Scripts/Python/txt/puntos-n16.txt",
    "/Users/misa_v/Library/CloudStorage/OneDrive-InstitutoTecnologicoydeEstudiosSuperioresdeMonterrey/Sem 5/Algoritmos_/Scripts/Python/txt/puntos-n20.txt",
    "/Users/misa_v/Library/CloudStorage/OneDrive-InstitutoTecnologicoydeEstudiosSuperioresdeMonterrey/Sem 5/Algoritmos_/Scripts/Python/txt/puntos-n50.txt",
    "/Users/misa_v/Library/CloudStorage/OneDrive-InstitutoTecnologicoydeEstudiosSuperioresdeMonterrey/Sem 5/Algoritmos_/Scripts/Python/txt/puntos-n100.txt"
]
filenames = [
    "puntos-n8.txt",
    "puntos-n10.txt",
    "puntos-n11.txt",
    "puntos-n15.txt",
    "puntos-n16.txt",
    "puntos-n20.txt",
    "puntos-n50.txt",
    "puntos-n100.txt"
]
this_instance = 0
print("Busqueda de los puntos más cercanos ...")

for i in range(len(instancias)):
    filename = instancias[this_instance]
    mapa = lectura_puntos(filename)
    #print_map()
    closest_points, min_distance = closest_pair(mapa)
    #imprime el par de puntos mas cercanos y la distancia entre ellos , tmabien incluye el indice de los puntos
    print("=========================================================================================")
    print(f"\t\tArchivo: {filenames[this_instance]}")
    print(f"\t\tLos puntos más cercanos son: {closest_points[0]} y {closest_points[1]}")
    print(f"\t\tDistancia: {min_distance}") 
    print("=========================================================================================\n")
    this_instance += 1