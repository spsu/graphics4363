#ifndef MATHLIBS
#define MATHLIBS

/**
 * This file contains the library functions we have been given 
 * throughout the course thus far.
 */


#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

namespace math 
{

	/**
	 * Allocate a new identity matrix
	 * XXX: Must call delete to deallocate.
	 */
	GLfloat* newIdentityMat();

	/**
	 * Copy a matrix. (Only 4x4)
	 * XXX: Must call delete to dealloc.
	 */
	GLfloat* copyMat(GLfloat* f);


	// Found on OpenGL wiki. Replaces fixed function GLU's Perspective. 
	void glhPerspectivef2(GLfloat *matrix, GLfloat fovyInDegrees, 
			GLfloat aspectRatio, GLfloat znear, GLfloat zfar);

		// Found on OpenGL wiki. Replaces fixed function GLU's Frustrum. 
	void glhFrustumf2(GLfloat *matrix, GLfloat left, GLfloat right, GLfloat bottom, 
			GLfloat top, GLfloat znear, GLfloat zfar);

	void makePerspectiveProjectionMatrix(GLfloat* result, GLfloat fov, 
			GLfloat aspect, GLfloat _near, GLfloat _far); 
		
	// This is a column-order matrix multiply of matrices m1 and m2.
	void matrixMult4x4 (GLfloat* result, GLfloat* m1, GLfloat* m2); 

	void translate (GLfloat* result, GLfloat x, GLfloat y, GLfloat z);
	
	void scale(GLfloat* result, GLfloat x, GLfloat y, GLfloat z);

	void rotateX (GLfloat* result, GLfloat rotation);

	void rotateY (GLfloat* result, GLfloat rotation);

	void rotateZ (GLfloat* result, GLfloat rotation);

	// Only prints 4x4
	void printMat(GLfloat* mat);

}

#endif
