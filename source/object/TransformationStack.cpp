#include "TransformationStack.hpp"
#include "../libs/math.hpp"

TransformationStack::TransformationStack()
{
	GLfloat* first = math::newIdentityMat(); 
	matrixStack.push(first);
}

void TransformationStack::push()
{
	matrixStack.push(copyTop());
}

void TransformationStack::pop()
{
	GLfloat* prev(0);

	prev = matrixStack.top();
	matrixStack.pop();

	delete prev; // XXX: Careful. Warn about shared ptr in doc.
}

GLfloat* TransformationStack::top()
{
	return matrixStack.top();
}

GLfloat* TransformationStack::copyTop()
{
	GLfloat* copy = new GLfloat[16];
	GLfloat* top = matrixStack.top(); 

	for(unsigned int i = 0; i < 16; i++) {
		copy[i] = top[i];
	}
	return copy;
}

// Replace 'top' with a translated top
void TransformationStack::translate(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat* top = matrixStack.top();
	GLfloat* trans = new GLfloat[16];
	GLfloat* newTop = new GLfloat[16];

	math::translate(trans, x, y, z);
	math::matrixMult4x4(newTop, top, trans);
	//math::matrixMult4x4(newTop, trans, top);

	matrixStack.pop();
	matrixStack.push(newTop);
	//GLfloat* temp = copyTop();
	//matrixStack.push(temp);

	delete top;
}

