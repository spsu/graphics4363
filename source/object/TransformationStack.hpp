#ifndef BT_GRAPHICS_OBJECT_TRANSFORMATIONSTACK
#define BT_GRAPHICS_OBJECT_TRANSFORMATIONSTACK

#include <GL/glew.h>
#include <GL/glut.h>
#include <stack>
#include "../libs/types.hpp"

class TransformationStack
{

	public:
		/**
		 * CTOR.
		 */
		TransformationStack();

		/**
		 * Returns the top of the stack.
		 */
		GLfloat* top();

		/**
		 * Push onto the stack.
		 */
		void push();

		/**
		 * Pop off the stack.
		 * XXX: Deletes the top. Beware of dangling pointers!
		 */
		void pop();

		unsigned int size();

		/**
		 * Translation functions.
		 * These are pending state changes that are committed to the 
		 * top of the stack when applyTransform is called.
		 */
		void rotate(GLfloat x, GLfloat y, GLfloat z);
		void scale(GLfloat x, GLfloat y, GLfloat z);
		void translate(GLfloat x, GLfloat y, GLfloat z);

		/**
		 * Applies state changes.
		 */
		void applyTransform();

	private:

		/**
		 * Stack of matrix transformations.
		 */
		std::stack<GLfloat*> matrixStack;

		/**
		 * Pending state changes to the top of the stack.
		 * Made when applyTransform() is called. 
		 */
		Vertex vRot;
		Vertex vScale;
		Vertex vTrans;

		/**
		 * Create a copy of the top matrix
		 */
		GLfloat* copyTop();
};


#endif
