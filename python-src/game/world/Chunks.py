import threading
import opengl_main_cpp
import settings
from OpenGL.GL import *
from game.world.PerlinNoise import PerlinNoise


def preparePerlinNoise(self, chunk, chunk_x, chunk_z):
    for x in range(chunk_x * 16, chunk_x * 16 + 18):
        for z in range(chunk_z * 16, chunk_z * 16 + 18):
            chunk.setPerlinNoise(int(x - chunk_x * 16), int(z - chunk_z * 16),
                                 int(round(opengl_main_cpp.getNoiseNumber(x, z))))
    self.prepareList.append(chunk)


class Chunks:
    def __init__(self, parent):
        self.worldGen_chunkPosition = [0, 0]
        self.parent = parent
        self.chunks = {}
        self.prepareList = []
        self.prepareCnt = 0
        self.prepareWait = 10
        self.perlinNoise = PerlinNoise(seed=4342, octaves=8)

    def prepareChunk(self, chunk_x, chunk_z):
        chunk = opengl_main_cpp.Chunk(chunk_x, chunk_z)
        self.chunks[(chunk_x, chunk_z)] = chunk
        th = threading.Thread(target=preparePerlinNoise, args=(self, chunk, chunk_x, chunk_z,))
        th.start()

    def checkPrepare(self):
        self.prepareCnt += 1
        if self.prepareCnt < self.prepareWait or not self.prepareList:
            return

        player_x_chunk = int(self.parent.player.x() // 16)
        player_z_chunk = int(self.parent.player.z() // 16)

        self.prepareCnt = 0
        chunk = self.prepareList.pop(-1)
        if abs(chunk.getX() - player_x_chunk) < self.parent.renderDistance / 2 or \
                abs(chunk.getZ() - player_z_chunk) < self.parent.renderDistance / 2:
            chunk.prepareChunk()

    def chunksUpdate(self):
        player_x_chunk = int(self.parent.player.x() // 16)
        player_z_chunk = int(self.parent.player.z() // 16)
        copy = self.chunks.copy()

        for x in range(player_x_chunk - self.parent.renderDistance, player_x_chunk + self.parent.renderDistance):
            for z in range(player_z_chunk - self.parent.renderDistance, player_z_chunk + self.parent.renderDistance):
                if (x, z) not in self.chunks:
                    self.prepareChunk(x, z)

        for position, value in copy.items():
            chunk_x, chunk_z = position
            if (chunk_x > player_x_chunk + self.parent.renderDistance and
                chunk_z > player_z_chunk + self.parent.renderDistance) or \
                    (chunk_x < player_x_chunk - self.parent.renderDistance and
                     chunk_z < player_z_chunk - self.parent.renderDistance):
                value.erase()
                self.chunks.pop(position)

        del copy

    def getBlock(self, x, y, z):
        return -1

    def update(self):
        self.prepareWait = self.parent.renderDistance * 2
        self.chunksUpdate()
        self.checkPrepare()

        glEnable(GL_TEXTURE_2D)
        glBindTexture(GL_TEXTURE_2D, int(self.parent.texture["terrain"]))
        for key, val in self.chunks.items():
            if not val.getPrepared():
                continue
            glPushMatrix()
            glTranslatef(key[0] * 16, 0, key[1] * 16)
            val.render()
            glPopMatrix()
        glBindTexture(GL_TEXTURE_2D, 0)

    def generateChunks(self):
        if self.worldGen_chunkPosition[1] <= settings.RENDER_DISTANCE * 2:
            self.prepareWait = 0
            chunk_x, chunk_z = int(self.worldGen_chunkPosition[0]) - settings.RENDER_DISTANCE, \
                               int(self.worldGen_chunkPosition[1]) - settings.RENDER_DISTANCE
            self.prepareChunk(chunk_x, chunk_z)

            self.worldGen_chunkPosition[0] += 1
            if self.worldGen_chunkPosition[0] >= settings.RENDER_DISTANCE * 2:
                self.worldGen_chunkPosition[0] = 0
                self.worldGen_chunkPosition[1] += 1
