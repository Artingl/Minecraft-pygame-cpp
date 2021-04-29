#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texcoord;
};

void _gl_engine_RENDER_INIT(GLuint _shaderProgram);
void _gl_engine_RENDER_DELETE();
void _gl_engine_RENDER_DRAW();

void _gl_engine_LoadIdentity();
void _gl_engine_Rotatef(float rot, float x, float y, float z);
void _gl_engine_Translatef(float x, float y, float z);
