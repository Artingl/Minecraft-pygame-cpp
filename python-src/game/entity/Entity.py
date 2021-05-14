import math
import time
import debug_module
import opengl_main_cpp
from functions import roundPos
from game.models.Model import Model


class Entity:
    def __init__(self, gl):
        self.position = [0, 70, 0]
        self.rotation = [0, 0, 0]
        self.hp = 20
        self.dy = 0
        self.lastHeight = -1
        self.tVel = 50
        self.gravity = 5.8
        self.speed = 0.03
        self.jump_speed = (2 * self.gravity) ** .5
        self.gl = gl

    def jump(self):
        if opengl_main_cpp.isUnderWater(int(self.x()), int(self.y()), int(self.z())):
            self.dy = self.jump_speed / 2
        else:
            if not self.dy:
                self.dy = 1.9

    def move(self, dt, dx, dy, dz):
        self.dy -= dt * self.gravity
        self.dy = max(self.dy, -self.tVel)
        dy += self.dy * dt

        if self.dy > 19.8:
            self.dy = 19.8

        x, y, z = self.position
        col = self.collide((x + dx, y + dy, z + dz))
        self.position = list(col)

        if opengl_main_cpp.getBlockExist(int(self.x()), int(self.y() - 1), int(self.z())) != 1:
            cnt = -1
            for i in range(2, 20):
                if opengl_main_cpp.getBlockExist(int(self.x()), int(self.y() - i + 2), int(self.z())) == 1:
                    cnt = i - 2
                    break
            if cnt != -1:
                if self.lastHeight < cnt:
                    self.lastHeight = cnt
            elif self.lastHeight != -1:
                print(self.lastHeight)
                self.lastHeight = -1

    def collide(self, pos):
        p = list(pos)
        np = roundPos(pos)
        for face in ((-1, 0, 0), (1, 0, 0), (0, -1, 0), (0, 1, 0), (0, 0, -1), (0, 0, 1)):
            for i in (0, 1, 2):
                if not face[i]:
                    continue
                d = (p[i] - np[i]) * face[i]
                pad = 0 if i == 1 and face[i] < 0 else 0.25
                if d < pad:
                    continue
                for dy in (0, 1):
                    op = list(np)
                    op[1] -= dy
                    op[i] += face[i]
                    if opengl_main_cpp.getBlockExist(op[0], op[1], op[2]) == 1:  # tuple(op) in self.cubes.collidable:
                        p[i] -= (d - pad) * face[i]
                        if face[1]:
                            self.dy = 0
                        break
        return tuple(p)

    def update(self):
        # self.gravity = 9.8 * self.gl.clock.get_fps() / 1000
        # self.speed = 0.1 * self.gl.clock.get_fps() / 1000
        if opengl_main_cpp.isUnderWater(int(self.x()), int(self.y()), int(self.z())):
            self.gravity = 6.8 * self.gl.clock.get_fps() / 1000
            self.speed = 0.02
        else:
            self.gravity = 9.8 * self.gl.clock.get_fps() / 1000
            self.speed = 0.03
        self.jump_speed = (2 * self.gravity) ** .5

        if self.position[0] > 128:
            self.position[0] = 128
        if self.position[0] < -128:
            self.position[0] = -128
        if self.position[2] > 128:
            self.position[2] = 128
        if self.position[2] < -128:
            self.position[2] = -128

    def x(self):
        return self.position[0]

    def y(self):
        return self.position[1]

    def z(self):
        return self.position[2]

    def get_sight_vector(self):
        rotX, rotY = -self.rotation[0] / 180 * math.pi, self.rotation[1] / 180 * math.pi
        dx, dz = math.sin(rotY), -math.cos(rotY)
        dy, m = math.sin(rotX), math.cos(rotX)
        return dx * m, dy, dz * m

    def hitTest(self, dist=6):
        m = 100
        x, y, z = self.position
        dx, dy, dz = self.get_sight_vector()
        dx /= m
        dy /= m
        dz /= m
        saved = []
        for i in range(dist * m):
            key = roundPos((x, y, z))
            block = opengl_main_cpp.getBlockExist(key[0], key[1], key[2])
            if block != -1:
                prev = None
                try:
                    prev = saved[-1]
                except:
                    pass
                return key, prev, block
            if key not in saved:
                saved.append(key)
            else:
                saved.pop(saved.index(key))
                saved.append(key)
            x, y, z = x + dx, y + dy, z + dz
        return None, None, block
