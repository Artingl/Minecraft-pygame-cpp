import pygame
import game

print(game.greet())

w, h = 868, 468

pygame.init()
clock = pygame.time.Clock()
scr = pygame.display.set_mode((w, h))

while True:
    for e in pygame.event.get():
        if e.type == pygame.QUIT:
            exit(0)
    scr.fill((230, 140, 120))

    pygame.display.flip()
    clock.tick(250)
