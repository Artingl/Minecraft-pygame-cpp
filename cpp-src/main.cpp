#include <boost/python.hpp>
#include "VAO.h"
#include "debug.h"

GLuint shaderProgram;

GLfloat vertices[] = {
        0.5, 0, 0,
        0.5, 0.5, 0.5,
        0.5, 0.5, 0,
        0.5, 0, 0,
};
GLuint VBO;

const char *readFile(char* path)
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

void _gl_engine_init() {
    _gl_engine_info("_gl_engine_init", "Initializing engine...");

    glewInit();

    const char* test_read = readFile("settings.py");

    // Load vertex shader
    _gl_engine_info("_gl_engine_init", "Loading vertex shader...");

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertex_shader_var = readFile("shaders/vertex.glsl");
    glShaderSource(vertexShader, 1, &vertex_shader_var, nullptr);
    glCompileShader(vertexShader);
    //

    // Load fragment shader
    _gl_engine_info("_gl_engine_init", "Loading fragment shader...");

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragment_shader = readFile("shaders/fragment.glsl");
    glShaderSource(fragmentShader, 1, &fragment_shader, nullptr);
    glCompileShader(fragmentShader);
    //

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check shaders
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        _gl_engine_handle_error("_gl_engine_init", "ERROR::SHADER::VERTEX::COMPILATION_FAILED", infoLog);
    }
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        _gl_engine_handle_error("_gl_engine_init", "ERROR::SHADERPROGRAM::COMPILATION_FAILED", infoLog);
    }
    //

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    _gl_engine_VAO_INIT();
}

void _gl_engine_draw() {
    _gl_engine_info("_gl_engine_draw", "Draw stuff");

    //_gl_engine_VBO_DRAW();
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glUseProgram(shaderProgram);

}

void createCube(int textureId, int x, int y, int z) {

}

BOOST_PYTHON_MODULE(opengl_main_cpp)
{
    using namespace boost::python;
    def("drawCube", createCube);
    def("_gl_engine_init", _gl_engine_init);
    def("_gl_engine_draw", _gl_engine_draw);
}
