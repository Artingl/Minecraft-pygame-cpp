#include <boost/python.hpp>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include "GL/Texture.h"
#include "GL/Chunk.h"

int renderDistance;

float getNoiseNumber(int x, int z)
{
    float result = 0;

    for (int xx = -10; xx < 10; ++xx)
    {
        for (int zz = -10; zz < 10; ++zz)
        {
            float value = glm::simplex(glm::vec2{(xx + x) / 128.0f, (zz + z) / 128.0f});
            value = (value + 1) / 2;
            value *= 4 + 4;

            result += value / 16.0f * 0.1f;
        }
    }


    return result;
}

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

    def("getNoiseNumber", getNoiseNumber);
    def("_gl_engine_init", _gl_engine_init);
    def("_gl_engine_quit", _gl_engine_quit);

    class_<Chunk>("Chunk", init<int, int>())
            .def("erase", &Chunk::erase)
            .def("setBlock", &Chunk::setBlock)
            .def("getPrepared", &Chunk::getPrepared)
            .def("getX", &Chunk::getX)
            .def("getZ", &Chunk::getZ)
            .def("removeBlock", &Chunk::removeBlock)
            .def("prepareChunk", &Chunk::prepareChunk)
            .def("setPerlinNoise", &Chunk::setPerlinNoise)
            .def("update", &Chunk::update)
            .def("checkBlockSide", &Chunk::checkBlockSide)
            .def("render", &Chunk::render)
            ;
}
