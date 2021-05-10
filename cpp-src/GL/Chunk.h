#pragma once

#include <boost/python/dict.hpp>
#include "GL.h"
#include "../AABB.h"
#include "BlockRender.h"
#include <GL/glew.h>
#include "Level.h"
#include "Block.h"

#define chunk_width 16
#define chunk_height 16
#define chunk_depth 16

using namespace boost::python;

class Chunk
{
public:
    GL *qb{};
    Level *level;

    int x{};
    int y{};
    int z{};
    int chunkList{};
    dict textures;

    Chunk(int x, int y, int z, Level *level, dict textures)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->level = level;
        this->textures = textures;

        this->chunkList = 0;
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

        qb = new GL(chunk_width * chunk_height * chunk_depth);
        qb->begin();

        int xcw = this->x * chunk_width;
        int ycw = this->y * chunk_height;
        int zcw = this->z * chunk_depth;

        for (int block_x = xcw; block_x <= chunk_width + xcw; ++block_x)
        {
            for (int block_y = ycw; block_y <= chunk_height + ycw; ++block_y)
            {
                for (int block_z = zcw; block_z <= chunk_depth + zcw; ++block_z)
                {
                    Block level_block = this->level->getBlock(block_x, block_y, block_z);
                    if (!level_block.exist) continue;
                    BlockRender block = BlockRender::getBlock(this->textures,
                                                              Block(level_block.id, level_block.x - xcw, level_block.y - ycw, level_block.z - zcw));
                    float down = 0.0f;
                    if (level_block.liquid)// && !this->level->getBlock(block_x, block_y + 1, block_z).liquid)
                        down = 0.1f;

                    if (haveToRenderBlock(0, block_x, block_y, block_z))
                    {
                        block.renderTopSide(qb, down);
                    }

                    if (haveToRenderBlock(1, block_x, block_y, block_z))
                    {
                        block.renderBottomSide(qb, down);
                    }

                    if (haveToRenderBlock(2, block_x, block_y, block_z))
                    {
                        block.renderBackSide(qb, down);
                    }

                    if (haveToRenderBlock(3, block_x, block_y, block_z))
                    {
                        block.renderRightSide(qb, down);
                    }

                    if (haveToRenderBlock(4, block_x, block_y, block_z))
                    {
                        block.renderFrontSide(qb, down);
                    }

                    if (haveToRenderBlock(5, block_x, block_y, block_z))
                    {
                        block.renderLeftSide(qb, down);
                    }

                }
            }
        }

        qb->render();
        glPopMatrix();
        glEndList();
    }

    bool haveToRenderBlock(int side, int x, int y, int z)
    {
        Block current = this->level->getBlock(x, y, z);
        if (current.liquid)
        {
            if (side == 0) return !this->level->blocks[std::make_pair(x,    std::make_pair(y + 1,z    ))].exist;
            if (side == 1) return !this->level->blocks[std::make_pair(x,    std::make_pair(y - 1,z    ))].exist;
            if (side == 2) return !this->level->blocks[std::make_pair(x,    std::make_pair(y,    z - 1))].exist;
            if (side == 3) return !this->level->blocks[std::make_pair(x + 1,std::make_pair(y,    z    ))].exist;
            if (side == 4) return !this->level->blocks[std::make_pair(x,    std::make_pair(y,    z + 1))].exist;
            if (side == 5) return !this->level->blocks[std::make_pair(x - 1,std::make_pair(y,    z    ))].exist;
        }

        if (side == 0) return !this->level->blocks[std::make_pair(x,    std::make_pair(y + 1,z    ))].exist || this->level->blocks[std::make_pair(x,    std::make_pair(y + 1,z    ))].liquid;
        if (side == 1) return !this->level->blocks[std::make_pair(x,    std::make_pair(y - 1,z    ))].exist;
        if (side == 2) return !this->level->blocks[std::make_pair(x,    std::make_pair(y,    z - 1))].exist || this->level->blocks[std::make_pair(x,    std::make_pair(y,    z - 1))].liquid;
        if (side == 3) return !this->level->blocks[std::make_pair(x + 1,std::make_pair(y,    z    ))].exist || this->level->blocks[std::make_pair(x + 1,std::make_pair(y,    z    ))].liquid;
        if (side == 4) return !this->level->blocks[std::make_pair(x,    std::make_pair(y,    z + 1))].exist || this->level->blocks[std::make_pair(x,    std::make_pair(y,    z + 1))].liquid;
        if (side == 5) return !this->level->blocks[std::make_pair(x - 1,std::make_pair(y,    z    ))].exist || this->level->blocks[std::make_pair(x - 1,std::make_pair(y,    z    ))].liquid;

        return false;
    }

    void render()
    {
        glCallList(chunkList);
    }

};
