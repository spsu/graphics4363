#include "vao.hpp"
#include "../shaderlib/registry.hpp"

VertexArray::VertexArray() :
	vao(0),
	vbo_vertex(0),
	vbo_color(0),
	numVertices(0)
{
	glGenVertexArrays(1, &vao); // Create only one
	glBindVertexArray(vao);

	// TODO: Once I understand better, allocate together into an array. 
	glGenBuffers(1, &vbo_vertex);
	glGenBuffers(1, &vbo_color);
	glGenBuffers(1, &vbo_normal);
}

VertexArray::~VertexArray()
{
	// TODO
}

void VertexArray::loadVertices(std::vector<GLfloat> vertices)
{
	GLuint loc(0);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);

	GLfloat verts2[vertices.size()];

	for(unsigned int i = 0; i < vertices.size(); i++) {
		verts2[i] = vertices.at(i);
	}

	// TODO: GL_STATIC_DRAW alternatives
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), verts2, 
			GL_STATIC_DRAW);

	loc = glGetAttribLocation(Registry::getProgramId(), "vPosition");

	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	numVertices = vertices.size()/3;
}

void VertexArray::loadVertices(GLfloat* vertices, GLuint num)
{
	GLuint loc(0);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);

	// TODO: GL_STATIC_DRAW alternatives
	// TODO: Use an std::vector.
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), vertices, 
			GL_STATIC_DRAW);

	loc = glGetAttribLocation(Registry::getProgramId(), "vPosition");

	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	numVertices = num;
}

void VertexArray::loadColors(GLfloat* colors, GLuint num)
{
	GLuint loc(0);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);

	// TODO: GL_STATIC_DRAW alternatives
	// TODO: Use an std::vector.
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), colors, 
			GL_STATIC_DRAW);

	loc = glGetAttribLocation(Registry::getProgramId(), "vColor");

	glEnableVertexAttribArray(loc);

	// index, numComponents, type, doNormalization, stride, first item ptr
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void VertexArray::loadNormals(std::vector<GLfloat> normals)
{
	GLuint loc(0);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);

	GLfloat data[normals.size()];

	for(unsigned int i = 0; i < normals.size(); i++) {
		data[i] = normals.at(i);
	}

	// TODO: GL_STATIC_DRAW alternatives
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), data, 
			GL_STATIC_DRAW);

	loc = glGetAttribLocation(Registry::getProgramId(), "vNormal");

	glEnableVertexAttribArray(loc);

	// Associate vertices with currently bound buffer.
	// index, numComponents, type, doNormalization, stride, first item ptr
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void VertexArray::draw()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_QUADS, 0, numVertices);
}


