#pragma once

#include <bits/stdc++.h>
#include <filesystem>
#include <boost/python/dict.hpp>
#include "GL.h"
#include "../debug.h"

class World
{
public:
    std::map<std::pair<int, std::pair<int, int>>, Chunk*> chunks;
    std::vector<Chunk*> prepare;
    dict textures;
    Level level;
    char* worldName;
    int texturesId;
    int renderDistance;
    int seed;

    void updateChunkByPos(int x, int y, int z)
    {
        for (int chunk_x = (x / 16) - 1; chunk_x < (x / 16) + 1; chunk_x++)
        {
            for (int chunk_y = (y / 16) - 1; chunk_y < (y / 16) + 1; chunk_y++)
            {
                for (int chunk_z = (z / 16) - 1; chunk_z < (z / 16) + 1; chunk_z++)
                {
                    if (this->chunks.count(std::make_pair(chunk_x, std::make_pair(chunk_y, chunk_z))))
                    {
                        this->chunks[std::make_pair(chunk_x, std::make_pair(chunk_y, chunk_z))]->update();
                        this->chunks[std::make_pair(chunk_x, std::make_pair(chunk_y, chunk_z))]->updated = true;
                    }
                }
            }

        }


    }

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

        for (int chunk_x = player_x_chunk - 8; chunk_x <= player_x_chunk + 8; chunk_x++)
        {
            for (int chunk_y = 0; chunk_y <= 16; chunk_y++)
            {
                for (int chunk_z = player_z_chunk - 8; chunk_z <= player_z_chunk + 8; chunk_z++)
                {
                    if (!this->chunks.count(std::make_pair(chunk_x, std::make_pair(chunk_y, chunk_z))))
                    {
                        generateChunk(chunk_x, chunk_y, chunk_z);
                    }
                }
            }
        }
    }

    void deleteWorld()
    {
        //for (const auto& [position, chunk] : chunks)
        //{
        //    chunk->erase();
        //    chunks.erase(position);
        //}
        //for (const auto& [position, block] : level.blocks)
        //{
        //    level.blocks.erase(position);
        //}
        level.erase();
        this->level = Level(this->seed);

        //chunks.clear();
    }

    World(int renderDistance, int texturesId, dict textures)
    {
        this->renderDistance = renderDistance;
        this->texturesId = texturesId;
        this->textures = textures;
        this->seed = 4334232;
        this->level = Level(this->seed);

        //_gl_engine_info("World->constructor", "Generating world...");
        //this->level.generateLevel(-128, -128, 128, 128, 64);
        //_gl_engine_info("World->constructor", "Generating chunks...");
        //this->generateChunks(0, 0, 0);
    }

    bool isUnderWater(int x, int y, int z)
    {
        if (level.getBlock(x, y, z).id == "water")
        {
            return true;
        }
        return false;
    }

    int getBlockExist(int x, int y, int z)
    {
        if (level.getBlock(x, y, z).exist && !level.getBlock(x, y, z).liquid)
        {
            if (level.getBlock(x, y, z).liquid) return 2;
            return 1;
        }
        return -1;
    }

    void removeBlock(int x, int y, int z)
    {
        if (level.getBlock(x, y, z).exist && level.getBlock(x, y, z).id != "bedrock")
        {
            level.removeBlock(x, y, z);
            updateChunkByPos(x, y, z);
        }
    }

    void setBlock(int x, int y, int z, char* name)
    {
        level.setBlock(Block(name, x, y, z));
        updateChunkByPos(x, y, z);
    }

    void setBlockWithoutBlockCheck(int x, int y, int z, char* name)
    {
        level.setBlock(Block(name, x, y, z), false);
        updateChunkByPos(x, y, z);
    }

    list hitTest(float x, float y, float z, float dx, float dy, float dz, int dist, int m)
    {
        //
    }

    char* getBlockName(int x, int y, int z)
    {
        return (char*)this->level.getBlock(x, y, z).id;
    }

    void setLevelName(char* name)
    {
        this->worldName = name;
    }

    void saveWorld()
    {
        std::ofstream world_config;
        world_config.open ("saves/" + std::string(this->worldName) + "/world.data");
        world_config << "name=" << this->worldName << "\n" << "lastopen=" << "idk" << "\nmode=" << "0,1" << "\nseed=" << this->seed;
        world_config.close();

        int i = 0;
        for (const auto& [position, chunk] : chunks)
        {
            if (!chunk->updated) continue;
            chunk->updated = false;

            std::string blocks = "";
            std::string position_str(std::to_string(i));
            std::string c_x(std::to_string(chunk->x));
            std::string c_y(std::to_string(chunk->y));
            std::string c_z(std::to_string(chunk->z));

            std::ofstream chunk_data;
            chunk_data.open ("saves/" + std::string(this->worldName) + "/chunks/chunk" + position_str + ".data");
            chunk_data << c_x << " " << c_y << " " << c_z << "\n";
            int xcw = chunk->x * chunk_width;
            int ycw = chunk->y * chunk_height;
            int zcw = chunk->z * chunk_depth;
            for (int block_x = xcw; block_x <= chunk_width + xcw; ++block_x)
            {
                for (int block_y = ycw; block_y <= chunk_height + ycw; ++block_y)
                {
                    for (int block_z = zcw; block_z <= chunk_depth + zcw; ++block_z)
                    {
                        Block level_block = this->level.getBlock(block_x, block_y, block_z);
                        const char* id;
                        if (this->getBlockExist(block_x, block_y, block_z) == -1)
                        {
                            id = "air";
                        }
                        else
                        {
                            id = level_block.id;
                            if (!id || id == NULL || id == "") id = "invalid";
                        }
                        blocks += std::string(id) + ";";
                    }
                }
            }
            chunk_data << blocks << std::endl;
            chunk_data.close();
            i++;
        }
    }

    void loadWorld(boost::python::list& chunk_files)
    {
        boost::python::ssize_t n = boost::python::len(chunk_files);
        for(boost::python::ssize_t i=0;i<n;i++) {
            boost::python::extract<char*> elem (chunk_files[i]);
            std::string chunk(this->readFile("saves/" + std::string(this->worldName) + "/chunks/" + std::string((char*)elem)));
            if (chunk == "") continue;
            std::vector <std::string> splt = this->split(chunk, '\n');
            std::string positions = splt[0];
            std::string blocks = splt[1];
            std::vector <std::string> posss = this->split(blocks, ';');

            int c_x = std::stoi(this->split(positions, ' ')[0]) * chunk_width;
            int c_y = std::stoi(this->split(positions, ' ')[1]) * chunk_height;
            int c_z = std::stoi(this->split(positions, ' ')[2]) * chunk_depth;

            printf("Chunk: %s\n", (char*)elem);

            int j = 0;
            for (int x = 0; x <= 16; x++)
            {
                for (int y = 0; y <= 16; y++)
                {
                    for (int z = 0; z <= 16; z++)
                    {
                        if (j > posss.size()) break;
                        const char* id = posss[j].c_str();
                        if (!id || id == NULL || id == "") continue;
                        removeBlock(x + c_x, y + c_y, z + c_z);
                        if (id != "air") level.setBlock(Block(id, x + c_x, y + c_y, z + c_z));
                        //else printf("AIR!\n");
                        j++;
                    }
                    if (j > posss.size()) break;
                    j++;
                }
                if (j > posss.size()) break;
                j++;
            }
        }
    }

    void update(float player_x, float player_y, float player_z)
    {
        glEnable(GL_ALPHA_TEST);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, this->texturesId);

        int player_x_chunk = (int) (player_x / chunk_width);
        int player_y_chunk = (int) (player_y / chunk_height);
        int player_z_chunk = (int) (player_z / chunk_depth);

        for (const auto& [position, chunk] : chunks)
        {
            int cx = chunk->x * 16;
            int cy = chunk->y * 16;
            int cz = chunk->z * 16;

            if (player_x_chunk - (this->renderDistance + 1) > chunk->x || chunk->x > player_x_chunk + (this->renderDistance + 1) ||
                player_y_chunk - (this->renderDistance + 1) > chunk->y || chunk->y > player_y_chunk + (this->renderDistance + 1) ||
                player_z_chunk - (this->renderDistance + 1) > chunk->z || chunk->z > player_z_chunk + (this->renderDistance + 1))
            {
                continue;
            }

            glPushMatrix();
            glTranslatef(cx, cy, cz);
            chunk->render();
            glPopMatrix();
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);

    }

    std::string readFile(std::string path)
    {
        std::stringstream str;
        std::ifstream stream(path);
        if(stream.is_open())
        {
            while(stream.peek() != EOF)
            {
                str << (char) stream.get();
            }
            stream.close();
            return str.str();
        }
    }

    std::vector <std::string> split(std::string str, char delim)
    {

        std::vector <std::string> tokens;
        std::stringstream check1(str);
        std::string intermediate;


        while(getline(check1, intermediate, delim))
        {
            tokens.push_back(intermediate);
        }

        return tokens;
    }

};
