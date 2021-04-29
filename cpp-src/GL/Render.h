#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texcoord;
};

void _gl_engine_RENDER_INIT(GLuint _shaderProgram);
void _gl_engine_RENDER_DELETE();
void _gl_engine_RENDER_DRAW();

void _gl_engine_LoadIdentity();
