#include "TransformationStack.hpp"
#include "../libs/math.hpp"
#include <stdio.h>

TransformationStack::TransformationStack():
	matrixStack(),
	vRot(),
	vScale(1.0f, 1.0f, 1.0f),
	vTrans()
{
	GLfloat* first = math::newIdentityMat(); 
	matrixStack.push(first);
}

void TransformationStack::push()
{
	matrixStack.push(copyTop());

	// Pending state changes no longer apply
	vRot = Vertex();
	vScale = Vertex(1.0f, 1.0f, 1.0f);
	vTrans = Vertex();
}

void TransformationStack::pop()
{
	GLfloat* prev(0);

	prev = matrixStack.top();
	matrixStack.pop();

	// Pending state changes no longer apply
	vRot = Vertex();
	vScale = Vertex(1.0f, 1.0f, 1.0f);
	vTrans = Vertex();

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

void TransformationStack::rotate(GLfloat x, GLfloat y, GLfloat z)
{
	vRot = Vertex(x, y, z);
}

void TransformationStack::scale(GLfloat x, GLfloat y, GLfloat z)
{
	vScale = Vertex(x, y, z);
}

void TransformationStack::translate(GLfloat x, GLfloat y, GLfloat z)
{
	vTrans = Vertex(x, y, z);
}


void TransformationStack::applyTransform()
{
	GLfloat* top = matrixStack.top();
	GLfloat* newTop = new GLfloat[16];

	// Intermediate matrices. 
	GLfloat* rotX = new GLfloat[16];
	GLfloat* rotY = new GLfloat[16];
	GLfloat* rotZ = new GLfloat[16];
	GLfloat* rotXY = new GLfloat[16];
	GLfloat* rotXYZ = new GLfloat[16];
	GLfloat* trans = new GLfloat[16];
	GLfloat* scale = new GLfloat[16];

	// Combination.
	GLfloat* scaleRot = new GLfloat[16];
	GLfloat* srt = new GLfloat[16];

	// Rotation
	math::rotateX(rotX, vRot.x);
	math::rotateY(rotY, vRot.y);
	math::rotateZ(rotZ, vRot.z);
	math::matrixMult4x4(rotXY, rotX, rotY);
	math::matrixMult4x4(rotXYZ, rotXY, rotZ);

	// Scale and Translation. 
	math::scale(scale, vScale.x, vScale.y, vScale.z);
	math::translate(trans, vTrans.x, vTrans.y, vTrans.z);

	// Combine. Order is S*R*T
	math::matrixMult4x4(scaleRot, scale, rotXYZ);
	math::matrixMult4x4(srt, scaleRot, trans);
	
	// Final combine. 
	math::matrixMult4x4(newTop, srt, top);
	matrixStack.top() = newTop;

	// State changes were applied, so reset the pending state buffers
	vRot = Vertex();
	vScale = Vertex(1.0f, 1.0f, 1.0f);
	vTrans = Vertex();

	// TODO: Cleanup ALL state.
	// TODO: Do this stuff on the stack, not the heap. 
	// TODO: Only create matrices and do matrix math when absolutely 
	// necessary to reduce overhead. 

	delete top; // XXX: Warn callee not to hold onto this. 

	delete rotX;
	delete rotY;
	delete rotZ;
	delete rotXY;
	delete rotXYZ;
	delete scale;
	delete trans;
	delete scaleRot;
	delete srt;
}

