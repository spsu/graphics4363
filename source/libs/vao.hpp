#ifndef BT_GRAPHICS_LIB_VAO
#define BT_GRAPHICS_LIB_VAO

/**
 * Object-oriented VAOs. 
 *
 * Vertex Array Objects group together data that would normally be
 * present in distinct Vertex Buffer Objects. This is so that the data
 * will be kept together on a per-vertex basis. (eg. vertices and 
 * their normals. XXX: Perhaps texture mappings too?)
 *
 * VAO data lives directly on the graphics card and is thus rendered 
 * blindingly fast. 
 *
 * 		vertices -> VBO -> VAO (with other data)
 */

#include <GL/glew.h>
#include <GL/glut.h>

class VertexArray
{
	public:

		/**
		 * CTOR.
		 */
		VertexArray();

		/**
		 * DTOR.
		 */
		~VertexArray();

		/**
		 * Load data.
		 */
		void loadVertices(GLfloat* vertices, GLuint num, GLint programId);
		void loadColors(GLfloat* colors, GLuint num, GLint programId);

	private: 

		/**
		 * VAO handle.
		 */
		GLuint vao;

		/**
		 * VBO handles
		 */
		GLuint vbo_vertex;
		GLuint vbo_color; // TODO: Normals, etc.
};

#endif
