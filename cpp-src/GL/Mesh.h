#pragma once

#include "../debug.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "vertex.h"
#include <vector>

class Mesh
{
private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 ModelMatrix;

    void initVertexData(Vertex *vertexArray, const unsigned &nov, GLuint *indexArray, const unsigned &noi)
    {
        for (int i = 0; i < nov; ++i)
        {
            this->vertices.push_back(vertexArray[i]);
        }

        for (int i = 0; i < noi; ++i)
        {
            this->indices.push_back(indexArray[i]);
        }
    }

    void initVAO()
    {
        // VAO
        glCreateVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);

      // VBO
        glGenBuffers(1, &this->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        // EBO
        glGenBuffers(1, &this->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
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

    void initModelMatrix()
    {
        this->position = glm::vec3(0.f);
        this->rotation = glm::vec3(0.f);
        this->scale = glm::vec3(1.f);

        this->ModelMatrix = glm::mat4(1.0f);
        this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
        //this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
        //this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
        //this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
        //this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);

    }

    void updateUniforms(Shader *shader)
    {
        shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
    }


public:
    Mesh(Vertex *vertexArray, const unsigned &nov, GLuint *indexArray, const unsigned &noi)
    {
        this->initVertexData(vertexArray, nov, indexArray, noi);
        this->initVAO();
        this->initModelMatrix();
    }

    ~Mesh()
    {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        glDeleteBuffers(1, &this->EBO);
    }

    void update()
    {

    }

    void render(Shader *shader)
    {
        this->updateUniforms(shader);

        shader->use();
        glBindVertexArray(this->VAO);    // Bind vertex array
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);         // Draw
        //glBindVertexArray(0);           // Unbind vertex array
        //shader->unuse();
    }

};
