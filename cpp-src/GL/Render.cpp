#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>

#include "Render.h"
#include "../debug.h"

glm::mat4 ModelMatrix;
Shader *shaderProgram;

glm::vec3 camPosition(0.f, 0.f, 0.f);

Vertex vertices[] = {
        glm::vec3(-0.5f, 0.5f, 0.0f),   glm::vec3(0.7f, 0.7f, 0.7f),   glm::vec2(0.0f, 1.0f),
        glm::vec3(-0.5f, -0.5f, 0.0f),  glm::vec3(0.7f, 0.7f, 0.7f),   glm::vec2(0.0f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),   glm::vec3(0.7f, 0.7f, 0.7f),   glm::vec2(1.0f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.0f),    glm::vec3(0.7f, 0.7f, 0.7f),   glm::vec2(1.0f, 1.0f)
};
GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
};

unsigned nov = sizeof(vertices) / sizeof(Vertex);
unsigned noi = sizeof(indices) / sizeof(GLuint);

GLuint EBO;
GLuint VAO;
GLuint VBO;


void _gl_engine_RENDER_INIT(Shader *_shaderProgram) {
    _gl_engine_info("_gl_engine_VAO_INIT", "Initializing VAO...");

    shaderProgram = _shaderProgram;

    // VAO
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //

    // Set vertex array pointers and enable

    // --- Position attrib ---
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FLOAT, sizeof(Vertex), (GLvoid*) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // ---

    // --- Color attrib ---
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FLOAT, sizeof(Vertex), (GLvoid*) offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    // ---

    // --- Texcoord attrib ---
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FLOAT, sizeof(Vertex), (GLvoid*) offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);
    // ---

    glBindVertexArray(0);
    //

}

void _gl_engine_RENDER_DELETE() {
    _gl_engine_info("_gl_engine_VAO_DELETE", "Deleting VAO...");

}

void _gl_engine_RENDER_DRAW() {
    shaderProgram->use(); // Use our shader program

    glBindVertexArray(VAO);    // Bind vertex array
    glDrawElements(GL_TRIANGLES, noi, GL_UNSIGNED_INT, 0);         // Draw

    glBindVertexArray(0);           // Unbind vertex array
    shaderProgram->unuse();

}

void _gl_engine_LoadIdentity() {
    ModelMatrix = glm::mat4(1.0f);
}



// gl stuff

void _gl_engine_Perspective(float fov, float width, float height, float nearPlane, float farPlane)
{
    glm::vec3 worldUp(0.f, 1.f, 0.f);
    glm::vec3 camFront (0.f, 0.f, -1.f);
    glm::mat4 ViewMatrix(1.f);

    ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);
    glm::mat4 ProjectionMatrix(1.f);
    ProjectionMatrix = glm::perspective(fov, static_cast<float>(width) / height, nearPlane, farPlane);

    shaderProgram->setMat4fv(ModelMatrix, "ModelMatrix");
    shaderProgram->setMat4fv(ViewMatrix, "ViewMatrix");
    shaderProgram->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

}


void _gl_engine_SET_2D(int width, int height)
{

}

void _gl_engine_MOVE(float x, float y, float z, float rotation_pitch, float rotation_yaw)
{
    ModelMatrix = glm::rotate(ModelMatrix, rotation_pitch, glm::vec3(1, 0, 0));
    ModelMatrix = glm::rotate(ModelMatrix, rotation_yaw, glm::vec3(0, 1, 0));
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(x, y, z));

    shaderProgram->setMat4fv(ModelMatrix, "ModelMatrix");
}
