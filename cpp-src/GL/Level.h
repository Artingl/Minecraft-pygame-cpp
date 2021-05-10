#pragma once

#include <glm/gtc/noise.hpp>

class Level
{
public:
    int seed;

    std::map<std::pair<int, std::pair<int, int>>, Block> blocks;

    Level()
    {
    }

    Level(int seed)
    {
        this->seed = seed;
    }

    float noise(float x, float y)
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
            noise += glm::simplex(glm::vec2 {x * freq, y * freq} ) * amp;
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

    void setBlock(Block block)
    {
        int x = block.x;
        int y = block.y;
        int z = block.z;

        if (!blocks.count(std::make_pair(x, std::make_pair(y, z))))
        {
            blocks[std::make_pair(x, std::make_pair(y, z))] = block;
        }
    }

    void removeBlock(int x, int y, int z)
    {
        if (blocks.count(std::make_pair(x, std::make_pair(y, z))))
        {
            blocks.erase(std::make_pair(x, std::make_pair(y, z)));
        }
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

                setBlock(Block("bedrock", x, 0, z));
                setBlock(Block("grass", x, y, z));

                for (int i = 1; i < y; i++)
                {
                    setBlock(Block("dirt", x, i, z));
                }
            }
        }
    }

};
