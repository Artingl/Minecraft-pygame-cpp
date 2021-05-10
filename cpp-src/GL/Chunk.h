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

    //std::map<std::pair<int, std::pair<int, int>>, BlockRender*> blocks_by_real_coords;
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
        //blocks_by_real_coords = std::map<std::pair<int, std::pair<int, int>>, BlockRender*>();

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

                    if (haveToRenderBlock(0, block_x, block_y, block_z))
                    {
                        block.renderTopSide(qb);
                    }

                    if (haveToRenderBlock(1, block_x, block_y, block_z))
                    {
                        block.renderBottomSide(qb);
                    }

                    if (haveToRenderBlock(2, block_x, block_y, block_z))
                    {
                        block.renderBackSide(qb);
                    }

                    if (haveToRenderBlock(3, block_x, block_y, block_z))
                    {
                        block.renderRightSide(qb);
                    }

                    if (haveToRenderBlock(4, block_x, block_y, block_z))
                    {
                        block.renderFrontSide(qb);
                    }

                    if (haveToRenderBlock(5, block_x, block_y, block_z))
                    {
                        block.renderLeftSide(qb);
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
        if (side == 0) return !this->level->blocks[std::make_pair(x,    std::make_pair(y + 1,z    ))].exist;
        if (side == 1) return !this->level->blocks[std::make_pair(x,    std::make_pair(y - 1,z    ))].exist;
        if (side == 2) return !this->level->blocks[std::make_pair(x,    std::make_pair(y,    z - 1))].exist;
        if (side == 3) return !this->level->blocks[std::make_pair(x + 1,std::make_pair(y,    z    ))].exist;
        if (side == 4) return !this->level->blocks[std::make_pair(x,    std::make_pair(y,    z + 1))].exist;
        if (side == 5) return !this->level->blocks[std::make_pair(x - 1,std::make_pair(y,    z    ))].exist;

        return false;
    }

    void render()
    {
        glCallList(chunkList);
    }

};
