#include "vao.hpp"

VertexArray::VertexArray() :
	vao(0),
	vbo_vertex(0),
	vbo_color(0)
{
	glGenVertexArrays(1, &vao); // Create only one
	glBindVertexArray(vao);

	// TODO: Once I understand better, allocate together into an array. 
	glGenBuffers(1, &vbo_vertex);
	glGenBuffers(1, &vbo_color);
}

VertexArray::~VertexArray()
{
	// TODO
}

// TODO: Use a registry to hold programId. 
void VertexArray::loadVertices(GLfloat* vertices, GLuint num, GLint programId)
{
	GLuint loc(0);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);

	// TODO: GL_STATIC_DRAW alternatives
	// TODO: Use an std::vector.
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), vertices, 
			GL_STATIC_DRAW);

	// TODO: ShaderProgramId... Use a registry. 
	loc = glGetAttribLocation(programId, "vPosition");

	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

// TODO: Use a registry to hold programId.
void VertexArray::loadColors(GLfloat* colors, GLuint num, GLint programId)
{
	GLuint loc(0);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);

	// TODO: GL_STATIC_DRAW alternatives
	// TODO: Use an std::vector.
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), colors, 
			GL_STATIC_DRAW);

	// TODO: ShaderProgramId... Use a registry. 
	loc = glGetAttribLocation(programId, "vColor");

	glEnableVertexAttribArray(loc);

	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

