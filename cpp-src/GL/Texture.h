#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../debug.h"

class Texture {
private:
    GLuint id;
    int width;
    int height;

public:

    Texture() {}

    Texture(GLuint id, int width, int height)
    {
        printf("[INFO] [TEXTURE] [Texture->construct]: Loading texture ID:%d WIDTH:%d HEIGHT:%d\n", id, width, height);

        this->id = id;
        this->width = width;
        this->height = height;
    }

    GLuint getID()
    {
        return this->id;
    }

    void bind(GLuint unit, GLenum type)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(type, this->id);
    }

};
