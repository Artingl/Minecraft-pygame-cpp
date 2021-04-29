#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void _gl_engine_VAO_INIT();
void _gl_engine_VAO_DELETE();
void _gl_engine_VBO_LOAD(GLfloat *vao_pos, GLfloat *vao_col, GLuint *vao_ix);
void _gl_engine_VBO_DRAW();
