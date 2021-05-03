#pragma once

#include <boost/python/dict.hpp>
#include "GL.h"
#include "../AABB.h"
#include "../noise.h"
#include <GL/glew.h>

#define chunk_width 16
#define chunk_height 256
#define chunk_depth 16

#define world_height 64

using namespace boost::python;

class Chunk
{
private:
    int random(int min, int max)
    {
        static bool first = true;
        if (first)
        {
            srand( time(NULL) ); //seeding for the first time only!
            first = false;
        }
        return min + rand() % (( max + 1 ) - min);
    }

public:
    GL *qb;

    int x;
    int z;

    bool prepared = false;
    int chunkList;

    bool chunks[32][32];
    Texture blocks[chunk_width + 1][chunk_height + 1][chunk_depth + 1];
    dict textures;

    PerlinNoise *HeightGenerator;



    Chunk(int x, int z, int seed, dict textures)
    {
        this->x = x;
        this->z = z;
        this->textures = textures;

        this->chunkList = 0;
        HeightGenerator = new PerlinNoise(0.5, 0.5, 30, 8, seed);
    }

    float getNoiseNumber(int x, int z)
    {
        float value = HeightGenerator->GetHeight(x / 16.0f, z / 16.0f);
        value = (value + 1) / 2;
        value *= 2;

        return value;
    }

    AABB getBlockAABB(int x, int y, int z)
    {
        AABB aabb(x - 0.5f, y - 0.5f, z - 0.5f, x + 0.5f, y + 0.5f, z + 0.5f);

        if ((y < 0 && y > 255) || (x > chunk_width && x < 0) || (z > chunk_depth && z < 0))
        {
            return aabb;
        }

        if (this->blocks[x][y][z].exist != -1)
        {
            aabb.exist = true;
        }

        return aabb;
    }

    int getX()
    {
        return this->x;
    }

    int getZ()
    {
        return this->z;
    }

    bool getPrepared()
    {
        return prepared;
    }

    void setChunk(int mx, int mz)
    {
        this->chunks[(int ) mx][(int ) mz] = true;
    }

    void setBlock(int x, int y, int z, Texture texture)
    {
        if (y < 0 || y > 255 || x > chunk_width || x < 0 || z > chunk_depth || z < 0)
        {
            return;
        }

        if (this->blocks[x][y][z].exist != -1)
        {
            return;
        }

        this->blocks[x][y][z] = texture;
        this->blocks[x][y][z].exist = 1;
    }

    int getBlock(int x, int y, int z)
    {
        if (y < 0)   y = 0;
        if (y > 255) y = 255;

        return this->blocks[x][y][z].exist;
    }

    void removeBlock(int x, int y, int z)
    {
        if (y < 0)   y = 0;
        if (y > 255) y = 255;
        this->blocks[x][y][z].exist = -1;
    }

    void prepareChunk()
    {
        for (int block_x = 0; block_x <= chunk_width; ++block_x)
        {
            for (int block_z = 0; block_z <= chunk_depth; ++block_z)
            {
                int block_y = world_height + getNoiseNumber(block_x + (this->x * chunk_width), block_z + (this->z * chunk_depth));

                setBlock(block_x, 0, block_z, Texture::getTexture("bedrock", this->textures));
                setBlock(block_x, block_y, block_z, Texture::getTexture("grass", this->textures));

                if (random(40, 100) == 50)
                {
                    generateTree(block_x, block_y + 1, block_z);
                }

                for (int i = 1; i < block_y; ++i)
                {
                    if (i > block_y - random(5, 10))
                    {
                        setBlock(block_x, i, block_z, Texture::getTexture("dirt", this->textures));
                    }
                    else
                    {
                        setBlock(block_x, i, block_z, Texture::getTexture("stone", this->textures));
                    }
                }
            }
        }

        update();
    }

    void erase()
    {
        delete qb;
        if(chunkList) glDeleteLists(chunkList, 1);
    }

    void update()
    {
        if(chunkList) glDeleteLists(chunkList, 1);
        if (qb) delete qb;
        chunkList = glGenLists(1);
        glNewList(chunkList, GL_COMPILE);
        glPushMatrix();

        glColor3f(1, 1, 1);

        qb = new GL(65536);
        qb->begin();

        int block_r = 255;
        int block_g = 255;
        int block_b = 255;

        for (int block_x = 0; block_x <= chunk_width; ++block_x)
        {
            for (int block_y = 0; block_y <= chunk_height; ++block_y)
            {
                for (int block_z = 0; block_z <= chunk_depth; ++block_z)
                {
                    if (this->blocks[block_x][block_y][block_z].exist == -1) continue;
                    Texture textureClass = this->blocks[block_x][block_y][block_z];

                    if (checkBlockSide(0, block_x, block_y, block_z))
                    {
                        qb->setColori(block_r, block_g, block_b);

                        qb->setTexCoord(textureClass.getTopX() + 0.0f, textureClass.getTopY() + textureClass.addTextCoord);
                        qb->addVertex(block_x, 1 + block_y, block_z);
                        qb->setTexCoord(textureClass.getTopX() + 0.0f, textureClass.getTopY() + 0.0f);
                        qb->addVertex(1 + block_x, 1 + block_y, block_z);
                        qb->setTexCoord(textureClass.getTopX() + textureClass.addTextCoord, textureClass.getTopY() + 0.0f);
                        qb->addVertex(1 + block_x, 1 + block_y, 1 + block_z);
                        qb->setTexCoord(textureClass.getTopX() + textureClass.addTextCoord, textureClass.getTopY() + textureClass.addTextCoord);
                        qb->addVertex(block_x, 1 + block_y, 1 + block_z);
                    }

                    if (checkBlockSide(1, block_x, block_y, block_z))
                    {
                        qb->setColori(block_r, block_g, block_b);
                        qb->setTexCoord(textureClass.getBottomX() + 0.0f, textureClass.getBottomY() + textureClass.addTextCoord);
                        qb->addVertex(block_x, block_y, 1 + block_z);
                        qb->setTexCoord(textureClass.getBottomX() + 0.0f, textureClass.getBottomY() + 0.0f);
                        qb->addVertex(1 + block_x, block_y, 1 + block_z);
                        qb->setTexCoord(textureClass.getBottomX() + textureClass.addTextCoord, textureClass.getBottomY() + 0.0f);
                        qb->addVertex(1 + block_x, block_y, block_z);
                        qb->setTexCoord(textureClass.getBottomX() + textureClass.addTextCoord, textureClass.getBottomY() + textureClass.addTextCoord);
                        qb->addVertex(block_x, block_y, block_z);
                    }

                    if (checkBlockSide(2, block_x, block_y, block_z))
                    {
                        qb->setColori(block_r - 40, block_g - 40, block_b - 40);

                        qb->setTexCoord(textureClass.getSideX() + textureClass.addTextCoord, textureClass.getSideY() + textureClass.addTextCoord);
                        qb->addVertex(block_x, block_y, block_z);
                        qb->setTexCoord(textureClass.getSideX() + 0.0f, textureClass.getSideY() + textureClass.addTextCoord);
                        qb->addVertex(1 + block_x, block_y, block_z);
                        qb->setTexCoord(textureClass.getSideX() + 0.0f, textureClass.getSideY() + 0.0f);
                        qb->addVertex(1 + block_x, 1 + block_y, block_z);
                        qb->setTexCoord(textureClass.getSideX() + textureClass.addTextCoord, textureClass.getSideY() + 0.0f);
                        qb->addVertex(block_x, 1 + block_y, block_z);
                    }

                    if (checkBlockSide(3, block_x, block_y, block_z))
                    {
                        qb->setColori(block_r - 80, block_g - 80, block_b - 80);

                        qb->setTexCoord(textureClass.getSideX() + textureClass.addTextCoord, textureClass.getSideY() + textureClass.addTextCoord);
                        qb->addVertex(1 + block_x, block_y, block_z);
                        qb->setTexCoord(textureClass.getSideX() + 0.0f, textureClass.getSideY() + textureClass.addTextCoord);
                        qb->addVertex(1 + block_x, block_y, 1 + block_z);
                        qb->setTexCoord(textureClass.getSideX() + 0.0f, textureClass.getSideY() + 0.0f);
                        qb->addVertex(1 + block_x, 1 + block_y, 1 + block_z);
                        qb->setTexCoord(textureClass.getSideX() + textureClass.addTextCoord, textureClass.getSideY() + 0.0f);
                        qb->addVertex(1 + block_x, 1 + block_y, block_z);
                    }

                    if (checkBlockSide(4, block_x, block_y, block_z))
                    {
                        qb->setColori(block_r - 40, block_g - 40, block_b - 40);

                        qb->setTexCoord(textureClass.getSideX() + textureClass.addTextCoord, textureClass.getSideY() + textureClass.addTextCoord);
                        qb->addVertex(1 + block_x, block_y, 1 + block_z);
                        qb->setTexCoord(textureClass.getSideX() + 0.0f, textureClass.getSideY() + textureClass.addTextCoord);
                        qb->addVertex(block_x, block_y, 1 + block_z);
                        qb->setTexCoord(textureClass.getSideX() + 0.0f, textureClass.getSideY() + 0.0f);
                        qb->addVertex(block_x, 1 + block_y, 1 + block_z);
                        qb->setTexCoord(textureClass.getSideX() + textureClass.addTextCoord, textureClass.getSideY() + 0.0f);
                        qb->addVertex(1 + block_x, 1 + block_y, 1 + block_z);
                    }

                    if (checkBlockSide(5, block_x, block_y, block_z))
                    {
                        qb->setColori(block_r - 80, block_g - 80, block_b - 80);

                        qb->setTexCoord(textureClass.getSideX() + textureClass.addTextCoord, textureClass.getSideY() + textureClass.addTextCoord);
                        qb->addVertex(block_x, block_y, 1 + block_z);
                        qb->setTexCoord(textureClass.getSideX() + 0.0f, textureClass.getSideY() + textureClass.addTextCoord);
                        qb->addVertex(block_x, block_y, block_z);
                        qb->setTexCoord(textureClass.getSideX() + 0.0f, textureClass.getSideY() + 0.0f);
                        qb->addVertex(block_x, 1 + block_y, block_z);
                        qb->setTexCoord(textureClass.getSideX() + textureClass.addTextCoord, textureClass.getSideY() + 0.0f);
                        qb->addVertex(block_x, 1 + block_y, 1 + block_z);
                    }

                }
            }
        }

        qb->render();
        glPopMatrix();
        glEndList();

        prepared = true;
    }

    bool checkBlockSide(int side, int x, int y, int z)
    {
        if (x == 0 || x == chunk_width || z == 0 || z == chunk_depth)
        {
            return true;
        }

        switch (side)
        {
            case 0:
                return this->blocks[x][y + 1][z].exist == -1;
            case 1:
                return this->blocks[x][y - 1][z].exist == -1;
            case 2:
                return this->blocks[x][y][z - 1].exist == -1;
            case 3:
                return this->blocks[x + 1][y][z].exist == -1;
            case 4:
                return this->blocks[x][y][z + 1].exist == -1;
            case 5:
                return this->blocks[x - 1][y][z].exist == -1;
        }

        return false;
    }

    void render()
    {
        glCallList(chunkList);
    }


    //
    void generateTree(int x, int y, int z)
    {
        int treeHeight = random(5, 7);
        for (int i = y; i < y + treeHeight; i++)
        {
            setBlock(x, i, z, Texture::getTexture("log_oak", this->textures));
        }
        for (int i = x + -2; i < x + 3; i++)
        {
            for (int j = z + -2; i < z + 3; i++)
            {
                for (int k = y + treeHeight - 2; i < y + treeHeight; i++)
                {
                    setBlock(x, i, z, Texture::getTexture("leaves_oak", this->textures));
                }
            }
        }

        /*
        treeHeight = random.randint(5, 7)

        for i in range(y, y + treeHeight):
            self.add((x, i, z), 'log_oak')
        for i in range(x + -2, x + 3):
            for j in range(z + -2, z + 3):
                for k in range(y + treeHeight - 2, y + treeHeight):
                    self.add((i, k, j), 'leaves_oak')
        for i in range(treeHeight, treeHeight + 1):
            for j in range(-1, 2):
                for k in range(-1, 2):
                    self.add((x + j, y + i, z + k), 'leaves_oak')
        cl = 2
        for i in range(treeHeight + 1, treeHeight + 2):
            for j in range(-1, 2):
                for k in range(-1, 2):
                    if cl % 2 != 0:
                        self.add((x + j, y + i, z + k), 'leaves_oak')
                    cl += 1
        self.add((x, y + treeHeight + 1, z), 'leaves_oak')
         */
    }


};
