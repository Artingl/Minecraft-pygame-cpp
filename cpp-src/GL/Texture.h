#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../debug.h"

class Texture {
private:
    float width = 16.0f;
    float height = 16.0f;

    int top;
    int bottom;
    int side;

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
    float addTextCoord = 1.0f / 100.0f;

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

    static Texture getTexture(const char *id)
    {
        if (id == "grass_block")
        {
            return Texture(54, 52, 53);
        }
        else if(id == "stone")
        {
            return Texture(42);
        }
        else if(id == "dirt")
        {
            return Texture(52);
        }
        else
        {
            return Texture(0);
        }
    }

    float getTopX() const { return this->top_x; }

    float getTopY() const { return this->top_y; }

    float getBottomX() const { return this->bottom_x; }

    float getBottomY() const { return this->bottom_y; }

    float getSideX() const { return this->side_x; }

    float getSideY() const { return this->side_y; }

};
