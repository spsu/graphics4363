#ifndef BT_GRAPHICS_LIBS_VERTEX
#define BT_GRAPHICS_LIBS_VERTEX

/**
 * Vertex Loading, Color Loading.  
 *
 * These library functions have been adapted from those given to us
 * throughout the course thus far. 
 */

#include <GL/glew.h>
#include <GL/glut.h>

GLuint createVertexArray();

GLuint loadVertexPositions(GLuint shaderProgramID, GLuint vao, 
		GLfloat* vVerts, GLuint numVertices); 

GLuint loadColors (GLuint shaderProgramID, GLuint vao, 
		GLfloat* vColors, GLuint numVertices);

#endif
