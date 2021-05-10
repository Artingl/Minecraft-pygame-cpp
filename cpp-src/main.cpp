#include <boost/python/dict.hpp>
#include <boost/python.hpp>
#include "AABB.h"
#include "GL/BlockRender.h"
#include "GL/Chunk.h"
#include "GL/World.h"
#include "GL/Level.h"

World *world;

void engineInit(int _renderDistance, int texturesId, dict textures)
{
    _gl_engine_info("engineInit", "Initializing engine...");

    glewInit();
    world = new World(_renderDistance, texturesId, textures);
}

void engineQuit() {
    _gl_engine_info("engineQuit", "Quit the game");
}

void updateWorld(float player_x, float player_y, float player_z)
{
    world->update(player_x, player_y, player_z);
}

bool isUnderWater(int x, int y, int z)
{
    return world->isUnderWater(x, y, z);
}

int getBlockExist(int x, int y, int z)
{
    return world->getBlockExist(x, y, z);
}

void removeBlock(int x, int y, int z)
{
    world->removeBlock(x, y, z);
}

BOOST_PYTHON_MODULE(opengl_main_cpp)
{
    using namespace boost::python;

    def("engineInit", engineInit);
    def("engineQuit", engineQuit);
    def("updateWorld", updateWorld);
    def("getBlockExist", getBlockExist);
    def("removeBlock", removeBlock);
    def("isUnderWater", isUnderWater);

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

    class_<Chunk>("Chunk", init<int, int, int, Level*, dict>())
            .def("erase", &Chunk::erase)
            .def("update", &Chunk::update)
            .def("haveToRenderBlock", &Chunk::haveToRenderBlock)
            .def("render", &Chunk::render)
            ;
}
