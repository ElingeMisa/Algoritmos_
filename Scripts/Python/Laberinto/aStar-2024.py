
import numpy as np
import queue
import random


from pygame.locals import *
from pygame.locals import Color
import pygame
import glob


#	Lee el archivo del mapa
def loadMap(fileName):
	f = open(fileName , "r")
	text = list()
	for line in f:
		aux = line.split('\n')
		text.append( list(map(int, aux[0].split('  '))) )
	#print(text)
	map_cells = np.array(text)
	print(map_cells)
	return(map_cells)



def showCoors(pygame, screen, rows, cols, font, map_cells):
	white = Color('white')
	black = Color('black')

	surface = pygame.Surface((cols * tile_size, rows * tile_size ), pygame.SRCALPHA, 32)
	pygame.draw.rect(  surface, (255, 255, 255, 80),  (0, 0, cols*tile_size, rows*tile_size ) )
	screen.blit(surface, (0, 0))

	for i in range(0, rows ):
		for j in range(0, cols ):
			if map_cells[i][j] == 0:
				textColor = white
			else:
				textColor = black
				
			text = font.render( str( i )+','+ str(j) , True, textColor )
			screen.blit(text, (j*tile_size + (tile_size/6), i*tile_size + (tile_size/3)) )

# Crea el mapa de tiles, calculado las coordenadas usando tile_size
def tiles(screen, map_cells, tile1, tile2):
	for i in range(0, len(map_cells)):
		for j in range(0, len(map_cells[0])):
			if map_cells[i][j] == 0: 
				screen.blit(tile1, (j * tile_size, i * tile_size))
			else:
				screen.blit(tile2, (j * tile_size, i * tile_size))

def yellow_path(pygame, screen, path_tile, path, step):
	for s in range(0, step+1):
		i = path[s][1]
		j = path[s][0]
		
		screen.blit(path_tile, (i*tile_size, j*tile_size) )

def manhattan(a, b):
        # Heurística Manhattan
        return abs(a[0] - b[0]) + abs(a[1] - b[1])

def aStar(mapa, pos_ini, pos_fin, rows, cols):
	# Cola de prioridad para los nodos abiertos
    open_set = queue.PriorityQueue()
    open_set.put((0, pos_ini))  # (f_score, nodo)
    
    # Diccionario para reconstruir el camino
    came_from = {}
    
    # Inicializar puntuaciones
    g_score = { (x, y): float('inf') for x in range(rows) for y in range(cols) }
    g_score[pos_ini] = 0

    f_score = { (x, y): float('inf') for x in range(rows) for y in range(cols) }
    f_score[pos_ini] = manhattan(pos_ini, pos_fin)

    while not open_set.empty():
        # Obtener nodo con menor f_score
        _, current = open_set.get()
        
        # Si alcanzamos el destino, reconstruimos el camino
        if current == pos_fin:
            path = []
            while current in came_from:
                path.append(current)
                current = came_from[current]
            path.append(pos_ini)
            return path[::-1]  # Invertir el camino
        
        # Explorar vecinos
        for i, j in [(0, 1), (1, 0), (0, -1), (-1, 0)]:
            vecino = (current[0] + i, current[1] + j)
            
            # Verificar que el vecino esté dentro del mapa
            if vecino[0] < 0 or vecino[0] >= rows or vecino[1] < 0 or vecino[1] >= cols:
                continue
            
            # Verificar que el vecino no sea un obstáculo
            if mapa[vecino[0]][vecino[1]] == 0:
                continue

            # Calcular coste tentativo
            tentative_g_score = g_score[current] + 1

            # Si encontramos un mejor camino hacia el vecino
            if tentative_g_score < g_score[vecino]:
                came_from[vecino] = current
                g_score[vecino] = tentative_g_score
                f_score[vecino] = g_score[vecino] + manhattan(vecino, pos_fin)
                open_set.put((f_score[vecino], vecino))
    return [pos_ini]
	


def main1():
	global tile_size
	tile_size = 40						# Tamaño de celdas

	pos_init = (9,1)
	pos_target = (3,15)
	
	prefix = "/Users/misa_v/Library/CloudStorage/OneDrive-InstitutoTecnologicoydeEstudiosSuperioresdeMonterrey/Sem 5/Algoritmos_/Scripts/Python/Laberinto/"
	map_cells = loadMap(prefix + "map2.txt") #	Get map from file, as a numpy array
	
	map_cells[pos_init] = '1'			# Initial position 
	map_cells[pos_target] = '10'		# Target position	

	rows = map_cells.shape[0]			#	Number of rows and cols
	cols = map_cells.shape[1]			#	Number of cols

	#	Cambia por la llamada a tu funcion que encuentra el path usando a*
	path  = [(9, 1), (9, 2), (9, 3), (9, 4), (9, 5), (8, 5), (7, 5), (6, 5), (5, 5), (5, 4), (5, 3), (6, 3), (7, 3)]
	path = aStar(map_cells, pos_init, pos_target, rows, cols)            	

	print(map_cells, '\n' )
	print('Steps : ', len(path)-1, '\n', path, '\n')



	#	Display game map: init, set the screen size, the refresh time, title, font
	pygame.init()
	screen = pygame.display.set_mode(( cols*tile_size, rows*tile_size ))
	clock = pygame.time.Clock()
	pygame.display.set_caption( 'Skull' )
	font = pygame.font.SysFont('arial.ttf', 20)

	#	Carga las imagenes de tiles y las escala
	img1 = pygame.image.load(prefix + "tile1.png")
	img2 = pygame.image.load(prefix + "tile2.png")
	img3 = pygame.image.load(prefix + "skull.png")
	img4 = pygame.image.load(prefix + "treasure.png")
	tile_green = pygame.transform.scale(img1, (tile_size, tile_size))
	tile_grey = pygame.transform.scale(img2, (tile_size, tile_size))
	skull = pygame.transform.scale(img3, (tile_size, tile_size))		# imagen skull 
	treasure = pygame.transform.scale(img4, (tile_size, tile_size))		# imagen treasure 

	path_tile = pygame.Surface((tile_size, tile_size))  				# tile amarillo para el path
	path_tile.fill( Color('yellow') ) 

	loop = 1				#	Flag para el loop de animacion
	step = 0				# 	Contador de pasos para el path
	litle_step = 0.0		#	Tamaño de paso
	dir_h = 0				# 	Variables para la dirrecion de moviemtos
	up_down = 1
	left_rigth = 1

	#	x,y position of the skull
	xb = pos_init[1] * tile_size 
	yb = pos_init[0] * tile_size 

	#	"Dibujar" es un ciclo: en cada clock tick, se refresca
	while loop:
		tiles(screen, map_cells, tile_green, tile_grey)
		screen.blit(treasure, (pos_target[1] * tile_size, pos_target[0] * tile_size) )
		
		yellow_path(pygame, screen, path_tile, path, step )
		screen.blit(skull, (xb, yb) )
		
		#showCoors(pygame, screen, rows, cols, font, map_cells)
		

		for event in pygame.event.get():
			if event.type == QUIT:
				loop = 0

		

		# Averigua en que direccion se mueve para controlar la animacion
		if step < (len(path) -1) :
			if path[step][1] == path[step+1][1]:
				dir_h = 1
				if path[step][0] > path[step+1][0]:
					left_rigth = 1
				else: 
					left_rigth = -1
			else:
				dir_h = 0
				if path[step][1] > path[step+1][1]:
					up_down = 1
				else: 
					up_down = -1
		elif step >= len(path) :
			litle_step = 0
			step = 0

		litle_step += 0.10 
		xb = path[step][1] * tile_size   
		yb = path[step][0] * tile_size 


		#	Esto hace que avance despacio, y lo reinicia
		if litle_step > 0.90 :
			litle_step = 0
			step += 1
			if step >= len(path):
				step = 0


		# Todo lo que quieras dibujar va antes de estas lineas
		pygame.display.update()
		screen.fill((0, 0, 0))
		clock.tick(32)
	
	pygame.quit()



if __name__ == '__main__':
	main1()
