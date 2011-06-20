#include "vertex.hpp"
#include <stdio.h>

// This function asks the driver for a vertex array object, which will store information
// about what is being drawn, such as geometry, color information, texture information and so on.
// Hold on to the ID that this function returns, because you can use it later on!
GLuint createVertexArray() {
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	return vao;
}

// This function takes in a vertex array object ID and an array of vertices. It should be called 
// immediately after calling createVertexArray.  It returns a buffer ID (which you probably won't
// use).  NOTE: this assumes that you want triangles and not points or lines.
GLuint loadVertexPositions(GLuint shaderProgramID, GLuint vao, GLfloat* vVerts, GLuint numVertices) {
	glBindVertexArray(vao);
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer (GL_ARRAY_BUFFER, buffer);
	
	// Copy the array data into the active buffer (remember, there's a GLuint above that's active)
	glBufferData(GL_ARRAY_BUFFER, numVertices*3*sizeof(GLfloat), vVerts, GL_STATIC_DRAW);
	
	// Figure out where vPosition is in our shader
	GLuint loc = glGetAttribLocation(shaderProgramID, "vPosition");
	// Turn it on
	glEnableVertexAttribArray(loc);
	// Tell it how the data should be parsed
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	return buffer;
}

GLuint loadColors (GLuint shaderProgramID, GLuint vao, GLfloat* vColors, GLuint numVertices) {
	glBindVertexArray(vao);
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer (GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, numVertices*3*sizeof(GLfloat), vColors, GL_STATIC_DRAW);

	GLuint loc = glGetAttribLocation(shaderProgramID, "vColor");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	return buffer;
}

