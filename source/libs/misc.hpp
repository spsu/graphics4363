#ifndef BT_GRAPHICS_LIBS
#define BT_GRAPHICS_LIBS

/**
 * This file contains the library functions we have been given 
 * throughout the course thus far.
 */

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>

const char* readFile (const char* filename);

// TODO: Documentation.
bool compiledStatus(GLint shaderID);

GLint makeVertexShader (char* shaderSource);

GLint makeFragmentShader (char* shaderSource);

bool linkedStatus(GLint programID);

GLuint makeShaderProgram (GLint vertexShaderID, GLint fragmentShaderID);


GLuint createVertexArray();


GLuint loadVertexPositions(GLuint shaderProgramID, GLuint vao, 
		GLfloat* vVerts, GLuint numVertices); 

GLuint loadColors (GLuint shaderProgramID, GLuint vao, 
		GLfloat* vColors, GLuint numVertices);

#endif
