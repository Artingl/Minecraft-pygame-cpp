#pragma once

#include <boost/python/dict.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../debug.h"

using namespace boost::python;

class Texture {
private:
    float width = 16.0f;
    float height = 16.0f;


    float top_x;      float top_y;
    float bottom_x;   float bottom_y;
    float side_x;     float side_y;

    void create()
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

public:
    int top;
    int bottom;
    int side;
    int exist = -1;
    float addTextCoord = 1.0f / 100.0f;

    Texture() {}

    Texture (int id_top, int id_bottom, int id_side)
    {
        this->top = id_top;
        this->bottom = id_bottom;
        this->side = id_side;

        create();
    }

    Texture (int id)
    {
        this->top = id;
        this->bottom = id;
        this->side = id;

        create();
    }

    static Texture getTexture(const char *id, dict textures)
    {
        if (!textures.contains(id))
        {
            extract<int> texture_top(textures["invalid"][0]);
            extract<int> texture_bottom(textures["invalid"][1]);
            extract<int> texture_side(textures["invalid"][2]);
            return Texture(texture_top, texture_bottom, texture_side);
        }

        extract<int> texture_top(textures[id][0]);
        extract<int> texture_bottom(textures[id][1]);
        extract<int> texture_side(textures[id][2]);

        return Texture(texture_top, texture_bottom, texture_side);
    }

    float getTopX() const { return this->top_x; }

    float getTopY() const { return this->top_y; }

    float getBottomX() const { return this->bottom_x; }

    float getBottomY() const { return this->bottom_y; }

    float getSideX() const { return this->side_x; }

    float getSideY() const { return this->side_y; }

};
