import math
import opengl_main_cpp
import pygame
from OpenGL.GL import *

from functions import roundPos
from game.entity.Entity import Entity


class Player(Entity):
    def __init__(self, x=0, y=0, z=0, rotation=[0, 0, 0], gl=None):
        super(Player, self).__init__(gl)
        self.position = [x, y, z]
        self.rotation = rotation

        self.cameraShake = [0, False]

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

            if key[pygame.K_w] or key[pygame.K_s] or key[pygame.K_a] or key[pygame.K_d]:
                self.setCameraShake()
            else:
                self.cameraShake = [0, False]

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
        glRotatef(self.rotation[2], 0, 0, 1)
        glTranslatef(-self.position[0], -self.position[1] + self.cameraShake[0], -self.position[2])

    def setCameraShake(self):
        if not self.cameraShake[1]:
            self.cameraShake[0] -= 0.003
            if self.cameraShake[0] < -0.07:
                self.cameraShake[1] = True
        else:
            self.cameraShake[0] += 0.003
            if self.cameraShake[0] > 0.07:
                self.cameraShake[1] = False

    def mouseEvent(self, button):
        #     opengl_main_cpp.removeBlock(blockByVec[0][0], blockByVec[0][1], blockByVec[0][2])
        # elif button == 3 and blockByVec[1]:
        #     opengl_main_cpp.setBlockWithoutBlockCheck(blockByVec[1][0], blockByVec[1][1], blockByVec[1][2], "stone")
        # # self.gl.gui.showText("Height limit for building is 256 blocks")
        blockByVec = self.hitTest()

        if button == 1 and blockByVec[0] and blockByVec[2] != 2:
            self.gl.destroy.destroy(
                opengl_main_cpp.getBlockName(blockByVec[0][0], blockByVec[0][1], blockByVec[0][2]), blockByVec)
        else:
            self.gl.destroy.destroyStage = -1

        if button == 3 and blockByVec[1]:
            playerPos = tuple(roundPos((self.position[0], self.position[1] - 1, self.position[2])))
            playerPos2 = tuple(roundPos((self.position[0], self.position[1], self.position[2])))
            blockByVec = blockByVec[1][0], blockByVec[1][1], blockByVec[1][2]
            if self.inventory.inventory[self.inventory.activeInventory][0] and \
                    self.inventory.inventory[self.inventory.activeInventory][1] and blockByVec != playerPos and \
                    blockByVec != playerPos2:
                blockName = self.inventory.inventory[self.inventory.activeInventory][0]
                opengl_main_cpp.setBlockWithoutBlockCheck(blockByVec[0], blockByVec[1], blockByVec[2], blockName)
                self.gl.blockSound.playBlockSound(blockName)
                self.inventory.inventory[self.inventory.activeInventory][1] -= 1
