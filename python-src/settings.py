import timeit

import pygame
from pyglet import font

font.add_file('gui/main.ttf')
mainFont = font.load('gui/main.ttf', 15)

pygame.init()

monitor = pygame.display.Info()
WIDTH = 854  # monitor.current_w
HEIGHT = 480  # monitor.current_h
MAX_FPS = 1000
PAUSE = True
IN_MENU = True
MC_VERSION = "1.0"
clock = pygame.time.Clock()

FOV = 90.0
RENDER_DISTANCE = 10

CHUNKS_RENDER_DISTANCE = 900
CHUNK_SIZE = (4, 60, 4)
