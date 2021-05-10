import threading
import opengl_main_cpp
import settings
from OpenGL.GL import *
# TODO: super flat world, world 256x256x256 etc

class Chunks:
    def __init__(self, parent):
        self.worldGen_chunkPosition = [0, 0]
        self.parent = parent
        self.chunks = {}
        self.prepareList = []
        self.prepareCnt = 0
        self.prepareWait = 10
        self.seed = 3424

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
        if not opengl_main_cpp.checkChunksContains(int(player_x_chunk), int(player_z_chunk)) or \
                y < 0 or y > 256:
            return -1
        return opengl_main_cpp.callChunk(int(player_x_chunk), int(player_z_chunk), x, y, z)

    def update(self):
        player_x_chunk = int(self.parent.player.x() // 16)
        player_z_chunk = int(self.parent.player.z() // 16)

        opengl_main_cpp.updateChunks(
            int(self.parent.texture["world"]),
            player_x_chunk, player_z_chunk,
            self.seed,
            self.parent.block,
            20
        )

    def generateChunks(self):
        if self.worldGen_chunkPosition[1] <= settings.RENDER_DISTANCE * 2:
            self.prepareWait = 0
            chunk_x, chunk_z = int(self.worldGen_chunkPosition[0]) - settings.RENDER_DISTANCE, \
                               int(self.worldGen_chunkPosition[1]) - settings.RENDER_DISTANCE
            opengl_main_cpp.generateChunk(chunk_x, chunk_z, self.seed, self.parent.block, True)

            self.worldGen_chunkPosition[0] += 1
            if self.worldGen_chunkPosition[0] >= settings.RENDER_DISTANCE * 2:
                self.worldGen_chunkPosition[0] = 0
                self.worldGen_chunkPosition[1] += 1
