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
#include <vector>

/**
 * TODO: Rename 'Model' or 'Geometry'
 *
 * TODO: Figure out how to clone, turn "on" and "off", how to translate,
 * rotate, etc only a single object. 
 */
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
		void loadVertices(std::vector<GLfloat> vertices);
		void loadVertices(GLfloat* vertices, GLuint num);
		void loadColors(GLfloat* colors, GLuint num);

		void loadNormals(std::vector<GLfloat> normals);

		/**
		 * Draw.
		 */
		void draw();

	private: 

		/**
		 * VAO handle.
		 */
		GLuint vao;

		/**
		 * VBO handles
		 */
		GLuint vbo_vertex;
		GLuint vbo_color;
		GLuint vbo_normal;

		/**
		 * Number of vertices. 
		 */
		int numVertices;
};

#endif
