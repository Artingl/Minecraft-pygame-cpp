import threading
import opengl_main_cpp
import settings
from OpenGL.GL import *


class Chunks:
    def __init__(self, parent):
        self.worldGen_chunkPosition = [0, 0]
        self.parent = parent
        self.chunks = {}
        self.prepareList = []
        self.prepareCnt = 0
        self.prepareWait = 10
        self.seed = 3424

    def prepareChunk(self, chunk_x, chunk_z):
        chunk = opengl_main_cpp.Chunk(chunk_x, chunk_z, self.seed, self.parent.block)
        self.chunks[(chunk_x, chunk_z)] = chunk
        self.prepareList.append(chunk)

    def checkPrepare(self):
        self.prepareCnt += 1
        if self.prepareCnt < self.prepareWait or not self.prepareList:
            return

        player_x_chunk = int(self.parent.player.x() // 16)
        player_z_chunk = int(self.parent.player.z() // 16)

        self.prepareCnt = 0
        chunk = self.prepareList.pop(0)
        chunk.prepareChunk()

    def chunksUpdate(self):
        player_x_chunk = int(self.parent.player.x() // 16)
        player_z_chunk = int(self.parent.player.z() // 16)

        for x in range(player_x_chunk - self.parent.renderDistance, player_x_chunk + self.parent.renderDistance):
            for z in range(player_z_chunk - self.parent.renderDistance, player_z_chunk + self.parent.renderDistance):
                if (x, z) not in self.chunks:
                    self.prepareChunk(x, z)

        copy = self.chunks.copy()
        for position, value in copy.items():
            chunk_x, chunk_z = position
            if (
                    player_x_chunk - self.parent.renderDistance > chunk_x or chunk_x > player_x_chunk + self.parent.renderDistance or
                    player_z_chunk - self.parent.renderDistance > chunk_z or chunk_z > player_z_chunk + self.parent.renderDistance):
                value.erase()
                self.chunks.pop(position)

        del copy

    def getBlockAABB(self, x, y, z):
        player_x_chunk = int(self.parent.player.x() // 16)
        player_z_chunk = int(self.parent.player.z() // 16)
        if (int(player_x_chunk), int(player_z_chunk)) not in self.chunks or y < 0 or y > 256:
            aabb = opengl_main_cpp.AABB(0, 0, 0, 0, 0, 0)
            aabb.setExist(False)
            return aabb
        chunk = self.chunks[(int(player_x_chunk), int(player_z_chunk))]
        return chunk.getBlockAABB(x, y, z)

    def removeBlock(self, x, y, z):
        pass

    def getBlock(self, x, y, z):
        player_x_chunk = int(self.parent.player.x() // 16)
        player_z_chunk = int(self.parent.player.z() // 16)
        if (int(player_x_chunk), int(player_z_chunk)) not in self.chunks or y < 0 or y > 256:
            return -1
        chunk = self.chunks[(int(player_x_chunk), int(player_z_chunk))]
        # print(x, y, z)
        return -1  # chunk.getBlock(int(x), int(y), int(z))

    def update(self):
        self.prepareWait = 9
        self.chunksUpdate()
        self.checkPrepare()

        glEnable(GL_TEXTURE_2D)
        glBindTexture(GL_TEXTURE_2D, int(self.parent.texture["world"]))
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
