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
#include <lib3ds/types.h>
#include <string>
#include "../libs/types.hpp"

/**
 * FIXME: Rename to something else. I shouldn't really call this a VAO
 * anymore since I've added so much functionality to it. Perhaps 
 * 'Model' or 'Geometry' or similar with a suffix like 'Manager' etc.
 * FIXME: Also, the names of the directories in this project are lame
 * and confusing.
 *
 * TODO: Implement cloning. (Copy CTOR, etc.)
 * TODO: Instancing. 
 * TODO: Calling multiple objects to draw without OpenGL API overhead. 
 *
 * TODO: GL_STATIC_DRAW alternatives
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
		void loadVertices(Lib3dsVector* vertices, int numFaces);
		void loadVertices(GLfloat* vertices, GLuint num);
	
		/**
		 * Load normals.
		 */
		void loadNormals(std::vector<GLfloat> normals);
		void loadNormals(Lib3dsVector* vertices, int numFaces);

		/**
		 * Load colors.
		 */
		void loadColors(GLfloat* colors, GLuint num);

		/**
		 * Load texture coords.
		 */
		void loadTextureCoords(Lib3dsTexel* texCoords, int numFaces);

		/**
		 * Load texture.
		 * TODO
		 */
		void loadTexture(std::string filename);

		/**
		 * Define transformations for the object.
		 * Setting up the matrix is CPU-bound, and does not occur on
		 * the GPU. Calculations occur in the draw() function.  
		 */
		void rotate(GLfloat x, GLfloat y, GLfloat z);
		void scale(GLfloat x, GLfloat y, GLfloat z);
		void translate(GLfloat x, GLfloat y, GLfloat z);

		// Uniform scale on X, Y, and Z
		void scale(GLfloat xyz);

		/**
		 * Change primitive mode.
		 */
		void setPrimitiveMode(GLuint pMode);

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
		GLuint vbo_normal;
		GLuint vbo_color;
		GLuint vbo_textureCoords;
		GLuint textureId;

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
