#pragma once

#include <boost/python/dict.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GL.h"
#include "../debug.h"
#include "Block.h"

using namespace boost::python;

//https://stackoverflow.com/questions/1031645/how-to-detect-utf-8-in-plain-c
bool is_utf8(const char * string)
{
    if(!string)
        return 0;

    const unsigned char * bytes = (const unsigned char *)string;
    while(*bytes)
    {
        if( (// ASCII
                // use bytes[0] <= 0x7F to allow ASCII control characters
                bytes[0] == 0x09 ||
                bytes[0] == 0x0A ||
                bytes[0] == 0x0D ||
                (0x20 <= bytes[0] && bytes[0] <= 0x7E)
        )
                ) {
            bytes += 1;
            continue;
        }

        if( (// non-overlong 2-byte
                (0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF)
        )
                ) {
            bytes += 2;
            continue;
        }

        if( (// excluding overlongs
                    bytes[0] == 0xE0 &&
                    (0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
                    (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            ) ||
            (// straight 3-byte
                    ((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
                     bytes[0] == 0xEE ||
                     bytes[0] == 0xEF) &&
                    (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                    (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            ) ||
            (// excluding surrogates
                    bytes[0] == 0xED &&
                    (0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
                    (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            )
                ) {
            bytes += 3;
            continue;
        }

        if( (// planes 1-3
                    bytes[0] == 0xF0 &&
                    (0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
                    (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                    (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            ) ||
            (// planes 4-15
                    (0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
                    (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                    (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                    (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            ) ||
            (// plane 16
                    bytes[0] == 0xF4 &&
                    (0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
                    (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                    (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            )
                ) {
            bytes += 4;
            continue;
        }

        return 0;
    }

    return 1;
}//



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
        extract<int> _invalid_texture(textures["invalid"][0]);

        if (!block.id || block.id == NULL || block.id == "")
        {
            return BlockRender(_invalid_texture, _invalid_texture, _invalid_texture, block.x, block.y , block.z);
        }
        if (!is_utf8(block.id))
        {
            return BlockRender(_invalid_texture, _invalid_texture, _invalid_texture, block.x, block.y , block.z);
        }
        if (!textures.contains(block.id))
        {
            return BlockRender(_invalid_texture, _invalid_texture, _invalid_texture, block.x, block.y , block.z);
        }

        //printf("%s\n", block.id);

        extract<int> texture_top(textures[block.id][0]);
        extract<int> texture_bottom(textures[block.id][1]);
        extract<int> texture_side(textures[block.id][2]);

        return BlockRender(texture_top, texture_bottom, texture_side, block.x, block.y, block.z);
    }

    // gl stuff

    void renderTopSide(GL *qb, float down, int block_r=255, int block_g=255, int block_b=255)
    {
        qb->setColori(block_r, block_g, block_b);
        qb->setTexCoord(this->top_x + 0.0f, this->top_y + this->addTextCoord);
        qb->addVertex(this->x - 0.5f, 0.5f + this->y - down, this->z - 0.5f);
        qb->setTexCoord(this->top_x + 0.0f, this->top_y + 0.0f);
        qb->addVertex(0.5f + this->x, 0.5f + this->y - down, this->z - 0.5f);
        qb->setTexCoord(this->top_x + this->addTextCoord, this->top_y + 0.0f);
        qb->addVertex(0.5f + this->x, 0.5f + this->y - down, 0.5f + this->z);
        qb->setTexCoord(this->top_x + this->addTextCoord, this->top_y + this->addTextCoord);
        qb->addVertex(this->x - 0.5f, 0.5f + this->y - down, 0.5f + this->z);
    }

    void renderBottomSide(GL *qb, float down, int block_r=255, int block_g=255, int block_b=255)
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

    void renderBackSide(GL *qb, float down, int block_r=255, int block_g=255, int block_b=255)
    {
        qb->setColori(block_r - 40, block_g - 40, block_b - 40);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + this->addTextCoord);
        qb->addVertex(this->x - 0.5f, this->y - 0.5f - down, this->z - 0.5f);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + this->addTextCoord);
        qb->addVertex(0.5f + this->x, this->y - 0.5f - down, this->z - 0.5f);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + 0.0f);
        qb->addVertex(0.5f + this->x, 0.5f + this->y - down, this->z - 0.5f);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + 0.0f);
        qb->addVertex(this->x - 0.5f, 0.5f + this->y - down, this->z - 0.5f);
    }

    void renderRightSide(GL *qb, float down, int block_r=255, int block_g=255, int block_b=255)
    {
        qb->setColori(block_r - 80, block_g - 80, block_b - 80);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + this->addTextCoord);
        qb->addVertex(0.5f + this->x, this->y - 0.5f - down, this->z - 0.5f);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + this->addTextCoord);
        qb->addVertex(0.5f + this->x, this->y - 0.5f - down, 0.5f + this->z);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + 0.0f);
        qb->addVertex(0.5f + this->x, 0.5f + this->y - down, 0.5f + this->z);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + 0.0f);
        qb->addVertex(0.5f + this->x, 0.5f + this->y - down, this->z - 0.5f);
    }

    void renderFrontSide(GL *qb, float down, int block_r=255, int block_g=255, int block_b=255)
    {
        qb->setColori(block_r - 40, block_g - 40, block_b - 40);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + this->addTextCoord);
        qb->addVertex(0.5f + this->x, this->y - 0.5f - down, 0.5f + this->z);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + this->addTextCoord);
        qb->addVertex(this->x - 0.5f, this->y - 0.5f - down, 0.5f + this->z);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + 0.0f);
        qb->addVertex(this->x - 0.5f, 0.5f + this->y - down, 0.5f + this->z);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + 0.0f);
        qb->addVertex(0.5f + this->x, 0.5f + this->y - down, 0.5f + this->z);
    }

    void renderLeftSide(GL *qb, float down, int block_r=255, int block_g=255, int block_b=255)
    {
        qb->setColori(block_r - 80, block_g - 80, block_b - 80);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + this->addTextCoord);
        qb->addVertex(this->x - 0.5f, this->y - 0.5f - down, 0.5f + this->z);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + this->addTextCoord);
        qb->addVertex(this->x - 0.5f, this->y - 0.5f - down, this->z - 0.5f);
        qb->setTexCoord(this->side_x + 0.0f, this->side_y + 0.0f);
        qb->addVertex(this->x - 0.5f, 0.5f + this->y - down, this->z - 0.5f);
        qb->setTexCoord(this->side_x + this->addTextCoord, this->side_y + 0.0f);
        qb->addVertex(this->x - 0.5f, 0.5f + this->y - down, 0.5f + this->z);
    }

};
