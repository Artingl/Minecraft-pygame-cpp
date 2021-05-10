#pragma once

#include <boost/python/dict.hpp>
#include "GL.h"
#include "../debug.h"

class World
{
private:
    std::map<std::pair<int, std::pair<int, int>>, Chunk*> chunks;
    std::vector<Chunk*> prepare;
    dict textures;
    Level level;
    int texturesId;
    int renderDistance;
    int seed;

    void generateChunk(int x, int y, int z)
    {
        Chunk *chunk = new Chunk(x, y, z, &level, this->textures);
        this->chunks[std::make_pair(x, std::make_pair(y, z))] = chunk;
        chunk->update();
    }

    void generateChunks(int x, int y, int z)
    {
        int player_x_chunk = (int) (x / chunk_width);
        int player_y_chunk = (int) (y / chunk_height);
        int player_z_chunk = (int) (z / chunk_depth);

        for (int chunk_x = player_x_chunk - this->renderDistance; chunk_x <= player_x_chunk + this->renderDistance; chunk_x++)
        {
            for (int chunk_y = 0; chunk_y <= 16; chunk_y++)
            {
                for (int chunk_z = player_z_chunk - this->renderDistance; chunk_z <= player_z_chunk + this->renderDistance; chunk_z++)
                {
                    if (!this->chunks.count(std::make_pair(chunk_x, std::make_pair(chunk_y, chunk_z))))
                    {
                        generateChunk(chunk_x, chunk_y, chunk_z);
                    }
                }
            }
        }
    }

public:
    World(int renderDistance, int texturesId, dict textures)
    {
        this->renderDistance = renderDistance;
        this->texturesId = texturesId;
        this->textures = textures;
        this->seed = 4334232;
        this->level = Level(this->seed);

        _gl_engine_info("World->constructor", "Generating world...");
        this->level.generateLevel(-128, -128, 128, 128, 64);
        _gl_engine_info("World->constructor", "Generating chunks...");


        this->generateChunks(0, 0, 0);
    }

    int getBlockExist(int x, int y, int z)
    {
        if (level.getBlock(x, y, z).exist)
        {
            return 1;
        }
        return -1;
    }

    void removeBlock(int x, int y, int z)
    {
        int chunk_x = (int) (abs(x) / 16);
        int chunk_y = (int) (abs(y) / 16);
        int chunk_z = (int) (abs(z) / 16);

        if (x < 0)
        {
            chunk_x = -chunk_x;
        }
        if (y < 0)
        {
            chunk_y = -chunk_y;
        }
        if (z < 0)
        {
            chunk_z = -chunk_z;
        }

        if (level.getBlock(x, y, z).exist)
        {
            if (this->chunks.count(std::make_pair(chunk_x, std::make_pair(chunk_y, chunk_z))))
            {
                level.removeBlock(x, y, z);
                this->chunks[std::make_pair(chunk_x, std::make_pair(chunk_y, chunk_z))]->update();
            }
        }
    }

    void update(float player_x, float player_y, float player_z)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, this->texturesId);

        for (const auto& [position, chunk] : chunks)
        {
            int cx = chunk->x * 16;
            int cy = chunk->y * 16;
            int cz = chunk->z * 16;

            //if (player_x_chunk - (this->renderDistance - 1) > chunk->x || chunk->x > player_x_chunk + (this->renderDistance - 1) ||
            //    player_z_chunk - (this->renderDistance - 1) > chunk->z || chunk->z > player_z_chunk + (this->renderDistance - 1))
            //{
            //    continue;
            //}

            glPushMatrix();
            glTranslatef(cx, cy, cz);
            chunk->render();
            glPopMatrix();
        }
        glBindTexture(GL_TEXTURE_2D, 0);

    }

};
