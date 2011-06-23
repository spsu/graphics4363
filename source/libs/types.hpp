#ifndef BT_GRAPHICS_LIBS_TYPES
#define BT_GRAPHICS_LIBS_TYPES

struct Vertex
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;

	Vertex() :
		x(0.0f), y(0.0f), z(0.0f), w(1.0f) {};

	Vertex(GLfloat px, GLfloat py, GLfloat pz) :
		x(px), y(py), z(pz), w(1.0f) {};

	Vertex(GLfloat px, GLfloat py, GLfloat pz, GLfloat pw) :
		x(px), y(py), z(pz), w(pw) {};
};

#endif
