#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>

#include "Render.h"
#include "../debug.h"

GLuint shaderProgram;

Vertex vertices[] = {
        glm::vec3(-0.5f, 0.5f, 0.0f),   glm::vec3(1.0f, 0.0f, 0.0f),   glm::vec2(0.0f, 1.0f),
        glm::vec3(-0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec2(0.0f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),   glm::vec3(0.0f, 0.0f, 1.0f),   glm::vec2(1.0f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.0f),    glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec2(1.0f, 1.0f)
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


void _gl_engine_RENDER_INIT(GLuint _shaderProgram) {
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
    glUseProgram(shaderProgram); // Use our shader program

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 23);

    glBindVertexArray(VAO);    // Bind vertex array
    glDrawElements(GL_TRIANGLES, noi, GL_UNSIGNED_INT, 0);         // Draw
    glBindVertexArray(0);           // Unbind vertex array

}



void _gl_engine_LoadIdentity() {
    glm::mat4 ModelMatrix(1.f);
}

