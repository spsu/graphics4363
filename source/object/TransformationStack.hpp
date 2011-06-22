#ifndef BT_GRAPHICS_OBJECT_TRANSFORMATIONSTACK
#define BT_GRAPHICS_OBJECT_TRANSFORMATIONSTACK

#include <GL/glew.h>
#include <GL/glut.h>
#include <stack>

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
		 */
		void translate(GLfloat x, GLfloat y, GLfloat z);

	private:

		/**
		 * Stack of matrix transformations.
		 */
		std::stack<GLfloat*> matrixStack;

		/**
		 * Current matrix that is being operated on.
		 * It can be saved to stack by calling save()
		 */
		GLfloat* currentMatrix;

		/**
		 * Create a copy of the top matrix
		 */
		GLfloat* copyTop();
};


#endif
