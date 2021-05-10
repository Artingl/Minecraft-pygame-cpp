#pragma once

#include <boost/python/dict.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GL.h"
#include "../debug.h"
#include "Block.h"

using namespace boost::python;

class BlockRender {
public:
    int x;
    int y;
    int z;

    float top_x;      float top_y;
    float bottom_x;   float bottom_y;
    float side_x;     float side_y;

    void createTexture()
    {
        top_x = 0; top_y = 0;
        for (int i = 0; i < top; i++)
        {
            top_x += addTextCoord;
            if (top_x > 4096)
            {
                top_x = 0;
                top_y += addTextCoord;
            }
        }

        bottom_x = 0; bottom_y = 0;
        for (int i = 0; i < bottom; i++)
        {
            bottom_x += addTextCoord;
            if (bottom_x > 4096)
            {
                bottom_x = 0;
                bottom_y += addTextCoord;
            }
        }

        side_x = 0; side_y = 0;
        for (int i = 0; i < side; i++)
        {
            side_x += addTextCoord;
            if (side_x > 4096)
            {
                side_x = 0;
                side_y += addTextCoord;
            }
        }
    }

    int top = 0;
    int bottom = 0;
    int side = 0;
    int exist = -1;

    float addTextCoord = 1.0f / 100.0f;

    BlockRender () {}

    BlockRender (int id_top, int id_bottom, int id_side, int x, int y, int z)
    {
        this->top = id_top;
        this->bottom = id_bottom;
        this->side = id_side;

        this->x = x;
        this->y = y;
        this->z = z;
        this->exist = 1;

        createTexture();
    }

    static BlockRender getBlock(dict textures, Block block)
    {
        if (!textures.contains(block.id))
        {
            extract<int> texture_top(textures["invalid"][0]);
            extract<int> texture_bottom(textures["invalid"][1]);
            extract<int> texture_side(textures["invalid"][2]);
            return BlockRender(texture_top, texture_bottom, texture_side, block.x, block.y , block.z);
        }

        extract<int> texture_top(textures[block.id][0]);
        extract<int> texture_bottom(textures[block.id][1]);
        extract<int> texture_side(textures[block.id][2]);

        return BlockRender(texture_top, texture_bottom, texture_side, block.x, block.y, block.z);
    }

    // gl stuff

    void renderTopSide(GL *qb, int block_r=255, int block_g=255, int block_b=255)
    {
        qb->setColori(block_r, block_g, block_b);
        qb->setTexCoord(this->top_x + 0.0f, this->top_y + this->addTextCoord);
        qb->addVertex(this->x - 0.5f, 0.5f + this->y, this->z - 0.5f);
        qb->setTexCoord(this->top_x + 0.0f, this->top_y + 0.0f);
        qb->addVertex(0.5f + this->x, 0.5f + this->y, this->z - 0.5f);
        qb->setTexCoord(this->top_x + this->addTextCoord, this->top_y + 0.0f);
        qb->addVertex(0.5f + this->x, 0.5f + this->y, 0.5f + this->z);
        qb->setTexCoord(this->top_x + this->addTextCoord, this->top_y + this->addTextCoord);
        qb->addVertex(this->x - 0.5f, 0.5f + this->y, 0.5f + this->z);
    }

    void renderBottomSide(GL *qb, int block_r=255, int block_g=255, int block_b=255)
    {
        qb->setColori(block_r, block_g, block_b);
        qb->setTexCoord(this->bottom_x + 0.0f, this->bottom_y + this->addTextCoord);
        qb->addVertex(this->x - 0.5f, this->y - 0.5f, 0.5f + this->z);
        qb->setTexCoord(this->bottom_x + 0.0f, this->bottom_y + 0.0f);
        qb->addVertex(0.5f + this->x, this->y - 0.5f, 0.5f + this->z);
        qb->setTexCoord(this->bottom_x + this->addTextCoord, this->bottom_y + 0.0f);
        qb->addVertex(0.5f + this->x, this->y - 0.5f, this->z - 0.5f);
        qb->setTexCoord(this->bottom_x + this->addTextCoord, this->bottom_y + this->addTextCoord);
        qb->addVertex(this->x - 0.5f, this->y - 0.5f, this->z - 0.5f);
    }

    void renderBackSide(GL *qb, int block_r=255, int block_g=255, int block_b=255)
    {
        qb->setColori(block_r - 40, block_g - 40, block_b - 40);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + this->addTextCoord);
        qb->addVertex(this->x - 0.5f, this->y - 0.5f, this->z - 0.5f);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + this->addTextCoord);
        qb->addVertex(0.5f + this->x, this->y - 0.5f, this->z - 0.5f);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + 0.0f);
        qb->addVertex(0.5f + this->x, 0.5f + this->y, this->z - 0.5f);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + 0.0f);
        qb->addVertex(this->x - 0.5f, 0.5f + this->y, this->z - 0.5f);
    }

    void renderRightSide(GL *qb, int block_r=255, int block_g=255, int block_b=255)
    {
        qb->setColori(block_r - 80, block_g - 80, block_b - 80);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + this->addTextCoord);
        qb->addVertex(0.5f + this->x, this->y - 0.5f, this->z - 0.5f);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + this->addTextCoord);
        qb->addVertex(0.5f + this->x, this->y - 0.5f, 0.5f + this->z);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + 0.0f);
        qb->addVertex(0.5f + this->x, 0.5f + this->y, 0.5f + this->z);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + 0.0f);
        qb->addVertex(0.5f + this->x, 0.5f + this->y, this->z - 0.5f);
    }

    void renderFrontSide(GL *qb, int block_r=255, int block_g=255, int block_b=255)
    {
        qb->setColori(block_r - 40, block_g - 40, block_b - 40);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + this->addTextCoord);
        qb->addVertex(0.5f + this->x, this->y - 0.5f, 0.5f + this->z);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + this->addTextCoord);
        qb->addVertex(this->x - 0.5f, this->y - 0.5f, 0.5f + this->z);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + 0.0f);
        qb->addVertex(this->x - 0.5f, 0.5f + this->y, 0.5f + this->z);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + 0.0f);
        qb->addVertex(0.5f + this->x, 0.5f + this->y, 0.5f + this->z);
    }

    void renderLeftSide(GL *qb, int block_r=255, int block_g=255, int block_b=255)
    {
        qb->setColori(block_r - 80, block_g - 80, block_b - 80);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + this->addTextCoord);
        qb->addVertex(this->x - 0.5f, this->y - 0.5f, 0.5f + this->z);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + this->addTextCoord);
        qb->addVertex(this->x - 0.5f, this->y - 0.5f, this->z - 0.5f);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + 0.0f);
        qb->addVertex(this->x - 0.5f, 0.5f + this->y, this->z - 0.5f);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + 0.0f);
        qb->addVertex(this->x - 0.5f, 0.5f + this->y, 0.5f + this->z);
    }

};
