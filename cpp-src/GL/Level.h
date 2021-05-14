#pragma once

#include <glm/gtc/noise.hpp>
#include <fstream>
#include "Chunk.h"

class Level
{
public:
    int random(int min, int max)
    {
        static bool first = true;
        if (first)
        {
            srand( this->seed ); //seeding for the first time only!
            first = false;
        }
        return min + rand() % (( max + 1 ) - min);
    }

    int seed;
    std::map<std::pair<int, std::pair<int, int>>, Block> blocks;

    Level()
    {
    }

    Level(int seed)
    {
        this->seed = seed;
    }

    void erase()
    {
        _gl_engine_info("Level->erase", "Deleting level...");
        std::map<std::pair<int, std::pair<int, int>>, Block>::iterator itr = blocks.begin();
        //_gl_engine_info("test", itr->second.id);
        printf("size:%d\n", ((&blocks))->size());
        for (; itr != blocks.end(); itr++)
        {
            //printf("Deleting block (MEM_LOCATION:%d) -> ", &(itr->second));
            (&itr->second)->erase();
            printf("%s\n", itr->second.id);
            blocks.erase(itr->first);
        }
    }

    float simplex(float x, float y)
    {
        return glm::simplex(glm::vec2{x, y});
    }

    float sumOcatave(int num_iterations, float x, float y, float persistence, float scale, float low, float high)
    {
        float maxAmp = 0;
        float amp = 1;
        float freq = scale;
        float noise = 0;

        //add successively smaller, higher-frequency terms
        for(int i = 0; i < num_iterations; ++i)
        {
            noise += simplex(x * freq, y * freq) * amp;
            maxAmp += amp;
            amp *= persistence;
            freq *= 2;
        }

        //take the average value of the iterations
        noise /= maxAmp;

        //normalize the result
        noise = noise * (high - low) / 2 + (high + low) / 2;

        return noise;
    }

    Block getBlock(int x, int y, int z)
    {
        if (blocks.count(std::make_pair(x, std::make_pair(y, z))))
        {
            return blocks[std::make_pair(x, std::make_pair(y, z))];
        }

        return Block();
    }

    void setBlock(Block block, bool blockCheck=true)
    {
        int x = block.x;
        int y = block.y;
        int z = block.z;

        if (y < 0 || y > 256) return;

        if (!blocks.count(std::make_pair(x, std::make_pair(y, z))) || !blockCheck)
        {
          blocks[std::make_pair(x, std::make_pair(y, z))] = block;
        }
    }

    void removeBlock(int x, int y, int z)
    {
        if (y < 0 || y > 256) return;

        if (blocks.count(std::make_pair(x, std::make_pair(y, z))))
        {
            blocks.erase(std::make_pair(x, std::make_pair(y, z)));
        }
    }

    void generateTree(int x, int y, int z)
    {
        int treeHeight = random(5, 7);
        for (int i = y; i < y + treeHeight; i++)
        {
            setBlock(Block("log_oak", x, i, z));
        }
        for (int i = x + -2; i < x + 3; i++)
        {
            for (int j = z + -2; j < z + 3; j++)
            {
                for (int k = y + treeHeight - 2; k < y + treeHeight; k++)
                {
                    setBlock(Block("leaves_oak", i, k, j));
                }
            }
        }
        for (int i = treeHeight; i < treeHeight + 1; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                for (int k = -1; k < 2; k++)
                {
                    setBlock(Block("leaves_oak", x + j, y + i, z + k));
                }
            }
        }
        int cl = 2;
        for (int i = treeHeight + 1; i < treeHeight + 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                for (int k = -1; k < 2; k++)
                {
                    if (cl % 2 != 0)
                    {
                        setBlock(Block("leaves_oak", x + j, y + i, z + k));
                    }
                    cl++;
                }
            }
        }
        setBlock(Block("leaves_oak", x, y + treeHeight + 1, z));
    }

    void generateLevel(int x_from, int z_from, int x_to, int z_to, int height)
    {
        for (int x = x_from; x <= x_to; x++)
        {
            for (int z = z_from; z <= z_to; z++)
            {
                int y = height + this->sumOcatave(16, (float) x, (float) z, .5f, .007f, -24.0f, 32.0f);

                if (y < 0) y = 0;
                if (y > 255) y = 255;
                bool wasSand = false;

                setBlock(Block("bedrock", x, 0, z));
                if (y < height - 1)
                {
                    setBlock(Block("sand", x, y, z));
                    wasSand = true;
                }
                else
                {
                    setBlock(Block("grass", x, y, z));
                }

                if (!wasSand && random(20, 100) == random(20, 100))
                {
                    this->generateTree(x, y, z);
                }

                for (int i = 1; i < y; i++)
                {
                    if (!wasSand)
                    {
                        if (i > y - random(5, 10))
                        {
                            setBlock(Block("dirt", x, i, z));
                        }
                        else
                        {
                            setBlock(Block("stone", x, i, z));
                        }
                    }
                    else
                    {
                        if (i > y - random(5, 10))
                        {
                            setBlock(Block("gravel", x, i, z));
                        }
                        else
                        {
                            setBlock(Block("stone", x, i, z));
                        }
                    }

                }
                if (y < height - 3)
                {
                    for (int i = y; i < height - 3; i++)
                    {
                        setBlock(Block("water", x, i, z));
                    }
                }

            }
        }
    }

};
