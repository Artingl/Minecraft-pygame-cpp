#include <boost/python.hpp>
#include <cmath>

#include "GL/Texture.h"
#include "GL/Chunk.h"

int renderDistance;

void _gl_engine_init(int _renderDistance)
{
    _gl_engine_info("_gl_engine_init", "Initializing engine...");

    glewInit();
    renderDistance = _renderDistance;
}

void _gl_engine_quit() {
    _gl_engine_info("_gl_engine_quit", "Quit the game");
}

BOOST_PYTHON_MODULE(opengl_main_cpp)
{
    using namespace boost::python;

    def("_gl_engine_init", _gl_engine_init);
    def("_gl_engine_quit", _gl_engine_quit);

    class_<Chunk>("Chunk", init<int, int>())
            .def("erase", &Chunk::erase)
            .def("prepareChunk", &Chunk::prepareChunk)
            .def("setPerlinNoise", &Chunk::setPerlinNoise)
            .def("update", &Chunk::update)
            .def("checkBlockSide", &Chunk::checkBlockSide)
            .def("render", &Chunk::render)
            ;
}
