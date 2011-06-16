#include "math.hpp"

/**
 * This file contains the library functions we have been given 
 * throughout the course thus far.
 */

#define T0 0
#define T1 4
#define T2 8
#define T3 12
#define T4 1
#define T5 5
#define T6 9
#define T7 13
#define T8 2
#define T9 6
#define T10 10
#define T11 14
#define T12 3
#define T13 7
#define T14 11
#define T15 15

void makePerspectiveProjectionMatrix(GLfloat* result, GLfloat fov, GLfloat aspect, GLfloat _near, GLfloat _far) {
	GLfloat f = 1.0f/tan(fov*3.1415926f/360.0f); //(float)(atan(fov)/2.0);
	for (int i = 0; i < 16; i++) {
		result[i] = 0.0f;
	}
	result[0] = f/aspect;
	result[5] = f;
	result[10] = ((_far+_near)/(_near-_far));
	result[11] = -1;
	result[14] = (2.0f*_far*_near)/(_near-_far);
	result[15] = 0;
}

// This is a column-order matrix multiply of matrices m1 and m2.
void matrixMult4x4 (GLfloat* result, GLfloat* m1, GLfloat* m2) {
	for (int i = 0; i < 16; i++) {
		result[i] = 0.0f;
	}
	result[T0] = m1[T0]*m2[T0]+m1[T1]*m2[T4]+m1[T2]*m2[T8]+m1[T3]*m2[T12];
	result[T1] = m1[T0]*m2[T1]+m1[T1]*m2[T5]+m1[T2]*m2[T9]+m1[T3]*m2[T13];
	result[T2] = m1[T0]*m2[T2]+m1[T1]*m2[T6]+m1[T2]*m2[T10]+m1[T3]*m2[T14];
	result[T3] = m1[T0]*m2[T3]+m1[T1]*m2[T7]+m1[T2]*m2[T11]+m1[T3]*m2[T15];

	result[T4] = m1[T4]*m2[T0]+m1[T5]*m2[T4]+m1[T6]*m2[T8]+m1[T7]*m2[T12];
	result[T5] = m1[T4]*m2[T1]+m1[T5]*m2[T5]+m1[T6]*m2[T9]+m1[T7]*m2[T13];
	result[T6] = m1[T4]*m2[T2]+m1[T5]*m2[T6]+m1[T6]*m2[T10]+m1[T7]*m2[T14];
	result[T7] = m1[T4]*m2[T3]+m1[T5]*m2[T7]+m1[T6]*m2[T11]+m1[T7]*m2[T15];

	result[T8] = m1[T8]*m2[T0]+m1[T9]*m2[T4]+m1[T10]*m2[T8]+m1[T11]*m2[T12];
	result[T9] = m1[T8]*m2[T1]+m1[T9]*m2[T5]+m1[T10]*m2[T9]+m1[T11]*m2[T13];
	result[T10] = m1[T8]*m2[T2]+m1[T9]*m2[T6]+m1[T10]*m2[T10]+m1[T11]*m2[T14];
	result[T11] = m1[T8]*m2[T3]+m1[T9]*m2[T7]+m1[T10]*m2[T11]+m1[T11]*m2[T15];

	result[T12] = m1[T12]*m2[T0]+m1[T13]*m2[T4]+m1[T14]*m2[T8]+m1[T15]*m2[T12];
	result[T13] = m1[T12]*m2[T1]+m1[T13]*m2[T5]+m1[T14]*m2[T9]+m1[T15]*m2[T13];
	result[T14] = m1[T12]*m2[T2]+m1[T13]*m2[T6]+m1[T14]*m2[T10]+m1[T15]*m2[T14];
	result[T15] = m1[T12]*m2[T3]+m1[T13]*m2[T7]+m1[T14]*m2[T11]+m1[T15]*m2[T15];
}

void translate (GLfloat* result, GLfloat x, GLfloat y, GLfloat z) {
	
	for (int i = 0; i < 16; i++) {
		result[i] = 0.0f;
	}
	result[0] = result[5] = result[10] = result[15]= 1.0f;
	result[3] = x;
	result[7] = y;
	result[11] = z;
	
}

void rotateX (GLfloat* result, GLfloat rotation) {
	for (int i = 0; i < 16; i++) {
		result[i] = 0.0f;
	}
	result[5] = result[10] = cos(rotation);
	result[6] = sin(rotation);
	result[9] = -result[6];
	result[0] = result[15] = 1;

}

void rotateY (GLfloat* result, GLfloat rotation) {
	for (int i = 0; i < 16; i++) {
		result[i] = 0.0f;
	}
	result[0] = result[10] = cos(rotation);
	result[2] = -sin(rotation);
	result[8] = -result[2];
	result[5] = result[15] = 1;
}

void rotateZ (GLfloat* result, GLfloat rotation) {
	for (int i = 0; i < 16; i++) {
		result[i] = 0.0f;
	}
	result[0] = result[5] = cos(rotation);
	result[1] = sin(rotation);
	result[4] = -result[1];
	result[10] = result[15] = 1;
}
void print4x4Matrix(GLfloat* mat) {
	printf ("====================================");
	for (int i = 0; i < 16; i++) {
		if (i%4 == 0) printf ("\n");
		printf ("%f\t", mat[i]);
	}
	printf ("\n");
}


