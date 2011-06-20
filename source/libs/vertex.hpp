#ifndef BT_GRAPHICS_LIBS_VERTEX
#define BT_GRAPHICS_LIBS_VERTEX

#include <GL/glew.h>
#include <GL/glut.h>

GLuint createVertexArray();

GLuint loadVertexPositions(GLuint shaderProgramID, GLuint vao, 
		GLfloat* vVerts, GLuint numVertices); 

GLuint loadColors (GLuint shaderProgramID, GLuint vao, 
		GLfloat* vColors, GLuint numVertices);

#endif
