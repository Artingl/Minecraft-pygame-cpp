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

void setBlock(int x, int y, int z, char* name)
{
    world->setBlock(x, y, z, name);
}

void setBlockWithoutBlockCheck(int x, int y, int z, char* name)
{
    world->setBlockWithoutBlockCheck(x, y, z, name);
}

void generateLevel(int x_from, int z_from, int x_to, int z_to, int height)
{
    world->level.generateLevel(x_from, z_from, x_to, z_to, height);
}

void generateChunks(int x, int y, int z)
{
    world->generateChunks(x, y, z);
}

void deleteWorld()
{
    world->deleteWorld();
}

void saveWorld()
{
    world->saveWorld();
}

void loadWorld(boost::python::list& chunks)
{
    world->loadWorld(chunks);
}

void setLevelName(char* name)
{
    world->setLevelName(name);
}

void setSeed(int seed)
{
    world->seed = seed;
    world->level.seed = seed;
}

void setRenderDistance(int r)
{
    world->renderDistance = r;
}

str getBlockName(int x, int y, int z)
{
    if (!getBlockExist(x, y, z))
    {
        return "invalid";
    }
    return world->getBlockName(x, y, z);
}

list hitTest(float x, float y, float z, float dx, float dy, float dz, int dist, int m)
{
    return world->hitTest(x, y, z, dx, dy, dz, dist, m);
}

int getPlayerStartX()
{
    return world->level.player_start_x;
}

int getPlayerStartY()
{
    return world->level.player_start_y;
}

int getPlayerStartZ()
{
    return world->level.player_start_z;
}

BOOST_PYTHON_MODULE(opengl_main_cpp)
{
    using namespace boost::python;

    def("engineInit", engineInit);
    def("engineQuit", engineQuit);
    def("updateWorld", updateWorld);
    def("getBlockExist", getBlockExist);
    def("removeBlock", removeBlock);
    def("setBlock", setBlock);
    def("isUnderWater", isUnderWater);
    def("generateLevel", generateLevel);
    def("generateChunks", generateChunks);
    def("deleteWorld", deleteWorld);
    def("hitTest", hitTest);
    def("setLevelName", setLevelName);
    def("loadWorld", loadWorld);
    def("saveWorld", saveWorld);
    def("getBlockName", getBlockName);
    def("getPlayerStartX", getPlayerStartX);
    def("getPlayerStartY", getPlayerStartY);
    def("getPlayerStartZ", getPlayerStartZ);
    def("setSeed", setSeed);
    def("setRenderDistance", setRenderDistance);
    def("setBlockWithoutBlockCheck", setBlockWithoutBlockCheck);

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
            .def("expand", &AABB::expand)
            ;

    class_<Chunk>("Chunk", init<int, int, int, Level*, dict>())
            .def("erase", &Chunk::erase)
            .def("update", &Chunk::update)
            .def("haveToRenderBlock", &Chunk::haveToRenderBlock)
            .def("render", &Chunk::render)
            ;
}
