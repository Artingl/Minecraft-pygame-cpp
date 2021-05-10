#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../debug.h"

class Shader {
private:
    GLuint id;

    const char *readShaderFile(char* path)
    {
        FILE *f = fopen(path, "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

        char *string = new char[fsize + 1];
        fread(string, 1, fsize, f);
        fclose(f);

        string[fsize] = 0;

        return string;
    }

    GLuint loadShader(GLenum type, char* fileName)
    {
        printf("[INFO] [SHADER] [Shader->loadShader]: Loading vertex shader %s\n", fileName);

        GLuint shader;
        shader = glCreateShader(type);
        const char *vertex_shader_var = this->readShaderFile(fileName);
        glShaderSource(shader, 1, &vertex_shader_var, nullptr);
        glCompileShader(shader);

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            _gl_engine_handle_error("engineInit", "ERROR::SHADER::COMPILATION_FAILED", infoLog);
        }

        return shader;
    }

    void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader)
    {
        GLint success;
        GLchar infoLog[512];

        this->id = glCreateProgram();
        glAttachShader(this->id, vertexShader);
        if (geometryShader) {
            glAttachShader(this->id, geometryShader);
        }
        glAttachShader(this->id, fragmentShader);
        glLinkProgram(this->id);

        glGetProgramiv(this->id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(this->id, 512, NULL, infoLog);
            _gl_engine_handle_error("engineInit", "ERROR::SHADER::LINK_FAILED", infoLog);
        }

        glDeleteShader(geometryShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

    }

public:

    Shader() {}

    Shader(char* vertexFile, char* fragmentFile, char* geometryFile="")
    {
        GLuint vertexShader;
        GLuint fragmentShader;
        GLuint geometryShader = 0;

        vertexShader = this->loadShader(GL_VERTEX_SHADER, vertexFile);
        fragmentShader = this->loadShader(GL_FRAGMENT_SHADER, fragmentFile);

        if (geometryFile != "")
        {
            geometryShader = this->loadShader(GL_GEOMETRY_SHADER, geometryFile);
        }

        this->linkProgram(vertexShader, geometryShader, fragmentShader);
    }

    void DeleteProgram()
    {
        glDeleteProgram(this->id);
    }

    void use()
    {
        glUseProgram(this->id);
    }

    void unuse()
    {
        glUseProgram(0);
    }

    void setVec1i(GLfloat value, GLchar* name)
    {
        this->use();
        glUniform1i(glGetUniformLocation(this->id, name), value);
        this->unuse();

    }

    void setVec1f(GLfloat value, GLchar* name)
    {
        this->use();
        glUniform1f(glGetUniformLocation(this->id, name), value);
        this->unuse();

    }

    void setVec2f(glm::fvec2 value, GLchar* name)
    {
        this->use();
        glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
        this->unuse();

    }

    void setVec3f(glm::fvec3 value, GLchar* name)
    {
        this->use();
        glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
        this->unuse();

    }

    void setVec4f(glm::fvec4 value, GLchar* name)
    {
        this->use();
        glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
        this->unuse();

    }

    void setMat3fv(glm::mat3 value, GLchar* name, GLboolean transpose= GL_FALSE)
    {
        this->use();
        glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
        this->unuse();
    }

    void setMat4fv(glm::mat4 value, GLchar* name, GLboolean transpose= GL_FALSE)
    {
        this->use();
        glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
        this->unuse();
    }

    GLuint getID()
    {
        return this->id;
    }

};
