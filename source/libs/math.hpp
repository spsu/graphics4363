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

void makePerspectiveProjectionMatrix(GLfloat* result, GLfloat fov, 
		GLfloat aspect, GLfloat _near, GLfloat _far); 
	
// This is a column-order matrix multiply of matrices m1 and m2.
void matrixMult4x4 (GLfloat* result, GLfloat* m1, GLfloat* m2); 

void translate (GLfloat* result, GLfloat x, GLfloat y, GLfloat z);

void rotateX (GLfloat* result, GLfloat rotation);

void rotateY (GLfloat* result, GLfloat rotation);

void rotateZ (GLfloat* result, GLfloat rotation);

void print4x4Matrix(GLfloat* mat);

#endif