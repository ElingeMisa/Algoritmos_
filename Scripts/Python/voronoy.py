import numpy as np
import matplotlib.pyplot as plt
import math

# Clase para los puntos
class Punto:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def show(self):
        return [self.x, self.y]
    
class PuntoConNombre:
    def __init__(self, name, x, y):
        self.name = name
        self.x = x
        self.y = y

    def show(self):
        return [self.name, self.x, self.y]

# Clase para los segmentos
class Segmento:
    def __init__(self, A, B):
        self.A = A
        self.B = B

    def show(self):
        return [self.A.x, self.A.y, self.B.x, self.B.y]

# Clase para los triángulos
class Triangulo:
    def __init__(self, puntos):
        self.puntos = puntos
        self.circuncentro, self.circunradio = calcular_circuncentro_y_radio(*puntos)

# Calcula circuncentro y radio
def calcular_circuncentro_y_radio(p1, p2, p3):
    D = 2 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y))
    if D == 0:
        return None, None
    Ux = ((p1.x**2 + p1.y**2) * (p2.y - p3.y) + (p2.x**2 + p2.y**2) * (p3.y - p1.y) + (p3.x**2 + p3.y**2) * (p1.y - p2.y)) / D
    Uy = ((p1.x**2 + p1.y**2) * (p3.x - p2.x) + (p2.x**2 + p2.y**2) * (p1.x - p3.x) + (p3.x**2 + p3.y**2) * (p2.x - p1.x)) / D
    radio = math.sqrt((Ux - p1.x)**2 + (Uy - p1.y)**2)
    return Punto(Ux, Uy), radio

# Generar la triangulación de Delaunay
def triangulacion_delaunay(puntos):
    triangulos = []
    for i in range(len(puntos)):
        for j in range(i + 1, len(puntos)):
            for k in range(j + 1, len(puntos)):
                p1, p2, p3 = puntos[i], puntos[j], puntos[k]
                centro, radio = calcular_circuncentro_y_radio(p1, p2, p3)
                if centro is None:
                    continue
                es_delaunay = all(math.sqrt((centro.x - p.x)**2 + (centro.y - p.y)**2) >= radio for p in puntos if p not in [p1, p2, p3])
                if es_delaunay:
                    triangulos.append(Triangulo([p1, p2, p3]))
    return triangulos

# Construir el diagrama de Voronoi
def construir_voronoi(triangulos):
    celdas = {}
    for tri in triangulos:
        for punto in tri.puntos:
            if punto not in celdas:
                celdas[punto] = []
            celdas[punto].append(tri.circuncentro)
    return celdas

# Asignar cada punto de 'deaths.csv' a su celda de Voronoi más cercana
def asignar_puntos_a_celdas(puntos_deaths, celdas):
    asignaciones = []
    for death in puntos_deaths:
        dist_min = float('inf')
        celda_asignada = None
        for sitio, vertices in celdas.items():
            dist = math.sqrt((sitio.x - death.x)**2 + (sitio.y - death.y)**2)
            if dist < dist_min:
                dist_min = dist
                celda_asignada = sitio
        asignaciones.append((death, celda_asignada))
    return asignaciones

# Graficar el diagrama de Voronoi
def graficar_voronoi(puntos, celdas, puntos_deaths, asignaciones):
    fig, ax = plt.subplots()
    ax.set_aspect('equal')
    colores = ['b', 'g', 'r', 'c', 'm', 'y', 'k']
    for i, (sitio, vertices) in enumerate(celdas.items()):
        color = colores[i % len(colores)]
        x_vals = [v.x for v in vertices]
        y_vals = [v.y for v in vertices]
        ax.fill(x_vals, y_vals, color=color, alpha=0.3)
        ax.plot(sitio.x, sitio.y, 'bo')  # Sitios de Voronoi en azul
    for death, sitio in asignaciones:
        ax.plot(death.x, death.y, 'ro')
        ax.plot([death.x, sitio.x], [death.y, sitio.y], 'k--', linewidth=0.5)
    plt.show()

# ---- Ejecución del código ---- #
archivo_pumps = "/Users/misa_v/Library/CloudStorage/OneDrive-InstitutoTecnologicoydeEstudiosSuperioresdeMonterrey/Sem 5/Algoritmos_/Scripts/Python/pumps.csv"
archivo_deaths = "/Users/misa_v/Library/CloudStorage/OneDrive-InstitutoTecnologicoydeEstudiosSuperioresdeMonterrey/Sem 5/Algoritmos_/Scripts/Python/deaths.csv"
puntos_name = cargar_puntos_name(archivo_pumps)
puntos_deaths = cargar_deaths(archivo_deaths)

puntos = [Punto(p.x, p.y) for p in puntos_name]
triangulos = triangulacion_delaunay(puntos)
celdas_voronoi = construir_voronoi(triangulos)
asignaciones = asignar_puntos_a_celdas(puntos_deaths, celdas_voronoi)
graficar_voronoi(puntos_name, celdas_voronoi, puntos_deaths, asignaciones)
