#include <boost/python.hpp>
#include "VAO.h"
#include "debug.h"


void _gl_engine_init() {
    _gl_engine_info("_gl_engine_init", "Initializing engine...");

    glewInit();
    VAO_INIT();
}

void _gl_engine_draw() {
    VAO_DRAW();
}

void drawCube(int textureId, int x, int y, int z) {

}

BOOST_PYTHON_MODULE(opengl_main_cpp)
{
    using namespace boost::python;
    def("drawCube", drawCube);
    def("_gl_engine_init", _gl_engine_init);
    def("_gl_engine_draw", _gl_engine_draw);
}
