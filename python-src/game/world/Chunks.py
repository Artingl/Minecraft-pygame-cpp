import opengl_main_cpp
import settings
from OpenGL.GL import *

from game.world.PerlinNoise import PerlinNoise


class Chunks:
    def __init__(self, parent):
        self.worldGen_chunkPosition = [0, 0]
        self.parent = parent
        self.chunks = {}
        self.perlinNoise = PerlinNoise(seed=4342, mh=8)

    def chunksUpdate(self):
        pass

    def getBlock(self, x, y, z):
        return -1

    def update(self):
        self.chunksUpdate()

        glEnable(GL_TEXTURE_2D)
        glEnable(GL_BLEND)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        glEnable(GL_DEPTH_TEST)
        glDepthFunc(GL_LEQUAL)
        glEnable(GL_ALPHA_TEST)
        glAlphaFunc(GL_GREATER, 0.5)
        glBindTexture(GL_TEXTURE_2D, int(self.parent.texture["terrain"]))
        for key, val in self.chunks.items():
            glPushMatrix()
            glTranslatef(key[0] * 16, 0, key[1] * 16)
            val.render()
            glPopMatrix()
        glBindTexture(GL_TEXTURE_2D, 0)

    def generateChunks(self):
        if self.worldGen_chunkPosition[1] <= settings.RENDER_DISTANCE * 2:
            chunk_x, chunk_z = int(self.worldGen_chunkPosition[0]) - settings.RENDER_DISTANCE, \
                               int(self.worldGen_chunkPosition[1]) - settings.RENDER_DISTANCE

            chunk = opengl_main_cpp.Chunk(chunk_x, chunk_z)
            self.chunks[(chunk_x, chunk_z)] = chunk
            for x in range(chunk_x * 16, chunk_x * 16 + 18):
                for z in range(chunk_z * 16, chunk_z * 16 + 18):
                    chunk.setPerlinNoise(int(x - chunk_x * 16), int(z - chunk_z * 16), self.perlinNoise(x, z))
            chunk.prepareChunk()

            self.worldGen_chunkPosition[0] += 1
            if self.worldGen_chunkPosition[0] >= settings.RENDER_DISTANCE * 2:
                self.worldGen_chunkPosition[0] = 0
                self.worldGen_chunkPosition[1] += 1
