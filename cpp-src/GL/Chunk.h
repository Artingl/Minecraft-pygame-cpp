#pragma once

#include "GL.h"
#include <GL/glew.h>

#define chunk_width 16
#define chunk_height 256
#define chunk_depth 16

#define world_height 64

class Chunk
{
public:
    int x;
    int z;

    int chunkList;
    int blocks[chunk_width + 1][chunk_height + 1][chunk_depth + 1];

    Chunk(int x, int z)
    {
        this->x = x;
        this->z = z;

        this->chunkList = 0;

        for (int block_x = 0; block_x <= chunk_width; ++block_x)
        {
            for (int block_y = 0; block_y <= chunk_height; ++block_y)
            {
                for (int block_z = 0; block_z <= chunk_depth; ++block_z)
                {
                    this->blocks[block_x][block_y][block_z] = -1;
                }
            }
        }

        create();
    }

    void create()
    {
        if(chunkList) glDeleteLists(chunkList, 1);
        chunkList = glGenLists(1);
        glNewList(chunkList, GL_COMPILE);
        glPushMatrix();

        glColor3f(1, 1, 1);

        GL *qb = new GL(65536);
        qb->begin();

        int block_r = 255;
        int block_g = 255;
        int block_b = 255;

        for (int block_x = 0; block_x <= chunk_width; ++block_x)
        {
            for (int block_y = 0; block_y <= world_height; ++block_y)
            {
                for (int block_z = 0; block_z <= chunk_depth; ++block_z)
                {
                    Texture textureClass = Texture::getTexture("grass_block");

                    this->blocks[block_x][block_y][block_z] = 10;

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
    }

    bool checkBlockSide(int side, int x, int y, int z)
    {
        // TODO

        if ((x == 16 || y == world_height || z == 16) || (x == 0 || y == 0 || z == 0))
        {
            return true;
        }

        return false;
    }

    void render()
    {
        glCallList(chunkList);
    }

};
