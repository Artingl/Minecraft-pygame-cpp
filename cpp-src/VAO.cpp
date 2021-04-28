#include <cstdio>
#include "VAO.h"
#include <GL/glew.h>
#include "debug.h"


GLuint vbo[4] = {(GLuint) -1, (GLuint) -1, (GLuint) -1, (GLuint) -1};
GLuint vao[4] = {(GLuint) -1, (GLuint) -1, (GLuint) -1, (GLuint) -1};
const GLfloat vao_pos[]=
        {
//  x    y    z
                -1.0,-1.0,-1.0,
                +1.0,-1.0,-1.0,
                +1.0,+1.0,-1.0,
                -1.0,+1.0,-1.0,
                -1.0,-1.0,+1.0,
                +1.0,-1.0,+1.0,
                +1.0,+1.0,+1.0,
                -1.0,+1.0,+1.0,
        };

const GLfloat vao_col[]=
        {
//  r   g   b
                0.0,0.0,0.0,
                1.0,0.0,0.0,
                1.0,1.0,0.0,
                0.0,1.0,0.0,
                0.0,0.0,1.0,
                1.0,0.0,1.0,
                1.0,1.0,1.0,
                0.0,1.0,1.0,
        };

const GLuint vao_ix[]=
        {
                0,1,2,3,
                4,5,6,7,
                0,1,5,4,
                1,2,6,5,
                2,3,7,6,
                3,0,4,7,
        };

void VAO_INIT() {
    _gl_engine_info("VBO_INIT", "Initializing VBO...");

    printf("[FUCKOFF] %d\n", glGenVertexArrays);

    GLuint i;
    //glGenVertexArrays(4,vao);
    //glGenBuffers(4,vbo);
    //glBindVertexArray(vao[0]);
    //i=0; // vertex
    //glBindBuffer(GL_ARRAY_BUFFER,vbo[i]);
    //glBufferData(GL_ARRAY_BUFFER,sizeof(vao_pos),vao_pos,GL_STATIC_DRAW);
    //glEnableVertexAttribArray(i);
    //glVertexAttribPointer(i,3,GL_FLOAT,GL_FALSE,0,0);
    //i=1; // indices
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[i]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(vao_ix),vao_ix,GL_STATIC_DRAW);
    //glEnableVertexAttribArray(i);
    //glVertexAttribPointer(i,4,GL_UNSIGNED_INT,GL_FALSE,0,0);
    //i=2; // normal
    //i=3; // color
    //glBindBuffer(GL_ARRAY_BUFFER,vbo[i]);
    //glBufferData(GL_ARRAY_BUFFER,sizeof(vao_col),vao_col,GL_STATIC_DRAW);
    //glEnableVertexAttribArray(i);
    //glVertexAttribPointer(i,3,GL_FLOAT,GL_FALSE,0,0);
//
    //glBindVertexArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER,0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
//  //glDisableVertexAttribArray(2);
    //glDisableVertexAttribArray(3);

}


void VAO_EXIT()
{
    glDeleteVertexArrays(4,vao);
    glDeleteBuffers(4,vbo);
}

void VAO_DRAW()
{
    glBindVertexArray(vao[0]);
    //  glDrawArrays(GL_LINE_LOOP,0,8);                 // lines ... no indices
    glDrawElements(GL_QUADS,24,GL_UNSIGNED_INT,0);  // indices (choose just one line not both !!!)
    glBindVertexArray(0);
}
