#include <boost/python.hpp>
#include "GL/Render.h"
#include "debug.h"
#include "GL/Shader.h"
#include "GL/Texture.h"

Texture texture(23, 16, 16);

Shader *core_shaderProgram;

void _gl_engine_init() {
    _gl_engine_info("_gl_engine_init", "Initializing engine...");

    glewInit();
    core_shaderProgram = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    _gl_engine_RENDER_INIT(core_shaderProgram);
}

void _gl_engine_quit() {
    _gl_engine_info("_gl_engine_quit", "Quit the game");

    _gl_engine_RENDER_DELETE();
    core_shaderProgram->DeleteProgram();
}

void _gl_engine_draw() {
    //_gl_engine_info("_gl_engine_draw", "Draw stuff");

    texture.bind(0, GL_TEXTURE_2D);
    _gl_engine_RENDER_DRAW();
}

void _gl_engine_DRAW_CUBE(int textureId, int x, int y, int z) {
    //_gl_engine_info("_gl_engine_DRAW_CUBE", "Create some cube");


}

BOOST_PYTHON_MODULE(opengl_main_cpp)
{
    using namespace boost::python;
    def("_gl_engine_DRAW_CUBE", _gl_engine_DRAW_CUBE);
    def("_gl_engine_init", _gl_engine_init);
    def("_gl_engine_draw", _gl_engine_draw);
    def("_gl_engine_quit", _gl_engine_quit);

    // GL sutff
    def("_gl_engine_MOVE", _gl_engine_MOVE);
    def("_gl_engine_Perspective", _gl_engine_Perspective);
    def("_gl_engine_SET_2D", _gl_engine_SET_2D);
    def("_gl_engine_LoadIdentity", _gl_engine_LoadIdentity);
    //
}
