import math
from random import randint
import debug_module
import pyglet
from OpenGL.GL import *
import opengl_main_cpp
from game.blocks.BlockEvent import *
from functions import roundPos, flatten, cube_vertices
from game.blocks.DestroyBlock import DestroyBlock
from settings import *


class Player:
    def __init__(self, x=0, y=0, z=0, rotation=[0, 0], gl=None):
        debug_module._gl_engine_info("_Player_python", "Init Player class...")

        self.position, self.rotation = [x, y, z], rotation
        self.speed = 0.02 * gl.clock.get_fps() / 1000  # 0.02
        self.gl = gl
        self.gravity = 5.8
        self.tVel = 50
        self.dy = 0
        self.shift = 0
        self.cameraShake = [0, False]
        self.canShake = True
        self.acceleration = 0
        self.lastShiftPos = self.position
        self.cameraType = 1
        self.hp = -1
        self.bInAir = False
        self.playerDead = False
        self.inventory = None
        self.sensitivity = 2

        self.lastPlayerPosOnGround = [0, 0, 0]
        self.playerFallY = 0

        self.kW, self.kS, self.kA, self.kD = 0, 0, 0, 0

    def setCameraShake(self):
        if not self.canShake or self.shift > 0:
            return

        if not self.cameraShake[1]:
            self.cameraShake[0] -= 0.007
            if self.cameraShake[0] < -0.1:
                self.cameraShake[1] = True
        else:
            self.cameraShake[0] += 0.007
            if self.cameraShake[0] > 0.1:
                self.cameraShake[1] = False

    def setShift(self, b):
        if b:
            if self.shift < 0.17:
                self.shift += 0.05
        else:
            if self.shift > 0:
                self.shift -= 0.05

    def updatePosition(self):
        if self.gl.allowEvents["movePlayer"]:
            self.speed = 1 * self.gl.clock.get_fps() / 1000

            rdx, rdy = pygame.mouse.get_pos()
            rdx, rdy = rdx - self.gl.WIDTH // 2, rdy - self.gl.HEIGHT // 2
            rdx /= self.sensitivity
            rdy /= self.sensitivity
            self.rotation[0] += rdy * self.gl.clock.get_fps() / 1000
            self.rotation[1] += rdx * self.gl.clock.get_fps() / 1000
            if self.rotation[0] > 88:
                self.rotation[0] = 88
            elif self.rotation[0] < -88:
                self.rotation[0] = -88

            if self.rotation[1] > 360:
                self.rotation[1] = 0
            elif self.rotation[1] < 0:
                self.rotation[1] = 360

            DX, DY, DZ = 0, 0, 0

            rotY = self.rotation[1] / 180 * math.pi
            dx, dz = (self.speed + self.acceleration - 0.008) * math.sin(rotY), \
                     (self.speed + self.acceleration - 0.008) * math.cos(rotY)

            key = pygame.key.get_pressed()
            if self.kW > 0 or key[pygame.K_w]:
                DX += dx
                DZ -= dz
            if self.kS > 0 or key[pygame.K_s]:
                DX -= dx
                DZ += dz
            if self.kA > 0 or key[pygame.K_a]:
                DX -= dz
                DZ -= dx
            if self.kD > 0 or key[pygame.K_d]:
                DX += dz
                DZ += dx
            if key[pygame.K_SPACE]:
                DY += self.speed
                # self.jump()
            if key[pygame.K_LSHIFT]:
                DY -= self.speed
            self.position = [self.position[0] + DX, self.position[1] + DY, self.position[2] + DZ]
            # self.move(self.speed, DX, DY, DZ)

        glRotatef(self.rotation[0], 1, 0, 0)
        glRotatef(self.rotation[1], 0, 1, 0)
        glTranslatef(-self.position[0],
                     -self.position[1] + self.shift + self.cameraShake[0],
                     -self.position[2])

    def move(self, dt, dx, dy, dz):
        self.dy -= dt * self.gravity
        self.dy = max(self.dy, -self.tVel)
        dy += self.dy * dt

        if self.dy > 19.8:
            self.dy = 19.8

        x, y, z = self.position
        col = self.collide((dx, dy, dz))
        self.position = col

    def collide(self, pos):
        xa = pos[0]
        ya = pos[1]
        za = pos[2]
        chunk_position = self.getPositionInChunk(self.position[0], self.position[1], self.position[2])
        aabb = opengl_main_cpp.AABB(
            self.position[0] - 0.6 / 2.0, self.position[1] - 1.8 / 2.0, self.position[2] - 0.6 / 2.0,
            self.position[0] + 0.6 / 2.0, self.position[1] + 1.8 / 2.0, self.position[2] + 0.6 / 2.0)

        for face in ((-1, 0, 0), (1, 0, 0), (0, -1, 0), (0, 1, 0), (0, 0, -1), (0, 0, 1)):
            block = self.gl.chunks.getBlockAABB(
                int(chunk_position[0] + face[0]),
                int(chunk_position[1] + face[1]),
                int(chunk_position[2] + face[2])
            )  # TODO
            if block.getExist():
                xa = block.clipYCollide(aabb, self.position[0] + pos[0])
                aabb.move(xa, 0.0, 0.0)
                ya = block.clipYCollide(aabb, self.position[1] + pos[1])
                aabb.move(0.0, ya, 0.0)
                za = block.clipYCollide(aabb, self.position[2] + pos[2])
                aabb.move(0.0, 0.0, za)

        if ya != pos[1]:
            self.dy = 0

        return (aabb.getMinX() + aabb.getMaxX()) / 2.0, aabb.getMinY(), (aabb.getMinZ() + aabb.getMaxZ()) / 2.0

    def get_sight_vector(self):
        rotX, rotY = -self.rotation[0] / 180 * math.pi, self.rotation[1] / 180 * math.pi
        dx, dz = math.sin(rotY), -math.cos(rotY)
        dy, m = math.sin(rotX), math.cos(rotX)
        return dx * m, dy, dz * m

    def jump(self):
        if not self.dy:
            self.dy = 4

    def dead(self):
        self.playerDead = True
        self.gl.deathScreen()
        for i in self.inventory.inventory.items():
            for j in range(i[1][1]):
                self.gl.droppedBlock.addBlock((
                    self.position[0] + randint(-2, 2), self.position[1], self.position[2] + randint(-2, 2)
                ), i[1][0])
            self.inventory.inventory[i[0]] = [i[1][0], 0]

    def mouseEvent(self, button):
        blockByVec = self.hitTest(self.position, self.get_sight_vector())

        if button == 1 and blockByVec[0]:
            self.gl.chunks.removeBlock(blockByVec[0][0], blockByVec[0][1], blockByVec[0][2])
            # self.gl.destroy.destroy(self.gl.cubes.cubes[blockByVec[0]].name, blockByVec)
        elif button == 3 and blockByVec[1]:
            pass  # if blockByVec[2] == -342:
            #     self.gl.gui.showText("Height limit for building is 256 blocks")
        else:
            pass  # self.gl.destroy.destroyStage = -1
        return

        if button == 2 and blockByVec[0]:
            if self.inventory.inventory[self.inventory.activeInventory][1] == 0:
                itm = -1
                for item in self.inventory.inventory.items():
                    i = item[1]
                    if i[0] == self.gl.cubes.cubes[blockByVec[0]].name and i[1] != 0:
                        itm = item[0]
                        break
                if itm != -1:
                    self.inventory.inventory[self.inventory.activeInventory] = [
                        self.inventory.inventory[itm][0], self.inventory.inventory[itm][1]]
                    self.inventory.inventory[itm][1] = 0
                    self.gl.gui.showText(self.inventory.inventory[itm][0])
        if button == 3:
            if blockByVec[0] and self.shift <= 0:
                if blockByVec[0] in self.gl.cubes.cubes:
                    if canOpenBlock(self, self.gl.cubes.cubes[blockByVec[0]], self.gl):
                        openBlockInventory(self, self.gl.cubes.cubes[blockByVec[0]], self.gl)
                        return
            if blockByVec[1] and self.shift <= 0:
                if blockByVec[1] in self.gl.cubes.cubes:
                    if canOpenBlock(self, self.gl.cubes.cubes[blockByVec[1]], self.gl):
                        openBlockInventory(self, self.gl.cubes.cubes[blockByVec[1]], self.gl)
                        return
            if blockByVec[1]:
                playerPos = tuple(roundPos((self.position[0], self.position[1] - 1, self.position[2])))
                playerPos2 = tuple(roundPos((self.position[0], self.position[1], self.position[2])))
                blockByVec = blockByVec[1][0], blockByVec[1][1], blockByVec[1][2]
                if self.inventory.inventory[self.inventory.activeInventory][0] and \
                        self.inventory.inventory[self.inventory.activeInventory][1] and blockByVec != playerPos and \
                        blockByVec != playerPos2:
                    self.gl.cubes.add(blockByVec, self.inventory.inventory[self.inventory.activeInventory][0], now=True)
                    self.gl.blockSound.playBlockSound(self.gl.cubes.cubes[blockByVec].name)
                    self.inventory.inventory[self.inventory.activeInventory][1] -= 1

    def getPositionInChunk(self, x, y, z):
        num_x = x / 16
        num_z = z / 16

        num_x = float(str(num_x - int(num_x))[1:])
        num_z = float(str(num_z - int(num_z))[1:])

        if num_x == 0:
            num_x = 16 / 100

        if num_z == 0:
            num_z = 16 / 100

        player_x_chunk = round(num_x * 16)
        player_z_chunk = round(num_z * 16)

        return player_x_chunk, y, player_z_chunk

    def hitTest(self, p, vec, dist=32):
        m = 8
        x, y, z = self.getPositionInChunk(p[0], p[1], p[2])
        dx, dy, dz = vec
        dx /= m
        dy /= m
        dz /= m
        prev = None
        for i in range(dist * m):
            key = roundPos((x, y, z))
            block = self.gl.chunks.getBlock(key[0], key[1], key[2])
            if block != -1:
                return key, prev, block
            prev = key
            x, y, z = x + dx, y + dy, z + dz
        return None, None, block

    def x(self):
        return self.position[0]

    def y(self):
        return self.position[1]

    def z(self):
        return self.position[2]

    def update(self):
        self.updatePosition()
