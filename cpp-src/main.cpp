#include <boost/python.hpp>
#include "GL/Render.h"
#include "debug.h"

GLuint core_shaderProgram;

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

    core_shaderProgram = glCreateProgram();
    glAttachShader(core_shaderProgram, vertexShader);
    glAttachShader(core_shaderProgram, fragmentShader);
    glLinkProgram(core_shaderProgram);

    // Check shaders
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        _gl_engine_handle_error("_gl_engine_init", "ERROR::SHADER::VERTEX::COMPILATION_FAILED", infoLog);
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        _gl_engine_handle_error("_gl_engine_init", "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED", infoLog);
    }
    glGetProgramiv(core_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(core_shaderProgram, 512, NULL, infoLog);
        _gl_engine_handle_error("_gl_engine_init", "ERROR::SHADERPROGRAM::COMPILATION_FAILED", infoLog);
    }
    //

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    _gl_engine_RENDER_INIT(core_shaderProgram);
}

void _gl_engine_quit() {
    _gl_engine_info("_gl_engine_quit", "Quit the game");

    _gl_engine_RENDER_DELETE();
    glDeleteProgram(core_shaderProgram);
}

void _gl_engine_draw() {
    //_gl_engine_info("_gl_engine_draw", "Draw stuff");
    _gl_engine_RENDER_DRAW();
}

void createCube(int textureId, int x, int y, int z) {

}

BOOST_PYTHON_MODULE(opengl_main_cpp)
{
    using namespace boost::python;
    def("drawCube", createCube);
    def("_gl_engine_init", _gl_engine_init);
    def("_gl_engine_draw", _gl_engine_draw);
    def("_gl_engine_quit", _gl_engine_quit);

    // GL sutff
    def("_gl_engine_Perspective", _gl_engine_Perspective);
    def("_gl_engine_SET_2D", _gl_engine_SET_2D);
    def("_gl_engine_LoadIdentity", _gl_engine_LoadIdentity);
    def("_gl_engine_Rotatef", _gl_engine_Rotatef);
    def("_gl_engine_Translatef", _gl_engine_Translatef);
    //
}
