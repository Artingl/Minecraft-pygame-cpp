#include <boost/python.hpp>
#include "GL/Texture.h"
#include "GL/Chunk.h"

std::vector<Chunk> chunks;
int renderDistance;

void _gl_engine_init(int _renderDistance)
{
    _gl_engine_info("_gl_engine_init", "Initializing engine...");

    glewInit();
    renderDistance = _renderDistance;

    for (int x = -renderDistance; x <= renderDistance; ++x)
    {
        for (int z = -renderDistance; z <= renderDistance; ++z)
        {
            chunks.push_back(Chunk(x, z));
        }
    }

}

void _gl_engine_quit() {
    _gl_engine_info("_gl_engine_quit", "Quit the game");
}

void _gl_engine_draw(int terraintTextureId) {
    //_gl_engine_info("_gl_engine_draw", "Draw stuff");

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glBindTexture(GL_TEXTURE_2D, terraintTextureId);
    glColor3f(1, 1, 1);
    for (Chunk chunk : chunks)
    {
        glPushMatrix();
        glTranslatef(chunk.x * chunk_width, 0, chunk.z * chunk_depth);
        chunk.render();
        glPopMatrix();
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

BOOST_PYTHON_MODULE(opengl_main_cpp)
{
    using namespace boost::python;

    def("_gl_engine_init", _gl_engine_init);
    def("_gl_engine_draw", _gl_engine_draw);
    def("_gl_engine_quit", _gl_engine_quit);
}
