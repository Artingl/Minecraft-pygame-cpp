#include <boost/python/dict.hpp>
#include <boost/python.hpp>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include "AABB.h"
#include "GL/Texture.h"
#include "GL/Chunk.h"
#include "noise.h"

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

    //Py_Initialize();

    def("_gl_engine_init", _gl_engine_init);
    def("_gl_engine_quit", _gl_engine_quit);

    class_<AABB>("AABB", init<double, double, double, double, double, double>())
            .def("getMinX", &AABB::getMinX)
            .def("getMinY", &AABB::getMinY)
            .def("getMinZ", &AABB::getMinZ)
            .def("getMaxX", &AABB::getMaxX)
            .def("getMaxY", &AABB::getMaxY)
            .def("getMaxZ", &AABB::getMaxZ)
            .def("getExist", &AABB::getExist)
            .def("setExist", &AABB::setExist)
            .def("clipXCollide", &AABB::clipXCollide)
            .def("clipYCollide", &AABB::clipYCollide)
            .def("clipZCollide", &AABB::clipZCollide)
            .def("move", &AABB::move)
            ;

    class_<Chunk>("Chunk", init<int, int, int, dict>())
            .def("erase", &Chunk::erase)
            .def("setBlock", &Chunk::setBlock)
            .def("getBlock", &Chunk::getBlock)
            .def("getBlockAABB", &Chunk::getBlockAABB)
            .def("setChunk", &Chunk::setChunk)
            .def("getPrepared", &Chunk::getPrepared)
            .def("getX", &Chunk::getX)
            .def("getZ", &Chunk::getZ)
            .def("removeBlock", &Chunk::removeBlock)
            .def("prepareChunk", &Chunk::prepareChunk)
            .def("update", &Chunk::update)
            .def("checkBlockSide", &Chunk::checkBlockSide)
            .def("render", &Chunk::render)
            ;
}
