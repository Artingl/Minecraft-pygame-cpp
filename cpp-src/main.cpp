#include <boost/python.hpp>
#include <SFML/OpenGL.hpp>
#include "VAO.h"

void _gl_engine_init() {
    VBO_INIT();
}

void _gl_engine_draw() {

}

void drawCube(int textureId, int x, int y, int z)
{

}

BOOST_PYTHON_MODULE(opengl_main_cpp)
{
    using namespace boost::python;
    def("drawCube", drawCube);
    def("_gl_engine_init", _gl_engine_init);
    def("_gl_engine_draw", _gl_engine_draw);
}
