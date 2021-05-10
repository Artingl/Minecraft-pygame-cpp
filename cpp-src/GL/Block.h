#pragma once

class Block
{
public:
    const char *id;
    int x;
    int y;
    int z;

    bool liquid = false;
    bool exist = false;

    Block() {}

    Block(const char *id, int x, int y, int z)
    {
        this->id = id;
        this->x = x;
        this->y = y;
        this->z = z;
        this->exist = true;

        if (id == "water" || id == "lava") this->liquid = true;
    }

    int getXByChunk(int chunk_x)
    {
        return x - (chunk_x * 16);
    }

    int getZByChunk(int chunk_z)
    {
        return z - (chunk_z * 16);
    }

};
