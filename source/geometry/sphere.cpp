#include "sphere.hpp"
#include <math.h>

// XXX: From http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=21027

std::vector<GLfloat> makeSphere(float radius, int STACKS)
{
	GLfloat r, x, y, z;
	//float vertex;
	std::vector<GLfloat> vertices;

	int SLICES = STACKS; // XXX: How does this affect things? 
	//STACKS = 50;
	//SLICES = 50;

	GLfloat PI = atan(1)*4;

	for (int stack = 0; stack < STACKS; ++stack) {
		for (int slice = 0; slice < SLICES; ++slice) {
			y = 2.0 * stack / STACKS - 1.0;
			/* for better distribution, use y = -cos(PI * stack / STACKS) */
			r = sqrt(1 - pow(y, 2));
			x = r * sin(2.0 * PI * slice / SLICES);
			z = r * cos(2.0 * PI * slice / SLICES);

			//vertex = radius * (x, y, z);

			vertices.push_back((GLfloat)radius * x);
			vertices.push_back((GLfloat)radius * y);
			vertices.push_back((GLfloat)radius * z);
			//vertices.push_back((GLfloat)1.0f);
		}
	}

	return vertices;
}
