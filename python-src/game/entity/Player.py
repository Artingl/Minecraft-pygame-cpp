import math
import opengl_main_cpp
import pygame
from OpenGL.GL import *
from game.entity.Entity import Entity


class Player(Entity):
    def __init__(self, x=0, y=0, z=0, rotation=[0, 0], gl=None):
        super(Player, self).__init__(gl)
        self.position = [x, y, z]
        self.rotation = rotation
        self.inventory = None

    def update(self):
        super().update()

        if self.gl.allowEvents["movePlayer"]:
            rdx, rdy = pygame.mouse.get_pos()
            rdx, rdy = rdx - self.gl.WIDTH // 2, rdy - self.gl.HEIGHT // 2
            rdx /= 8
            rdy /= 8
            self.rotation[0] += rdy  # * self.gl.clock.get_fps() / 1000
            self.rotation[1] += rdx  # * self.gl.clock.get_fps() / 1000
            if self.rotation[0] > 90:
                self.rotation[0] = 90
            elif self.rotation[0] < -90:
                self.rotation[0] = -90

            if self.rotation[1] > 360:
                self.rotation[1] = 0
            elif self.rotation[1] < 0:
                self.rotation[1] = 360

            DX, DY, DZ = 0, 0, 0

            rotY = self.rotation[1] / 180 * math.pi
            dx, dz = self.speed * math.sin(rotY), self.speed * math.cos(rotY)

            key = pygame.key.get_pressed()

            if key[pygame.K_w]:
                DX += dx
                DZ -= dz
            if key[pygame.K_s]:
                DX -= dx
                DZ += dz
            if key[pygame.K_a]:
                DX -= dz
                DZ -= dx
            if key[pygame.K_d]:
                DX += dz
                DZ += dx
            if key[pygame.K_SPACE]:
                self.jump()

            dt = self.speed
            if dt < 0.2:
                dt /= 10
                DX /= 10
                DY /= 10
                DZ /= 10
                for i in range(10):
                    self.move(dt, DX, DY, DZ)

        glRotatef(self.rotation[0], 1, 0, 0)
        glRotatef(self.rotation[1], 0, 1, 0)
        glTranslatef(-self.position[0], -self.position[1], -self.position[2])

    def mouseEvent(self, button):
        blockByVec = self.hitTest()

        if button == 1 and blockByVec[0] and blockByVec[2] != 2:
            opengl_main_cpp.removeBlock(blockByVec[0][0], blockByVec[0][1], blockByVec[0][2])
        elif button == 3 and blockByVec[1]:
            opengl_main_cpp.setBlock(blockByVec[1][0], blockByVec[1][1], blockByVec[1][2], "dirt")
            #     self.gl.gui.showText("Height limit for building is 256 blocks")
        else:
            pass  # self.gl.destroy.destroyStage = -1
