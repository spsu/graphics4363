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
#include "../libs/types.hpp"

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
		 * Define transformations for the object.
		 * Setting up the matrix is CPU-bound, and does not occur on
		 * the GPU. Calculations occur in the draw() function.  
		 */
		void rotate(GLfloat x, GLfloat y, GLfloat z);
		void scale(GLfloat x, GLfloat y, GLfloat z);
		void translate(GLfloat x, GLfloat y, GLfloat z);

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

		/**
		 * Type of primitive.
		 * GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_QUADS, etc.
		 */
		GLuint primitiveMode;

		/**
		 * Saved transformation data.
		 */
		Vertex vRot;
		Vertex vScale;
		Vertex vTrans;

		/**
		 * Matrix representing the transformation data.
		 */
		GLfloat* mTransform;
		bool recalcMat;
};

#endif
