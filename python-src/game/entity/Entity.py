import math
import opengl_main_cpp
from functions import roundPos
from game.models.Model import Model


class Entity:
    def __init__(self, gl):
        self.position = [0, 70, 0]
        self.rotation = [0, 0, 0]
        self.hp = 0
        self.dy = 0
        self.tVel = 50
        self.gravity = 5.8
        self.speed = 0.03
        self.gl = gl

    def jump(self):
        if not self.dy:
            self.dy = 3

    def move(self, dt, dx, dy, dz):
        self.dy -= dt * self.gravity
        self.dy = max(self.dy, -self.tVel)
        dy += self.dy * dt

        if self.dy > 19.8:
            self.dy = 19.8

        x, y, z = self.position
        col = self.collide((x + dx, y + dy, z + dz))
        self.position = list(col)

    def collide(self, pos):
        p = list(pos)
        np = roundPos(pos)
        for face in ((-1, 0, 0), (1, 0, 0), (0, -1, 0), (0, 1, 0), (0, 0, -1), (0, 0, 1)):
            for i in (0, 1, 2):
                if not face[i]:
                    continue
                d = (p[i] - np[i]) * face[i]
                pad = 0.25
                if d < pad:
                    continue
                for dy in (0, 1):
                    op = list(np)
                    op[1] -= dy
                    op[i] += face[i]
                    if opengl_main_cpp.getBlockExist(int(op[0]), int(op[1]), int(op[2])) == 1:
                        p[i] -= (d - pad) * face[i]
                        if face[1]:
                            self.dy = 0
                        break
        return tuple(p)

    def update(self):
        # self.gravity = 9.8 * self.gl.clock.get_fps() / 1000
        # self.speed = 0.1 * self.gl.clock.get_fps() / 1000
        self.gravity = 3.8
        self.speed = 0.03

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

    def hitTest(self, dist=32):
        m = 8
        x, y, z = self.position
        dx, dy, dz = self.get_sight_vector()
        dx /= m
        dy /= m
        dz /= m
        prev = None
        for i in range(dist * m):
            key = roundPos((x, y, z))
            block = opengl_main_cpp.getBlockExist(key[0], key[1], key[2])
            if block != -1:
                return key, prev, block
            prev = key
            x, y, z = x + dx, y + dy, z + dz
        return None, None, block
