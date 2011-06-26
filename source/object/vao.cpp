#include "vao.hpp"
#include "../shaderlib/registry.hpp"
#include "../libs/math.hpp"
#include "../libs/image.hpp"
#include "../3rdparty/stb_image.h"
#include "TransformationStack.hpp"
#include "TransformationStackRegistry.hpp"
#include <stdio.h>
#include <stdlib.h>

VertexArray::VertexArray() :
	vao(0),
	vbo_vertex(0),
	vbo_color(0),
	vbo_textureCoords(0),
	textureId(0),
	numVertices(0),
	primitiveMode(GL_TRIANGLES),
	vRot(),
	vScale(1.0f, 1.0f, 1.0f),
	vTrans(),
	mTransform(0),
	recalcMat(true) // Must calculate on first draw
{
	glGenVertexArrays(1, &vao); // Create only one
	glBindVertexArray(vao);

	// TODO: Once I understand better, allocate together into an array. 
	// TODO: For now, we should allocate these as they are requested, 
	// not upfront
	glGenBuffers(1, &vbo_vertex);
	glGenBuffers(1, &vbo_normal);
	glGenBuffers(1, &vbo_color);
	glGenBuffers(1, &vbo_textureCoords);
	glGenTextures(1, &textureId);

	// TODO: Temporary. This is how imported objs load.
	// Need a way to better specify this
	primitiveMode = GL_QUADS; 

	mTransform = new GLfloat[16];
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

void VertexArray::loadVertices(Lib3dsVector* vertices, int numFaces)
{
	GLuint loc(0);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector)*3*numFaces, vertices, 
			GL_STATIC_DRAW);

	loc = glGetAttribLocation(Registry::getProgramId(), "vPosition");
	glEnableVertexAttribArray(loc);

	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	numVertices = numFaces*3;
	primitiveMode = GL_TRIANGLES; 
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

void VertexArray::loadNormals(Lib3dsVector* normals, int numFaces)
{
	GLuint loc(0);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);

	// TODO: GL_STATIC_DRAW alternatives
	glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector)*3*numFaces, normals, 
			GL_STATIC_DRAW);

	loc = glGetAttribLocation(Registry::getProgramId(), "vNormal");
	glEnableVertexAttribArray(loc);

	// Associate vertices with currently bound buffer.
	// index, numComponents, type, doNormalization, stride, first item ptr
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void VertexArray::loadTextureCoords(Lib3dsTexel* texCoords, int numFaces)
{
	GLuint loc(0);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_textureCoords);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsTexel)*3*numFaces, texCoords, 
			GL_STATIC_DRAW);

	loc = glGetAttribLocation(Registry::getProgramId(), "vTextureCoord");
	glEnableVertexAttribArray(loc);

	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

	// Associate vertices with currently bound buffer.
	// index, numComponents, type, doNormalization, stride, first item ptr
	//glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(36*sizeof(GLfloat))); // XXX: from slides
	//glTexCoordPointer(2, GL_FLOAT, 0, NULL); // XXX XXX FIXME: Is this right?
}

void VertexArray::loadTexture(std::string filename)
{
	GLuint loc(0);
	int width = 0;
	int height = 0;
	int size = 0;
	int channels = 0;
	unsigned char* pixData = 0;
	unsigned char* pixData2 = 0;

	glBindTexture(GL_TEXTURE_2D, textureId);

	// Set wrapping and filtering modes
	// XXX: Try other parameter options. 
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	pixData = stbi_load(filename.c_str(), &width, &height, &channels, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 
					0, GL_RGBA, GL_UNSIGNED_BYTE, pixData);

	stbi_image_free(pixData);

	// TODO: CLEANUP on delete VAO:, glDeleteTextures(TEXTURE_COUNT, textures)

	glGenerateMipmap(GL_TEXTURE_2D);

	// XXX: I don't understand this... 
	//glActiveTexture(GL_TEXTURE0);
	loc = glGetUniformLocation(Registry::getProgramId(), "texture");

	// Bind to texture 0
	glUniform1i(loc, 0);

	//glEnableVertexAttribArray(loc);
	// XXX: ????
	glActiveTexture(GL_TEXTURE0);
}

void VertexArray::rotate(GLfloat x, GLfloat y, GLfloat z)
{
	vRot = Vertex(x, y, z);
	recalcMat = true;
}

void VertexArray::scale(GLfloat x, GLfloat y, GLfloat z)
{
	vScale = Vertex(x, y, z);
	recalcMat = true;
}

void VertexArray::translate(GLfloat x, GLfloat y, GLfloat z)
{
	vTrans = Vertex(x, y, z);
	recalcMat = true;
}

void VertexArray::scale(GLfloat xyz)
{
	vScale = Vertex(xyz, xyz, xyz);
	recalcMat = true;
}


void VertexArray::setPrimitiveMode(GLuint pMode)
{
	primitiveMode = pMode;
}

void VertexArray::draw()
{
	// Graphics matrix stack pipeline.
	TransformationStack* tStack = TransformationStackRegistry::get();
	tStack->push();


	//if(recalcMat) 
	{
		//translate(mTransform, vTrans.x, vTrans.y, vTrans.z);
		//math::translate(mTransform, 0.0f, 0.0f, 0.0f);
		
		tStack->translate(vTrans.x, vTrans.y, vTrans.z);
		tStack->scale(vScale.x, vScale.y, vScale.z);
		tStack->rotate(vRot.x, vRot.y, vRot.z);
		tStack->applyTransform();
		//tStack->translate(0.0f, -5.0f, -5.0f);
		recalcMat = false;
	}

	glBindVertexArray(vao);

	glBindTexture(GL_TEXTURE_2D, textureId); // TODO TODO TODO TODO  TODO TODO  TODO TODO  TODO TODO  TODO TODO 

	GLuint r = glGetUniformLocation(Registry::getProgramId(), "mv");
	//glUniformMatrix4fv(r, 1, GL_TRUE, mTransform);
	glUniformMatrix4fv(r, 1, GL_TRUE, tStack->top());

	glDrawArrays(primitiveMode, 0, numVertices);

	tStack->pop();
}


