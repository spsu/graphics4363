#ifndef BT_GRAPHICS_GEOMETRY_BASIC
#define BT_GRAPHICS_GEOMETRY_BASIC

/**
 * Header-only declaration of some basic geometry.
 */

#include <vector>
#include <stdio.h>

// TODO: Const
const GLfloat TRIANGLE_A[] = 
{
		0.1f, 0.2f, 0.0f,
		-0.1f, 0.0f, 0.0f,
		0.1f, -0.3f, 0.0f
};

// TODO: Const
const GLfloat LETTER_B[108] =
{
		-0.5f, 0.5f, 0.0f,		// t1
		-0.5f, -0.6f, 0.0f,
		-0.3f, -0.6f, 0.0f,
		-0.3f, 0.5f, 0.0f,		// t2
		-0.5f, 0.5f, 0.0f,
		-0.3f, -0.6f, 0.0f,
		0.3f, 0.2f, 0.0f,		// t3
		-0.3f, 0.5f, 0.0f,
		-0.3f, 0.2f, 0.0f,
		-0.3f, 0.5f, 0.0f,		// t4
		0.3f, 0.2f, 0.0f,
		0.3f, 0.5f, 0.0f,
		0.1f, 0.2f, 0.0f,		// t5
		0.1f, 0.0f, 0.0f,
		0.3f, 0.0f, 0.0f,
		0.1f, 0.2f, 0.0f,		// t6
		0.3f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.0f,
		-0.3f, 0.0f, 0.0f,		// t7
		-0.3f, -0.2f, 0.0f,
		0.5f, -0.2f, 0.0f,
		-0.3f, 0.0f, 0.0f,		// t8
		0.5f, -0.2f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.3f, -0.2f, 0.0f,		// t9
		0.3f, -0.4f, 0.0f,
		0.5, -0.4f, 0.0f,
		0.3f, -0.2f, 0.0f,		// t10
		0.5f, -0.4f, 0.0f,
		0.5f, -0.2f, 0.0f,
		-0.3f, -0.4f, 0.0f,		// t11
		-0.3f, -0.6f, 0.0f,
		0.5f, -0.6f, 0.0f,
		-0.3f, -0.4f, 0.0f,		// t12
		0.5f, -0.6f, 0.0f,
		0.5f, -0.4f, 0.0f,
};

// Lazy vertex debug 
void printVertices(vector<GLfloat> verts)
{
	int count = 0;
	for(unsigned int i = 0; i < verts.size(); i++) {
		printf("%f, ", verts.at(i));
		count += 1;
		if(count %3 == 0) {
			printf("\n");
		}
	}
};

#endif
