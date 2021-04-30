#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Shader.h"
#include "vertex.h"
#include "Mesh.h"

void _gl_engine_RENDER_INIT(Shader *_shaderProgram);
void _gl_engine_RENDER_DELETE();
void _gl_engine_RENDER_DRAW();


void _gl_engine_update_camera_vectors();
void _gl_engine_Perspective(float fov, float width, float height, float nearPlane, float farPlane);
void _gl_engine_SET_2D(int width, int height);
void _gl_engine_LoadIdentity();
void _gl_engine_MOVE(float x, float y, float z, float rotation_pitch, float rotation_yaw);
