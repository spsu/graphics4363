#include "TransformationStack.hpp"
#include "../libs/math.hpp"
#include <stdio.h>

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

unsigned int TransformationStack::size()
{
	return matrixStack.size();
}

GLfloat* TransformationStack::top()
{
	return matrixStack.top();
}

GLfloat* TransformationStack::copyTop()
{
	// XXX: Must deallocate. 
	return math::copyMat(matrixStack.top());	
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

	matrixStack.top() = newTop;
	//matrixStack.top() = trans;

	//matrixStack.pop();
	//matrixStack.push(newTop);
	//GLfloat* temp = copyTop();
	//matrixStack.push(temp);
	//

	//printf("Mat size: %d\n", size());
	//math::print4x4Matrix(tran);

	delete top;
}

